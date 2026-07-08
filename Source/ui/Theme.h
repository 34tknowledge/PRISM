#pragma once

#include <juce_graphics/juce_graphics.h>

namespace prism::theme
{
    // ---- surfaces ----
    const juce::Colour bg0      { 0xff0d0f14 };   // window base
    const juce::Colour bg1      { 0xff12151d };   // gradient bottom
    const juce::Colour panel    { 0xff171b24 };   // section panel
    const juce::Colour panelHi  { 0xff1e2430 };   // panel top edge
    const juce::Colour stroke   { 0xff2a3140 };   // hairline
    const juce::Colour track    { 0xff20232c };   // knob track
    const juce::Colour text     { 0xffd7deeb };   // primary text
    const juce::Colour textDim  { 0xff7c8699 };   // captions

    // ---- section accents (neon, glow-friendly) ----
    const juce::Colour osc      { 0xffffb020 };   // amber
    const juce::Colour sub      { 0xffff6a3d };   // orange
    const juce::Colour filter   { 0xff21e6d6 };   // cyan
    const juce::Colour ampEnv   { 0xff45e85f };   // green
    const juce::Colour filtEnv  { 0xff2fd0a8 };   // teal
    const juce::Colour lfo      { 0xffa974ff };   // purple
    const juce::Colour fx       { 0xffff4fa3 };   // magenta
    const juce::Colour master   { 0xffcdd7f5 };   // ice white
    const juce::Colour nova     { 0xffff2e63 };   // supernova core (hot pink/red)
}
