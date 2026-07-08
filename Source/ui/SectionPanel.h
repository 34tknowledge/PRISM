#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "Theme.h"
#include <vector>

namespace prism
{
    /**
        A colour-coded, self-laying-out section container:
          title bar -> header row (combos/toggles) -> optional display band -> knob grid.
    */
    class SectionPanel : public juce::Component
    {
    public:
        SectionPanel (juce::String titleText, juce::Colour accentColour)
            : title (std::move (titleText)), accent (accentColour) {}

        void addHeader (juce::Component* c) { header.push_back (c); addAndMakeVisible (c); }
        void addKnob   (juce::Component* c) { knobs.push_back (c);  addAndMakeVisible (c); }
        void setDisplay (juce::Component* c, int h) { display = c; displayH = h; addAndMakeVisible (c); }
        void setColumns (int c) { columns = juce::jmax (1, c); }

        void paint (juce::Graphics& g) override
        {
            auto b = getLocalBounds().toFloat();
            juce::ColourGradient grad (theme::panelHi, 0.0f, 0.0f, theme::panel, 0.0f, (float) getHeight(), false);
            g.setGradientFill (grad);
            g.fillRoundedRectangle (b, 8.0f);
            g.setColour (theme::stroke);
            g.drawRoundedRectangle (b.reduced (0.5f), 8.0f, 1.0f);

            auto tb = getLocalBounds().removeFromTop (titleH);
            g.setColour (accent);
            g.setFont (juce::Font (juce::FontOptions().withHeight (13.0f).withStyle ("Bold")));
            g.drawText (title.toUpperCase(), tb.reduced (12, 0), juce::Justification::centredLeft);

            // accent divider under the title
            g.setColour (accent.withAlpha (0.35f));
            g.fillRect (juce::Rectangle<float> (10.0f, (float) titleH - 2.0f, (float) getWidth() - 20.0f, 1.0f));
        }

        void resized() override
        {
            auto b = getLocalBounds().reduced (10);
            b.removeFromTop (titleH - 6);

            if (! header.empty())
            {
                auto row = b.removeFromTop (26);
                b.removeFromTop (8);
                const int n = (int) header.size();
                const int w = row.getWidth() / n;
                for (int i = 0; i < n; ++i)
                {
                    auto cell = (i == n - 1) ? row : row.removeFromLeft (w);
                    if (i != n - 1) row.removeFromLeft (0);
                    header[(size_t) i]->setBounds (cell.reduced (3, 1));
                }
            }

            if (display != nullptr)
            {
                display->setBounds (b.removeFromTop (displayH));
                b.removeFromTop (8);
            }

            const int n = (int) knobs.size();
            if (n > 0)
            {
                const int cols = columns;
                const int rows = (n + cols - 1) / cols;
                const int cw = b.getWidth() / cols;
                const int ch = b.getHeight() / juce::jmax (1, rows);
                for (int i = 0; i < n; ++i)
                {
                    const int ri = i / cols, ci = i % cols;
                    knobs[(size_t) i]->setBounds (b.getX() + ci * cw, b.getY() + ri * ch, cw, ch);
                }
            }
        }

    private:
        juce::String title;
        juce::Colour accent;
        std::vector<juce::Component*> header, knobs;
        juce::Component* display = nullptr;
        int displayH = 0, columns = 3, titleH = 24;
    };
}
