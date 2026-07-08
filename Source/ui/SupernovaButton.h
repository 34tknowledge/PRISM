#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "Theme.h"
#include <cmath>

namespace prism
{
    /**
        The signature glowing button. Momentary: engaged while held. Radiates a
        pulsing supernova whose intensity tracks the live envelope value fed in
        from the editor, with a gentle idle "breathing" shimmer.
    */
    class SupernovaButton : public juce::Component, private juce::Timer
    {
    public:
        SupernovaButton() { startTimerHz (30); setWantsKeyboardFocus (false); }
        ~SupernovaButton() override { stopTimer(); }

        void setParameter (juce::RangedAudioParameter* p) { param = p; }

        void setGlow (float g)
        {
            glow = juce::jlimit (0.0f, 1.0f, g);
        }

        void mouseDown (const juce::MouseEvent&) override { press (true); }
        void mouseUp   (const juce::MouseEvent&) override { press (false); }

        void paint (juce::Graphics& g) override
        {
            auto b = getLocalBounds().toFloat();
            const float cx = b.getCentreX();
            const float cy = b.getCentreY() - 4.0f;
            const float baseR = juce::jmin (b.getWidth(), b.getHeight()) * 0.30f;
            const float intensity = juce::jlimit (0.0f, 1.0f, glow + (held ? 0.25f : 0.0f));
            const float breathe = 0.5f + 0.5f * std::sin (phase);

            // outer glow halo
            for (int i = 5; i >= 1; --i)
            {
                const float rr = baseR * (1.2f + (float) i * 0.28f) * (0.85f + 0.15f * breathe + 0.4f * intensity);
                g.setColour (theme::nova.withAlpha ((0.05f + 0.06f * intensity) * (float) i / 5.0f));
                g.fillEllipse (juce::Rectangle<float> (rr * 2.0f, rr * 2.0f).withCentre ({ cx, cy }));
            }

            // burst rays
            const int rays = 12;
            for (int i = 0; i < rays; ++i)
            {
                const float a = (float) i / (float) rays * juce::MathConstants<float>::twoPi + phase * 0.3f;
                const float inner = baseR * 0.9f;
                const float outer = baseR * (1.35f + 0.35f * intensity + 0.12f * std::sin (phase * 1.7f + (float) i));
                const juce::Point<float> p1 { cx + inner * std::cos (a), cy + inner * std::sin (a) };
                const juce::Point<float> p2 { cx + outer * std::cos (a), cy + outer * std::sin (a) };
                g.setColour (theme::nova.withAlpha (0.25f + 0.5f * intensity));
                g.drawLine ({ p1, p2 }, 1.5f + 1.5f * intensity);
            }

            // core
            juce::ColourGradient core (juce::Colours::white.withAlpha (0.95f), cx, cy,
                                       theme::nova.withAlpha (0.0f), cx, cy - baseR, true);
            core.addColour (0.35, theme::nova.brighter (0.3f + 0.4f * intensity));
            core.addColour (0.75, theme::nova.withAlpha (0.8f));
            g.setGradientFill (core);
            g.fillEllipse (juce::Rectangle<float> (baseR * 2.0f, baseR * 2.0f).withCentre ({ cx, cy }));

            g.setColour (juce::Colours::white.withAlpha (0.6f + 0.4f * intensity));
            g.drawEllipse (juce::Rectangle<float> (baseR * 2.0f, baseR * 2.0f).withCentre ({ cx, cy }), 1.5f);

            // label
            g.setColour (juce::Colours::white.withAlpha (0.9f));
            g.setFont (juce::Font (juce::FontOptions().withHeight (12.0f).withStyle ("Bold")));
            g.drawText ("SUPERNOVA", b.removeFromBottom (16.0f), juce::Justification::centred);
        }

    private:
        void press (bool on)
        {
            held = on;
            if (param != nullptr)
            {
                if (on) param->beginChangeGesture();
                param->setValueNotifyingHost (on ? 1.0f : 0.0f);
                if (! on) param->endChangeGesture();
            }
            repaint();
        }

        void timerCallback() override
        {
            phase += 0.12f;
            if (phase > juce::MathConstants<float>::twoPi) phase -= juce::MathConstants<float>::twoPi;
            repaint();
        }

        juce::RangedAudioParameter* param = nullptr;
        float glow = 0.0f, phase = 0.0f;
        bool held = false;
    };
}
