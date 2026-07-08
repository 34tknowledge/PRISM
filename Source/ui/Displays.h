#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "Theme.h"
#include <cmath>

namespace prism
{
    /** Shared dark rounded panel used behind every live display. */
    inline void paintDisplayBackground (juce::Graphics& g, juce::Rectangle<float> b, juce::Colour accent)
    {
        g.setColour (theme::bg0);
        g.fillRoundedRectangle (b, 6.0f);
        g.setColour (accent.withAlpha (0.25f));
        g.drawRoundedRectangle (b.reduced (0.5f), 6.0f, 1.0f);
    }

    inline void glowStroke (juce::Graphics& g, const juce::Path& p, juce::Colour c, float base)
    {
        for (int i = 3; i >= 1; --i)
        {
            g.setColour (c.withAlpha (0.09f * (float) i));
            g.strokePath (p, juce::PathStrokeType (base + (float) i * 2.0f));
        }
        g.setColour (c);
        g.strokePath (p, juce::PathStrokeType (base));
    }

    // ---------------------------------------------------------------- Waveform
    class WaveformDisplay : public juce::Component
    {
    public:
        explicit WaveformDisplay (juce::Colour c) : accent (c) {}

        void setWave (int w, float pulseWidth)
        {
            if (w != wave || std::abs (pulseWidth - pw) > 1.0e-4f)
            {
                wave = w; pw = pulseWidth; repaint();
            }
        }

        void paint (juce::Graphics& g) override
        {
            auto b = getLocalBounds().toFloat().reduced (2.0f);
            paintDisplayBackground (g, b, accent);

            auto area = b.reduced (8.0f, 6.0f);
            const float amp = area.getHeight() * 0.42f;
            const float midY = area.getCentreY();

            juce::Path p;
            const int n = juce::jmax (2, (int) area.getWidth());
            for (int i = 0; i <= n; ++i)
            {
                const float x = area.getX() + (float) i;
                const float phase = std::fmod (((float) i / area.getWidth()) * 2.0f, 1.0f);
                const float v = waveValue (phase);
                const float y = midY - v * amp;
                if (i == 0) p.startNewSubPath (x, y);
                else        p.lineTo (x, y);
            }
            glowStroke (g, p, accent, 1.6f);
        }

    private:
        float waveValue (float phase) const
        {
            switch (wave)
            {
                case 0: return 2.0f * phase - 1.0f;                              // Saw
                case 1: return phase < 0.5f ? 1.0f : -1.0f;                      // Square
                case 2: return 1.0f - 4.0f * std::abs (phase - 0.5f);           // Triangle
                case 3: return std::sin (juce::MathConstants<float>::twoPi * phase); // Sine
                case 4: return phase < pw ? 1.0f : -1.0f;                       // PWM
                default: return 0.0f;
            }
        }

        juce::Colour accent;
        int wave = 0;
        float pw = 0.5f;
    };

    // ------------------------------------------------------------ Filter curve
    class FilterCurveDisplay : public juce::Component
    {
    public:
        explicit FilterCurveDisplay (juce::Colour c) : accent (c) {}

        void setFilter (int t, float cutoffHz, float reso, bool is24)
        {
            type = t; cutoff = cutoffHz; res = reso; slope24 = is24;
            repaint();
        }

        void paint (juce::Graphics& g) override
        {
            auto b = getLocalBounds().toFloat().reduced (2.0f);
            paintDisplayBackground (g, b, accent);
            auto area = b.reduced (8.0f, 8.0f);

            const float Q = 0.5f + res * res * 17.5f;
            const float dbTop = 18.0f, dbBot = -48.0f;

            auto dbToY = [&] (float db)
            {
                const float t = juce::jlimit (0.0f, 1.0f, (db - dbBot) / (dbTop - dbBot));
                return area.getBottom() - t * area.getHeight();
            };

            juce::Path p;
            const int n = juce::jmax (2, (int) area.getWidth());
            for (int i = 0; i <= n; ++i)
            {
                const float fx = (float) i / (float) n;
                const float freq = 20.0f * std::pow (1000.0f, fx);   // 20 .. 20k
                const float w = freq / juce::jmax (10.0f, cutoff);
                const float denom = std::sqrt ((1.0f - w * w) * (1.0f - w * w) + (w / Q) * (w / Q));
                float mag = 1.0f;
                switch (type)
                {
                    case 0: mag = 1.0f / denom;                 break; // LP
                    case 1: mag = (w * w) / denom;              break; // HP
                    case 2: mag = (w / Q) / denom;              break; // BP
                    case 3: mag = std::abs (1.0f - w * w) / denom; break; // Notch
                    default: break;
                }
                if (slope24) mag *= mag;
                const float db = 20.0f * std::log10 (juce::jmax (1.0e-4f, mag));
                const float x = area.getX() + fx * area.getWidth();
                const float y = dbToY (db);
                if (i == 0) p.startNewSubPath (x, y);
                else        p.lineTo (x, y);
            }

            // cutoff marker
            const float cx = area.getX() + (std::log (juce::jlimit (20.0f, 20000.0f, cutoff) / 20.0f)
                                            / std::log (1000.0f)) * area.getWidth();
            g.setColour (accent.withAlpha (0.25f));
            g.drawVerticalLine ((int) cx, area.getY(), area.getBottom());

            glowStroke (g, p, accent, 1.8f);
        }

    private:
        juce::Colour accent;
        int type = 0;
        float cutoff = 14000.0f, res = 0.15f;
        bool slope24 = true;
    };

    // --------------------------------------------------------------- ADSR shape
    class EnvelopeDisplay : public juce::Component
    {
    public:
        explicit EnvelopeDisplay (juce::Colour c) : accent (c) {}

        void setADSR (float a, float d, float s, float r)
        {
            atk = a; dec = d; sus = s; rel = r; repaint();
        }

        void paint (juce::Graphics& g) override
        {
            auto b = getLocalBounds().toFloat().reduced (2.0f);
            paintDisplayBackground (g, b, accent);
            auto area = b.reduced (8.0f, 8.0f);

            // compress times logarithmically so long stages still show
            auto comp = [] (float ms) { return std::log10 (1.0f + juce::jmax (0.0f, ms) * 0.01f); };
            const float ca = comp (atk), cd = comp (dec), cr = comp (rel);
            const float sustainW = 0.22f;                     // fixed fraction for sustain hold
            const float total = ca + cd + cr + 0.0001f;
            const float span = (1.0f - sustainW);
            const float wa = span * (ca / total);
            const float wd = span * (cd / total);
            const float wr = span * (cr / total);

            const float x0 = area.getX(),  yBot = area.getBottom(),  yTop = area.getY();
            const float sy = yBot - juce::jlimit (0.0f, 1.0f, sus) * area.getHeight();

            const float xA = x0 + wa * area.getWidth();
            const float xD = xA + wd * area.getWidth();
            const float xS = xD + sustainW * area.getWidth();
            const float xR = xS + wr * area.getWidth();

            juce::Path line;
            line.startNewSubPath (x0, yBot);
            line.lineTo (xA, yTop);
            line.lineTo (xD, sy);
            line.lineTo (xS, sy);
            line.lineTo (xR, yBot);

            // filled area under the curve
            juce::Path fill = line;
            fill.lineTo (xR, yBot);
            fill.closeSubPath();
            g.setColour (accent.withAlpha (0.14f));
            g.fillPath (fill);

            glowStroke (g, line, accent, 1.8f);

            // node dots
            g.setColour (accent);
            for (auto pt : { juce::Point<float> (xA, yTop), juce::Point<float> (xD, sy),
                             juce::Point<float> (xR, yBot) })
                g.fillEllipse (juce::Rectangle<float> (5.0f, 5.0f).withCentre (pt));
        }

    private:
        juce::Colour accent;
        float atk = 2, dec = 400, sus = 0.8f, rel = 300;
    };
}
