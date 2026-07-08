#pragma once

#include <cmath>
#include <juce_core/juce_core.h>

namespace prism
{
    /**
        One TPT / zero-delay-feedback State Variable Filter stage
        (Andrew Simper / Cytomic "linear trapezoidal optimised" form).

        Produces low/band/high/notch outputs simultaneously, is stable up to
        self-oscillation and is cheap per sample. Used both by the main
        multi-mode filter and by the colored crusher's resonant tone stage.
    */
    struct SvfStage
    {
        void setSampleRate (double sr) { sampleRate = sr; }

        void reset() noexcept { ic1eq = 0.0f; ic2eq = 0.0f; }

        void setCutoffQ (float cutoffHz, float Q) noexcept
        {
            cutoffHz = juce::jlimit (10.0f, (float) (sampleRate * 0.49), cutoffHz);
            const float g = std::tan (juce::MathConstants<float>::pi * cutoffHz / (float) sampleRate);
            k  = 1.0f / juce::jmax (0.05f, Q);
            a1 = 1.0f / (1.0f + g * (g + k));
            a2 = g * a1;
            a3 = g * a2;
        }

        struct Outputs { float lp, bp, hp, notch; };

        Outputs process (float v0) noexcept
        {
            const float v3 = v0 - ic2eq;
            const float v1 = a1 * ic1eq + a2 * v3;
            const float v2 = ic2eq + a2 * ic1eq + a3 * v3;
            ic1eq = 2.0f * v1 - ic1eq;
            ic2eq = 2.0f * v2 - ic2eq;

            const float lp = v2;
            const float bp = v1;
            const float hp = v0 - k * v1 - v2;
            const float notch = v0 - k * v1;      // = lp + hp
            return { lp, bp, hp, notch };
        }

        float processLP (float v0) noexcept { return process (v0).lp; }

        double sampleRate = 44100.0;
        float k = 1.0f, a1 = 0.0f, a2 = 0.0f, a3 = 0.0f;
        float ic1eq = 0.0f, ic2eq = 0.0f;
    };

    /** One-pole DC blocker: y = x - x1 + R*y1. R≈0.9995 kills sub-sonic DC. */
    struct DcBlocker
    {
        void reset() noexcept { x1 = y1 = 0.0f; }
        void setR (float newR) noexcept { R = newR; }
        float process (float x) noexcept
        {
            const float y = x - x1 + R * y1;
            x1 = x; y1 = y;
            return y;
        }
        float R = 0.9995f, x1 = 0.0f, y1 = 0.0f;
    };
}
