#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "params/Parameters.h"
#include "dsp/SynthEngine.h"
#include "dsp/Effects.h"

/**
    PRISM — mono-focused lead/bass synthesizer.
    Developer: starfinesse.
*/
class PrismAudioProcessor : public juce::AudioProcessor
{
public:
    PrismAudioProcessor();
    ~PrismAudioProcessor() override = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "PRISM"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 4.0; }

    // Program (preset) interface — lets DAWs browse the factory bank.
    int getNumPrograms() override;
    int getCurrentProgram() override { return currentProgram; }
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }
    prism::SynthEngine& getEngine() { return engine; }

private:
    float paramValue (const char* id) const noexcept
    {
        return apvts.getRawParameterValue (id)->load();
    }

    void updateParametersForBlock();

    juce::AudioProcessorValueTreeState apvts;
    prism::SynthEngine engine;
    prism::FxChain fx;

    double currentBpm = 120.0;
    int currentProgram = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PrismAudioProcessor)
};
