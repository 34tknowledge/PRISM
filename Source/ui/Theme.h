#pragma once

#include <juce_graphics/juce_graphics.h>

namespace prism::theme
{
    // ---- surfaces (clean, low-contrast dark) ----
    const juce::Colour bg0      { 0xff15161b };   // window base
    const juce::Colour bg1      { 0xff101116 };   // gradient bottom
    const juce::Colour panel    { 0xff1c1d25 };   // section panel
    const juce::Colour panelHi  { 0xff23242e };   // knob body / raised
    const juce::Colour stroke   { 0xff2b2d38 };   // hairline borders
    const juce::Colour track    { 0xff32343e };   // knob track (inactive)
    const juce::Colour text     { 0xffe8eaf1 };   // primary text
    const juce::Colour textDim  { 0xff868b9c };   // captions / secondary

    // ---- single accent, taken from the SF logo ----
    const juce::Colour accent    { 0xffe24d97 };  // pink
    const juce::Colour accentDim { 0xff8f3d64 };  // muted pink (subtle fills)

    // Legacy per-section names kept so existing call sites compile — the UI is
    // now monochrome + one accent, so they all resolve to the accent. Sections
    // are distinguished by typography and spacing, not by colour.
    const juce::Colour osc     = accent;
    const juce::Colour sub     = accent;
    const juce::Colour filter  = accent;
    const juce::Colour ampEnv  = accent;
    const juce::Colour filtEnv = accent;
    const juce::Colour lfo     = accent;
    const juce::Colour fx      = accent;
    const juce::Colour master  = accent;
    const juce::Colour nova    = accent;
}
