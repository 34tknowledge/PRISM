#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "Theme.h"

namespace prism
{
    /** Custom look: glowing rotary knobs, neon toggles and dark combo boxes. */
    class PrismLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        PrismLookAndFeel();

        void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height,
                               float sliderPos, float startAngle, float endAngle,
                               juce::Slider&) override;

        void drawToggleButton (juce::Graphics&, juce::ToggleButton&,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override;

        void drawComboBox (juce::Graphics&, int width, int height, bool isButtonDown,
                           int buttonX, int buttonY, int buttonW, int buttonH,
                           juce::ComboBox&) override;
        void positionComboBoxText (juce::ComboBox&, juce::Label&) override;
        juce::Font getComboBoxFont (juce::ComboBox&) override;
        juce::Font getPopupMenuFont() override;

        juce::Label* createSliderTextBox (juce::Slider&) override;
        juce::Font getLabelFont (juce::Label&) override;
    };
}
