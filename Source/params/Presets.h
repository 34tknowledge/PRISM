#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <map>
#include <vector>

namespace prism
{
    struct Preset
    {
        juce::String name;
        juce::String category;                 // Lead / Bass / Keys / Pluck / Pad
        std::map<juce::String, float> values;  // paramID -> actual (not normalised) value
    };

    /** The 16 hand-tuned factory presets (5 leads, 5 basses, 3 keys/plucks, 3 pads). */
    const std::vector<Preset>& getFactoryPresets();

    /** Apply a preset's values to the APVTS (params not present keep their default). */
    void applyPreset (juce::AudioProcessorValueTreeState& apvts, const Preset& preset);
}
