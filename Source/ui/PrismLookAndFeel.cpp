#include "PrismLookAndFeel.h"
#include <cmath>

namespace prism
{
    static juce::Font prismFont (float h, bool bold = false)
    {
        auto o = juce::FontOptions().withHeight (h);
        if (bold) o = o.withStyle ("Bold");
        return juce::Font (o);
    }

    PrismLookAndFeel::PrismLookAndFeel()
    {
        setColour (juce::PopupMenu::backgroundColourId,       theme::panel);
        setColour (juce::PopupMenu::textColourId,             theme::text);
        setColour (juce::PopupMenu::highlightedBackgroundColourId, theme::filter.withAlpha (0.25f));
        setColour (juce::PopupMenu::highlightedTextColourId,  juce::Colours::white);
        setColour (juce::Slider::textBoxTextColourId,         theme::text);
        setColour (juce::Slider::textBoxOutlineColourId,      juce::Colours::transparentBlack);
        setColour (juce::Label::textColourId,                 theme::text);
    }

    void PrismLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                             float sliderPos, float startAngle, float endAngle,
                                             juce::Slider& slider)
    {
        auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (4.0f);
        const float radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
        const float cx = bounds.getCentreX();
        const float cy = bounds.getCentreY();
        const float angle = startAngle + sliderPos * (endAngle - startAngle);
        const auto accent = slider.findColour (juce::Slider::rotarySliderFillColourId);

        const float thick = radius * 0.16f;
        const float arcR  = radius - thick * 0.6f;

        // background track
        juce::Path bg;
        bg.addCentredArc (cx, cy, arcR, arcR, 0.0f, startAngle, endAngle, true);
        g.setColour (theme::track);
        g.strokePath (bg, { thick, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

        // value arc with layered outer glow
        juce::Path val;
        val.addCentredArc (cx, cy, arcR, arcR, 0.0f, startAngle, angle, true);
        for (int i = 3; i >= 1; --i)
        {
            g.setColour (accent.withAlpha (0.10f * (float) i));
            g.strokePath (val, { thick + (float) i * 3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });
        }
        g.setColour (accent);
        g.strokePath (val, { thick, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

        // knob body
        const float bodyR = arcR - thick * 0.9f;
        auto body = juce::Rectangle<float> (bodyR * 2.0f, bodyR * 2.0f).withCentre ({ cx, cy });
        juce::ColourGradient grad (theme::panelHi, cx, cy - bodyR, theme::bg0, cx, cy + bodyR, false);
        g.setGradientFill (grad);
        g.fillEllipse (body);
        g.setColour (theme::stroke);
        g.drawEllipse (body, 1.0f);

        // pointer with glow
        const float tipR   = bodyR * 0.92f;
        const float innerR = bodyR * 0.28f;
        const juce::Point<float> tip  { cx + tipR   * std::sin (angle), cy - tipR   * std::cos (angle) };
        const juce::Point<float> base { cx + innerR * std::sin (angle), cy - innerR * std::cos (angle) };
        g.setColour (accent.withAlpha (0.35f));
        g.drawLine ({ base, tip }, thick * 1.7f);
        g.setColour (accent);
        g.drawLine ({ base, tip }, thick * 0.85f);

        // centre dot
        g.setColour (accent.brighter (0.2f));
        g.fillEllipse (juce::Rectangle<float> (thick, thick).withCentre ({ cx, cy }));
    }

    void PrismLookAndFeel::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                                             bool /*hi*/, bool /*down*/)
    {
        auto b = button.getLocalBounds().toFloat().reduced (1.0f);
        const auto accent = button.findColour (juce::ToggleButton::tickColourId);
        const bool on = button.getToggleState();
        const float r = b.getHeight() * 0.3f;

        if (on)
        {
            g.setColour (accent.withAlpha (0.25f));
            g.fillRoundedRectangle (b.expanded (2.5f), r + 2.0f);
            g.setColour (accent);
            g.fillRoundedRectangle (b, r);
            g.setColour (theme::bg0);
        }
        else
        {
            g.setColour (theme::track);
            g.fillRoundedRectangle (b, r);
            g.setColour (theme::stroke);
            g.drawRoundedRectangle (b, r, 1.0f);
            g.setColour (theme::textDim);
        }

        g.setFont (prismFont (juce::jmin (13.0f, b.getHeight() * 0.5f), true));
        g.drawText (button.getButtonText(), b, juce::Justification::centred);
    }

    void PrismLookAndFeel::drawComboBox (juce::Graphics& g, int width, int height, bool,
                                         int, int, int, int, juce::ComboBox& box)
    {
        auto b = juce::Rectangle<int> (0, 0, width, height).toFloat().reduced (1.0f);
        const auto accent = box.findColour (juce::ComboBox::outlineColourId);

        g.setColour (theme::track);
        g.fillRoundedRectangle (b, 4.0f);
        g.setColour (accent.withAlpha (0.5f));
        g.drawRoundedRectangle (b, 4.0f, 1.0f);

        juce::Path arrow;
        const float aw = 7.0f, ah = 4.0f;
        const float ax = (float) width - 14.0f, ay = (float) height * 0.5f;
        arrow.addTriangle (ax - aw * 0.5f, ay - ah * 0.5f,
                           ax + aw * 0.5f, ay - ah * 0.5f,
                           ax,             ay + ah * 0.5f);
        g.setColour (accent);
        g.fillPath (arrow);
    }

    void PrismLookAndFeel::positionComboBoxText (juce::ComboBox& box, juce::Label& label)
    {
        label.setBounds (8, 0, box.getWidth() - 24, box.getHeight());
        label.setFont (prismFont (13.0f));
        label.setColour (juce::Label::textColourId, theme::text);
        label.setJustificationType (juce::Justification::centredLeft);
    }

    juce::Font PrismLookAndFeel::getComboBoxFont (juce::ComboBox&)  { return prismFont (13.0f); }
    juce::Font PrismLookAndFeel::getPopupMenuFont()                 { return prismFont (14.0f); }
    juce::Font PrismLookAndFeel::getLabelFont (juce::Label&)        { return prismFont (13.0f); }

    juce::Label* PrismLookAndFeel::createSliderTextBox (juce::Slider& slider)
    {
        auto* l = juce::LookAndFeel_V4::createSliderTextBox (slider);
        l->setJustificationType (juce::Justification::centred);
        l->setColour (juce::Label::textColourId, theme::textDim);
        l->setColour (juce::Label::outlineColourId, juce::Colours::transparentBlack);
        l->setColour (juce::Label::backgroundColourId, juce::Colours::transparentBlack);
        l->setFont (prismFont (11.5f));
        return l;
    }
}
