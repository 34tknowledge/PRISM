#pragma once

#include <cmath>
#include <juce_core/juce_core.h>

namespace prism
{
    inline float fracp (float x) noexcept { return x - std::floor (x); }

    /** PolyBLEP residual for band-limiting discontinuities (saw / pulse edges). */
    inline float polyBlep (float t, float dt) noexcept
    {
        if (dt <= 0.0f) return 0.0f;
        if (t < dt)               { t /= dt;              return t + t - t * t - 1.0f; }
        if (t > 1.0f - dt)        { t = (t - 1.0f) / dt;  return t * t + t + t + 1.0f; }
        return 0.0f;
    }

    /**
        Single anti-aliased oscillator. Saw and pulse/PWM are PolyBLEP-corrected;
        triangle and sine are generated directly (their aliasing is negligible in
        the musical range). Square is just a pulse locked to 50% width.
    */
    class PolyBlepOsc
    {
    public:
        enum Wave { Saw = 0, Square, Triangle, Sine, PWM };

        void setSampleRate (double sr) noexcept { sampleRate = sr; updateInc(); }
        void setFrequency (float f) noexcept    { freq = f; updateInc(); }
        void setWave (int w) noexcept           { wave = w; }
        void setPulseWidth (float pw) noexcept  { width = juce::jlimit (0.05f, 0.95f, pw); }
        void resetPhase (float p = 0.0f) noexcept { phase = fracp (p); }
        float getPhase() const noexcept { return phase; }
        bool wrappedThisSample() const noexcept { return wrapped; }

        float next() noexcept
        {
            const float t = phase;
            float value = 0.0f;

            switch (wave)
            {
                case Sine:
                    value = std::sin (juce::MathConstants<float>::twoPi * t);
                    break;

                case Saw:
                    value = 2.0f * t - 1.0f;
                    value -= polyBlep (t, inc);
                    break;

                case Square:
                    value = pulse (t, 0.5f);
                    break;

                case PWM:
                    value = pulse (t, width);
                    break;

                case Triangle:
                default:
                    // Naive triangle in [-1, 1]; peak at t=0.5, troughs at 0 and 1.
                    value = 1.0f - 4.0f * std::abs (t - 0.5f);
                    break;
            }

            phase += inc;
            wrapped = false;
            if (phase >= 1.0f) { phase -= 1.0f; wrapped = true; }
            return value;
        }

    private:
        float pulse (float t, float w) noexcept
        {
            float v = (t < w) ? 1.0f : -1.0f;
            v += polyBlep (t, inc);
            v -= polyBlep (fracp (t - w + 1.0f), inc);
            return v;
        }

        void updateInc() noexcept
        {
            inc = (sampleRate > 0.0) ? (float) (freq / sampleRate) : 0.0f;
            inc = juce::jlimit (0.0f, 0.49f, inc);
        }

        double sampleRate = 44100.0;
        float freq = 440.0f, inc = 0.0f, phase = 0.0f, width = 0.5f;
        int wave = Saw;
        bool wrapped = false;
    };

    /**
        Unison stack: up to 7 detuned copies of one oscillator, panned across the
        stereo field for a fat mono/lead tone. Voice 0 is the centre "master"
        (used as the hard-sync source).
    */
    class UnisonOsc
    {
    public:
        static constexpr int maxVoices = 7;

        void setSampleRate (double sr) noexcept
        {
            sampleRate = sr;
            for (auto& o : oscs) o.setSampleRate (sr);
        }

        void setWave (int w) noexcept       { wave = w;   for (auto& o : oscs) o.setWave (w); }
        void setPulseWidth (float pw) noexcept { width = pw; for (auto& o : oscs) o.setPulseWidth (pw); }

        void setUnison (int voices, float detuneCents, float stereoSpread) noexcept
        {
            count  = juce::jlimit (1, maxVoices, voices);
            detune = detuneCents;
            spread = stereoSpread;
            recalcConfig();     // pow() only here, when config changes
            applyFrequency();
        }

        void setFrequency (float f) noexcept { baseFreq = f; applyFrequency(); }

        void resetPhases (juce::Random& rng) noexcept
        {
            for (int i = 0; i < maxVoices; ++i)
                oscs[i].resetPhase (count > 1 ? rng.nextFloat() : 0.0f);
        }

        /** Reset osc phase to a fixed value (used when hard-synced). */
        void hardSyncReset() noexcept
        {
            for (int i = 0; i < count; ++i)
                oscs[i].resetPhase (0.0f);
        }

        bool masterWrapped() const noexcept { return oscs[0].wrappedThisSample(); }

        void next (float& outL, float& outR) noexcept
        {
            float l = 0.0f, r = 0.0f;
            for (int i = 0; i < count; ++i)
            {
                const float s = oscs[i].next();
                l += s * panL[i];
                r += s * panR[i];
            }
            outL = l * norm;
            outR = r * norm;
        }

    private:
        // Precompute detune multipliers + pan gains (the only pow/trig work).
        void recalcConfig() noexcept
        {
            norm = 1.0f / std::sqrt ((float) count);
            for (int i = 0; i < count; ++i)
            {
                const float pos = (count == 1) ? 0.0f
                                               : ((float) i / (float) (count - 1)) * 2.0f - 1.0f;
                detuneMult[i] = std::pow (2.0f, (pos * detune) / 1200.0f);

                const float pan = pos * spread;                 // [-1, 1]
                const float angle = (pan * 0.5f + 0.5f) * juce::MathConstants<float>::halfPi;
                panL[i] = std::cos (angle);
                panR[i] = std::sin (angle);
            }
        }

        // Cheap per-sample: just scale the base frequency by cached multipliers.
        void applyFrequency() noexcept
        {
            for (int i = 0; i < count; ++i)
                oscs[i].setFrequency (baseFreq * detuneMult[i]);
        }

        PolyBlepOsc oscs[maxVoices];
        float panL[maxVoices] { 0 }, panR[maxVoices] { 0 }, detuneMult[maxVoices] { 1 };
        double sampleRate = 44100.0;
        float baseFreq = 440.0f, detune = 0.0f, spread = 0.0f, width = 0.5f, norm = 1.0f;
        int count = 1, wave = 0;
    };
}
