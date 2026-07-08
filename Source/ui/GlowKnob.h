#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "Theme.h"

namespace prism
{
    /** A captioned glowing rotary knob (drawn by PrismLookAndFeel). */
    class GlowKnob : public juce::Component
    {
    public:
        GlowKnob (const juce::String& captionText, juce::Colour accentColour)
            : caption (captionText), accent (accentColour)
        {
            slider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
            slider.setRotaryParameters (juce::MathConstants<float>::pi * 1.25f,
                                        juce::MathConstants<float>::pi * 2.75f, true);
            slider.setColour (juce::Slider::rotarySliderFillColourId, accent);
            slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 64, 14);
            slider.setColour (juce::Slider::textBoxTextColourId, theme::textDim);
            slider.setVelocityBasedMode (false);
            addAndMakeVisible (slider);
        }

        void resized() override
        {
            auto b = getLocalBounds();
            b.removeFromTop (15);                 // caption row
            slider.setBounds (b);
        }

        void paint (juce::Graphics& g) override
        {
            g.setColour (theme::textDim);
            g.setFont (juce::Font (juce::FontOptions().withHeight (11.5f).withStyle ("Bold")));
            g.drawText (caption.toUpperCase(), getLocalBounds().removeFromTop (15),
                        juce::Justification::centred);
        }

        juce::Slider slider;

    private:
        juce::String caption;
        juce::Colour accent;
    };
}
