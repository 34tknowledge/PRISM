#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "params/Presets.h"

using namespace prism;
using namespace prism::id;

PrismAudioProcessor::PrismAudioProcessor()
    : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PRISM", createParameterLayout())
{
}

bool PrismAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto& out = layouts.getMainOutputChannelSet();
    return out == juce::AudioChannelSet::mono() || out == juce::AudioChannelSet::stereo();
}

void PrismAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    engine.prepare (sampleRate, samplesPerBlock);
    fx.prepare (sampleRate, samplesPerBlock, getTotalNumOutputChannels());
}

void PrismAudioProcessor::updateParametersForBlock()
{
    // ---- voice params ----
    VoiceParams vp;
    vp.osc1Wave = (int) paramValue (osc1Wave);
    vp.osc1Coarse = paramValue (osc1Coarse);
    vp.osc1Fine = paramValue (osc1Fine);
    vp.osc1PW = paramValue (osc1PW);
    vp.osc1Level = paramValue (osc1Level);
    vp.osc1Uni = (int) paramValue (osc1Unison);
    vp.osc1Detune = paramValue (osc1Detune);
    vp.osc1Spread = paramValue (osc1Spread);

    vp.osc2Wave = (int) paramValue (osc2Wave);
    vp.osc2Coarse = paramValue (osc2Coarse);
    vp.osc2Fine = paramValue (osc2Fine);
    vp.osc2PW = paramValue (osc2PW);
    vp.osc2Level = paramValue (osc2Level);
    vp.osc2Uni = (int) paramValue (osc2Unison);
    vp.osc2Detune = paramValue (osc2Detune);
    vp.osc2Spread = paramValue (osc2Spread);
    vp.osc2Sync = paramValue (osc2Sync) > 0.5f;

    vp.subWave = (int) paramValue (subWave);
    vp.subOctave = (int) paramValue (subOctave);
    vp.subLevel = paramValue (subLevel);
    vp.noiseLevel = paramValue (noiseLevel);
    vp.noiseColor = (int) paramValue (noiseColor);

    vp.filterType = (int) paramValue (filterType);
    vp.filterSlope24 = paramValue (filterSlope) > 0.5f;
    vp.filterCutoff = paramValue (filterCutoff);
    vp.filterReso = paramValue (filterReso);
    vp.filterDrive = paramValue (filterDrive);
    vp.filterKeytrack = paramValue (filterKeytrk);
    vp.filterEnvAmt = paramValue (filterEnvAmt);
    engine.setVoiceParams (vp);

    // ---- envelopes ----
    juce::ADSR::Parameters ampP {
        paramValue (ampAttack)  * 0.001f, paramValue (ampDecay)  * 0.001f,
        paramValue (ampSustain),          paramValue (ampRelease) * 0.001f };
    juce::ADSR::Parameters filtP {
        paramValue (filtAttack) * 0.001f, paramValue (filtDecay) * 0.001f,
        paramValue (filtSustain),         paramValue (filtRelease) * 0.001f };
    engine.setEnvParams (ampP, filtP);

    // ---- voice mode / glide / bend ----
    engine.setModeParams ((int) paramValue (voiceMode), paramValue (glideTime), (int) paramValue (glideMode));
    engine.setBendRange (paramValue (bendRange));

    // ---- LFOs (tempo sync computes Hz from host BPM) ----
    const double bps = currentBpm / 60.0;
    auto lfoHz = [&] (bool sync, float freeHz, int divIdx)
    {
        if (! sync) return freeHz;
        const double beats = syncDivisionInBeats (divIdx);
        return (float) (bps / juce::jmax (1.0e-6, beats));
    };
    engine.setLfoParams (0, (int) paramValue (lfo1Wave),
                         lfoHz (paramValue (lfo1Sync) > 0.5f, paramValue (lfo1Rate), (int) paramValue (lfo1SyncRate)),
                         (int) paramValue (lfo1Dest), paramValue (lfo1Amount));
    engine.setLfoParams (1, (int) paramValue (lfo2Wave),
                         lfoHz (paramValue (lfo2Sync) > 0.5f, paramValue (lfo2Rate), (int) paramValue (lfo2SyncRate)),
                         (int) paramValue (lfo2Dest), paramValue (lfo2Amount));
}

void PrismAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;

    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (auto* ph = getPlayHead())
        if (auto pos = ph->getPosition())
            if (auto bpm = pos->getBpm())
                currentBpm = *bpm;

    updateParametersForBlock();

    // 1. synth voices (pre-FX)
    engine.render (buffer, midi);

    // 2. FX chain
    FxChain::Params fp;
    fp.crushOn = paramValue (crushOn) > 0.5f;
    fp.crushDrive = paramValue (crushDrive);
    fp.bits = paramValue (crushBits);
    fp.srFactor = paramValue (crushSR);
    fp.shape = paramValue (crushShape);
    fp.tone = paramValue (crushTone);
    fp.reso = paramValue (crushReso);
    fp.crushMix = paramValue (crushMix);

    fp.chorusOn = paramValue (chorusOn) > 0.5f;
    fp.chorusRate = paramValue (chorusRate);
    fp.chorusDepth = paramValue (chorusDepth);
    fp.chorusFbk = paramValue (chorusFbk);
    fp.chorusMix = paramValue (chorusMix);

    fp.delayOn = paramValue (delayOn) > 0.5f;
    const bool dSync = paramValue (delaySync) > 0.5f;
    fp.delayTimeMs = dSync ? (float) (syncDivisionInBeats ((int) paramValue (delaySyncRate)) * 60000.0 / currentBpm)
                           : paramValue (delayTime);
    fp.delayFbk = paramValue (delayFbk);
    fp.delayPingpong = paramValue (delayPingpong);
    fp.delayTone = paramValue (delayTone);
    fp.delayMix = paramValue (delayMix);

    fp.reverbOn = paramValue (reverbOn) > 0.5f;
    fp.reverbSize = paramValue (reverbSize);
    fp.reverbDamp = paramValue (reverbDamp);
    fp.reverbWidth = paramValue (reverbWidth);
    fp.reverbMix = paramValue (reverbMix);

    fp.nova = 0.0f;
    fx.process (buffer, fp);

    // 3. master output gain
    const float g = juce::Decibels::decibelsToGain (paramValue (masterGain), -60.0f);
    buffer.applyGain (g);
}

juce::AudioProcessorEditor* PrismAudioProcessor::createEditor()
{
    return new PrismAudioProcessorEditor (*this);
}

// ---- program (preset) interface ----
int PrismAudioProcessor::getNumPrograms() { return (int) getFactoryPresets().size(); }

void PrismAudioProcessor::setCurrentProgram (int index)
{
    const auto& presets = getFactoryPresets();
    if (index < 0 || index >= (int) presets.size()) return;
    currentProgram = index;
    applyPreset (apvts, presets[(size_t) index]);
    updateHostDisplay();
}

const juce::String PrismAudioProcessor::getProgramName (int index)
{
    const auto& presets = getFactoryPresets();
    if (index < 0 || index >= (int) presets.size()) return {};
    return presets[(size_t) index].name;
}

// ---- state ----
void PrismAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos (destData, true);
    apvts.state.writeToStream (mos);
}

void PrismAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData (data, (size_t) sizeInBytes);
    if (tree.isValid())
        apvts.replaceState (tree);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PrismAudioProcessor();
}
