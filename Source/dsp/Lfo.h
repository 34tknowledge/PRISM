#pragma once

#include <cmath>
#include <juce_core/juce_core.h>

namespace prism
{
    /**
        Low-frequency oscillator. Output is bipolar [-1, 1]. Supports free-run
        (Hz) or tempo-synced rates (the host sets the Hz). S&H holds a random
        value each cycle; Random slews smoothly between random targets.
    */
    class Lfo
    {
    public:
        enum Wave { Sine = 0, Triangle, Saw, Square, SampleHold, Random };

        void setSampleRate (double sr) noexcept { sampleRate = sr; updateInc(); }
        void setRateHz (float hz) noexcept       { rateHz = hz; updateInc(); }
        void setWave (int w) noexcept            { wave = w; }

        void reset() noexcept
        {
            phase = 0.0f;
            shValue    = rng.nextFloat() * 2.0f - 1.0f;
            prevTarget = shValue;
            nextTarget = rng.nextFloat() * 2.0f - 1.0f;
        }

        /** Advance one sample and return the current value in [-1, 1]. */
        float next() noexcept
        {
            float v = 0.0f;
            switch (wave)
            {
                case Sine:       v = std::sin (juce::MathConstants<float>::twoPi * phase); break;
                case Triangle:   v = 1.0f - 4.0f * std::abs (phase - 0.5f);                break;
                case Saw:        v = 2.0f * phase - 1.0f;                                  break;
                case Square:     v = (phase < 0.5f) ? 1.0f : -1.0f;                        break;
                case SampleHold: v = shValue;                                              break;
                case Random:     v = prevTarget + (nextTarget - prevTarget) * phase;       break;
                default: break;
            }

            phase += inc;
            if (phase >= 1.0f)
            {
                phase -= 1.0f;
                shValue    = rng.nextFloat() * 2.0f - 1.0f;
                prevTarget = nextTarget;
                nextTarget = rng.nextFloat() * 2.0f - 1.0f;
            }
            return v;
        }

    private:
        void updateInc() noexcept
        {
            inc = (sampleRate > 0.0) ? (float) (rateHz / sampleRate) : 0.0f;
        }

        juce::Random rng;
        double sampleRate = 44100.0;
        float rateHz = 2.0f, inc = 0.0f, phase = 0.0f;
        float shValue = 0.0f, prevTarget = 0.0f, nextTarget = 0.0f;
        int wave = Sine;
    };
}
