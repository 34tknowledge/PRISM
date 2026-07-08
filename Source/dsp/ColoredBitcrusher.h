#pragma once

#include "Svf.h"
#include <cmath>

namespace prism
{
    /**
        PRISM's signature "colored" crusher. Not a hard clipper — it emulates a
        lo-fi AD/DA path and then re-analogs it:

            drive -> bit reduction (grid-bias + error-feedback noise shaping)
                  -> sample-rate reduction (fractional sample & hold)
                  -> asymmetric waveshaper (even harmonics) -> DC blocker
                  -> resonant TPT tone filter -> dry/wet mix

        The quantiser + decimator create the digital edges; the asymmetric
        saturator warms them with even harmonics; the resonant filter tames the
        aliasing and voices the grit. Design distilled from the D16 Decimort /
        Tritik Krush lineage.
    */
    class ColoredBitcrusher
    {
    public:
        void setSampleRate (double sr) noexcept
        {
            sampleRate = sr;
            for (auto& c : ch)
            {
                c.tone.setSampleRate (sr);
                c.dc.setR (1.0f - juce::MathConstants<float>::twoPi * 12.0f / (float) sr);
                c.reset();
            }
        }

        void reset() noexcept { for (auto& c : ch) c.reset(); }

        void setParams (float drive, float bits, float srFactor, float shape,
                        float toneHz, float reso, float mix) noexcept
        {
            driveGain    = std::pow (10.0f, (drive * 18.0f) / 20.0f);   // dB -> gain
            step         = 1.0f / std::pow (2.0f, juce::jlimit (1.0f, 16.0f, bits) - 1.0f);
            gridBias     = shape * 0.25f * step;
            rateInc      = 1.0f / juce::jmax (1.0f, srFactor);
            satDrive     = 1.0f + shape * 4.0f;
            asymB        = shape * 0.4f;
            wet          = mix;

            const float Q = 0.5f + reso * 7.5f;
            for (auto& c : ch) c.tone.setCutoffQ (toneHz, Q);
        }

        /** Process one stereo frame in place. */
        void process (float& l, float& r) noexcept
        {
            l = processSample (ch[0], l);
            r = processSample (ch[1], r);
        }

    private:
        struct Channel
        {
            SvfStage tone;
            DcBlocker dc;
            float quantErr = 0.0f, phase = 1.0f, held = 0.0f;
            void reset() noexcept { tone.reset(); dc.reset(); quantErr = 0.0f; phase = 1.0f; held = 0.0f; }
        };

        float processSample (Channel& c, float in) noexcept
        {
            const float dry = in;

            // 0. input drive
            float x = in * driveGain;

            // 1. bit reduction — error-feedback noise shaping + asymmetric grid bias
            x += noiseShape * c.quantErr;
            const float xb = x + gridBias;
            const float q  = std::round (xb / step) * step - gridBias;
            c.quantErr = q - x;
            x = q;

            // 2. sample-rate reduction (fractional S&H) — aliasing is intentional
            c.phase += rateInc;
            if (c.phase >= 1.0f) { c.phase -= 1.0f; c.held = x; }
            x = c.held;

            // 3. asymmetric waveshaper (even-harmonic warmth), re-centred
            x = std::tanh (satDrive * (x + asymB)) - std::tanh (satDrive * asymB);

            // 3b. DC blocker
            x = c.dc.process (x);

            // 4. resonant tone filter tames aliasing and voices the grit
            x = c.tone.processLP (x);

            // 5. dry / wet
            return dry * (1.0f - wet) + x * wet;
        }

        Channel ch[2];
        double sampleRate = 44100.0;
        float driveGain = 1.0f, step = 1.0f / 32768.0f, gridBias = 0.0f;
        float rateInc = 1.0f, satDrive = 1.0f, asymB = 0.0f, wet = 1.0f;
        static constexpr float noiseShape = 0.3f;
    };
}
