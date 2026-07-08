#include "Presets.h"
#include "Parameters.h"

// The preset table below mixes int and float literals for readability
// ({ voiceMode, 1 } next to { osc1Level, 0.8f }); every int fits exactly in a
// float, so silence the benign int->float narrowing warning here.
#if defined (_MSC_VER)
 #pragma warning (push)
 #pragma warning (disable: 4244)
#endif

namespace prism
{
    using V = std::map<juce::String, float>;

    static const std::vector<juce::String>& getAllParameterIDs()
    {
        using namespace prism::id;
        static const std::vector<juce::String> ids {
            masterGain, voiceMode, glideTime, glideMode, bendRange,
            osc1Wave, osc1Coarse, osc1Fine, osc1PW, osc1Level, osc1Unison, osc1Detune, osc1Spread,
            osc2Wave, osc2Coarse, osc2Fine, osc2PW, osc2Level, osc2Unison, osc2Detune, osc2Spread, osc2Sync,
            subWave, subOctave, subLevel, noiseLevel, noiseColor,
            filterType, filterSlope, filterCutoff, filterReso, filterDrive, filterKeytrk, filterEnvAmt,
            ampAttack, ampDecay, ampSustain, ampRelease,
            filtAttack, filtDecay, filtSustain, filtRelease,
            lfo1Wave, lfo1Rate, lfo1Sync, lfo1SyncRate, lfo1Dest, lfo1Amount,
            lfo2Wave, lfo2Rate, lfo2Sync, lfo2SyncRate, lfo2Dest, lfo2Amount,
            crushOn, crushDrive, crushBits, crushSR, crushShape, crushTone, crushReso, crushMix,
            chorusOn, chorusRate, chorusDepth, chorusFbk, chorusMix,
            delayOn, delaySync, delayTime, delaySyncRate, delayFbk, delayPingpong, delayTone, delayMix,
            reverbOn, reverbSize, reverbDamp, reverbWidth, reverbMix,
            supernova
        };
        return ids;
    }

    const std::vector<Preset>& getFactoryPresets()
    {
        using namespace prism::id;
        static const std::vector<Preset> presets = {

        // ============================ LEADS ============================
        { "Solar Flare Lead", "Lead", V {
            { voiceMode, 1 }, { glideTime, 30 }, { masterGain, -7 },
            { osc1Wave, 0 }, { osc1Unison, 7 }, { osc1Detune, 28 }, { osc1Spread, 0.8f }, { osc1Level, 0.8f },
            { osc2Wave, 0 }, { osc2Unison, 7 }, { osc2Detune, 30 }, { osc2Fine, 12 }, { osc2Level, 0.5f }, { osc2Spread, 0.8f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 9000 }, { filterReso, 0.2f }, { filterKeytrk, 0.3f }, { filterEnvAmt, 0.35f },
            { ampAttack, 5 }, { ampDecay, 800 }, { ampSustain, 0.85f }, { ampRelease, 250 },
            { filtAttack, 5 }, { filtDecay, 400 }, { filtSustain, 0.4f }, { filtRelease, 300 },
            { chorusOn, 1 }, { chorusRate, 0.5f }, { chorusDepth, 0.4f }, { chorusMix, 0.3f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 11 }, { delayFbk, 0.3f }, { delayMix, 0.22f },
            { reverbOn, 1 }, { reverbSize, 0.5f }, { reverbMix, 0.2f } } },

        { "Screamer", "Lead", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -8 },
            { osc1Wave, 1 }, { osc1Unison, 3 }, { osc1Detune, 15 }, { osc1Level, 0.8f },
            { osc2Wave, 0 }, { osc2Fine, 5 }, { osc2Level, 0.5f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 3500 }, { filterReso, 0.55f }, { filterDrive, 0.6f }, { filterEnvAmt, 0.5f },
            { ampAttack, 2 }, { ampDecay, 300 }, { ampSustain, 0.9f }, { ampRelease, 120 },
            { filtAttack, 2 }, { filtDecay, 250 }, { filtSustain, 0.35f }, { filtRelease, 150 },
            { crushOn, 1 }, { crushDrive, 0.5f }, { crushBits, 8 }, { crushSR, 2 }, { crushShape, 0.5f }, { crushTone, 6000 }, { crushMix, 0.5f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 11 }, { delayFbk, 0.35f }, { delayMix, 0.2f },
            { reverbOn, 1 }, { reverbSize, 0.3f }, { reverbMix, 0.15f } } },

        { "Glass Slide", "Lead", V {
            { voiceMode, 2 }, { glideTime, 140 }, { glideMode, 1 }, { masterGain, -7 },
            { osc1Wave, 0 }, { osc1Unison, 1 }, { osc1Level, 0.8f },
            { osc2Wave, 2 }, { osc2Fine, -12 }, { osc2Level, 0.6f },
            { subWave, 0 }, { subLevel, 0.3f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 6000 }, { filterReso, 0.2f }, { filterKeytrk, 0.5f }, { filterEnvAmt, 0.2f },
            { ampAttack, 20 }, { ampDecay, 600 }, { ampSustain, 0.8f }, { ampRelease, 400 },
            { filtAttack, 15 }, { filtDecay, 400 }, { filtSustain, 0.5f }, { filtRelease, 400 },
            { chorusOn, 1 }, { chorusDepth, 0.4f }, { chorusMix, 0.3f },
            { reverbOn, 1 }, { reverbSize, 0.6f }, { reverbMix, 0.3f } } },

        { "8-Bit Hero", "Lead", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -7 },
            { osc1Wave, 4 }, { osc1PW, 0.35f }, { osc1Unison, 1 }, { osc1Level, 0.9f },
            { osc2Wave, 1 }, { osc2Coarse, 12 }, { osc2Level, 0.3f },
            { filterType, 0 }, { filterSlope, 0 }, { filterCutoff, 14000 }, { filterReso, 0.1f },
            { ampAttack, 2 }, { ampDecay, 150 }, { ampSustain, 0.6f }, { ampRelease, 80 },
            { crushOn, 1 }, { crushDrive, 0.3f }, { crushBits, 5 }, { crushSR, 6 }, { crushShape, 0.4f }, { crushTone, 9000 }, { crushReso, 0.3f }, { crushMix, 0.8f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 11 }, { delayFbk, 0.4f }, { delayMix, 0.25f },
            { lfo1Wave, 0 }, { lfo1Rate, 4 }, { lfo1Dest, 2 }, { lfo1Amount, 0.4f } } },

        { "Neon Vintage", "Lead", V {
            { voiceMode, 1 }, { glideTime, 20 }, { masterGain, -7 },
            { osc1Wave, 4 }, { osc1PW, 0.5f }, { osc1Unison, 3 }, { osc1Detune, 12 }, { osc1Level, 0.8f },
            { osc2Wave, 4 }, { osc2PW, 0.5f }, { osc2Fine, 7 }, { osc2Level, 0.5f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 5000 }, { filterReso, 0.25f }, { filterDrive, 0.2f }, { filterKeytrk, 0.4f }, { filterEnvAmt, 0.3f },
            { ampAttack, 8 }, { ampDecay, 500 }, { ampSustain, 0.8f }, { ampRelease, 300 },
            { filtAttack, 10 }, { filtDecay, 400 }, { filtSustain, 0.4f }, { filtRelease, 300 },
            { lfo1Wave, 0 }, { lfo1Rate, 5.5f }, { lfo1Dest, 1 }, { lfo1Amount, 0.1f },
            { chorusOn, 1 }, { chorusDepth, 0.5f }, { chorusMix, 0.35f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 11 }, { delayMix, 0.18f },
            { reverbOn, 1 }, { reverbSize, 0.5f }, { reverbMix, 0.25f } } },

        // ============================ BASSES ============================
        { "Reese Monster", "Bass", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -6 },
            { osc1Wave, 0 }, { osc1Unison, 2 }, { osc1Detune, 30 }, { osc1Spread, 0.3f }, { osc1Level, 0.8f },
            { osc2Wave, 0 }, { osc2Unison, 2 }, { osc2Detune, 34 }, { osc2Level, 0.8f }, { osc2Spread, 0.3f },
            { subWave, 1 }, { subOctave, 0 }, { subLevel, 0.4f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 1800 }, { filterReso, 0.35f }, { filterDrive, 0.3f }, { filterEnvAmt, 0.2f },
            { ampAttack, 2 }, { ampDecay, 400 }, { ampSustain, 0.9f }, { ampRelease, 120 },
            { filtAttack, 2 }, { filtDecay, 300 }, { filtSustain, 0.4f }, { filtRelease, 150 },
            { lfo1Wave, 0 }, { lfo1Sync, 1 }, { lfo1SyncRate, 11 }, { lfo1Dest, 4 }, { lfo1Amount, 0.25f },
            { chorusOn, 1 }, { chorusDepth, 0.3f }, { chorusMix, 0.2f } } },

        { "Sub Killer", "Bass", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -5 },
            { osc1Wave, 3 }, { osc1Level, 0.5f },
            { osc2Level, 0.0f },
            { subWave, 0 }, { subOctave, 0 }, { subLevel, 0.9f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 1200 }, { filterReso, 0.1f }, { filterKeytrk, 0.3f }, { filterEnvAmt, 0.0f },
            { ampAttack, 2 }, { ampDecay, 200 }, { ampSustain, 0.95f }, { ampRelease, 80 } } },

        { "Acid 303", "Bass", V {
            { voiceMode, 1 }, { glideTime, 60 }, { glideMode, 0 }, { masterGain, -7 },
            { osc1Wave, 0 }, { osc1Unison, 1 }, { osc1Level, 0.9f },
            { osc2Level, 0.0f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 900 }, { filterReso, 0.8f }, { filterDrive, 0.5f }, { filterKeytrk, 0.5f }, { filterEnvAmt, 0.6f },
            { ampAttack, 2 }, { ampDecay, 200 }, { ampSustain, 0.7f }, { ampRelease, 100 },
            { filtAttack, 2 }, { filtDecay, 260 }, { filtSustain, 0.2f }, { filtRelease, 120 },
            { crushOn, 1 }, { crushDrive, 0.3f }, { crushBits, 12 }, { crushSR, 2 }, { crushShape, 0.3f }, { crushMix, 0.3f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 14 }, { delayFbk, 0.3f }, { delayMix, 0.2f } } },

        { "Growl Bass", "Bass", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -6 },
            { osc1Wave, 0 }, { osc1Unison, 3 }, { osc1Detune, 18 }, { osc1Level, 0.8f },
            { osc2Wave, 1 }, { osc2Fine, -12 }, { osc2Level, 0.4f },
            { subWave, 0 }, { subLevel, 0.5f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 700 }, { filterReso, 0.5f }, { filterDrive, 0.5f }, { filterEnvAmt, 0.3f },
            { ampAttack, 2 }, { ampDecay, 300 }, { ampSustain, 0.9f }, { ampRelease, 100 },
            { filtAttack, 2 }, { filtDecay, 250 }, { filtSustain, 0.5f }, { filtRelease, 150 },
            { lfo1Wave, 2 }, { lfo1Sync, 1 }, { lfo1SyncRate, 12 }, { lfo1Dest, 4 }, { lfo1Amount, 0.5f },
            { lfo2Wave, 0 }, { lfo2Rate, 6 }, { lfo2Dest, 5 }, { lfo2Amount, 0.2f },
            { crushOn, 1 }, { crushDrive, 0.4f }, { crushBits, 9 }, { crushSR, 3 }, { crushShape, 0.5f }, { crushMix, 0.35f } } },

        { "Grime Wobble", "Bass", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -6 },
            { osc1Wave, 0 }, { osc1Unison, 4 }, { osc1Detune, 24 }, { osc1Level, 0.8f },
            { osc2Wave, 0 }, { osc2Fine, 12 }, { osc2Level, 0.5f },
            { subWave, 1 }, { subLevel, 0.5f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 1200 }, { filterReso, 0.4f }, { filterDrive, 0.4f }, { filterEnvAmt, 0.2f },
            { ampAttack, 2 }, { ampDecay, 300 }, { ampSustain, 0.95f }, { ampRelease, 120 },
            { lfo1Wave, 0 }, { lfo1Sync, 1 }, { lfo1SyncRate, 8 }, { lfo1Dest, 4 }, { lfo1Amount, 0.6f },
            { crushOn, 1 }, { crushDrive, 0.5f }, { crushBits, 8 }, { crushSR, 4 }, { crushShape, 0.6f }, { crushTone, 4000 }, { crushReso, 0.4f }, { crushMix, 0.5f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 11 }, { delayFbk, 0.25f }, { delayMix, 0.15f },
            { reverbOn, 1 }, { reverbSize, 0.25f }, { reverbMix, 0.1f } } },

        // ======================= KEYS / PLUCKS =======================
        { "Crystal Pluck", "Keys", V {
            { voiceMode, 0 }, { masterGain, -7 },
            { osc1Wave, 0 }, { osc1Unison, 3 }, { osc1Detune, 15 }, { osc1Spread, 0.6f }, { osc1Level, 0.7f },
            { osc2Wave, 2 }, { osc2Fine, 12 }, { osc2Level, 0.4f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 4000 }, { filterReso, 0.2f }, { filterKeytrk, 0.5f }, { filterEnvAmt, 0.5f },
            { ampAttack, 2 }, { ampDecay, 250 }, { ampSustain, 0.0f }, { ampRelease, 200 },
            { filtAttack, 2 }, { filtDecay, 200 }, { filtSustain, 0.1f }, { filtRelease, 200 },
            { chorusOn, 1 }, { chorusMix, 0.2f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 11 }, { delayFbk, 0.35f }, { delayMix, 0.3f },
            { reverbOn, 1 }, { reverbSize, 0.5f }, { reverbMix, 0.3f } } },

        { "Prism Keys", "Keys", V {
            { voiceMode, 0 }, { masterGain, -7 },
            { osc1Wave, 0 }, { osc1Unison, 2 }, { osc1Detune, 10 }, { osc1Spread, 0.5f }, { osc1Level, 0.6f },
            { osc2Wave, 4 }, { osc2PW, 0.5f }, { osc2Level, 0.4f },
            { subWave, 0 }, { subLevel, 0.2f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 7000 }, { filterReso, 0.15f }, { filterKeytrk, 0.4f }, { filterEnvAmt, 0.2f },
            { ampAttack, 8 }, { ampDecay, 500 }, { ampSustain, 0.6f }, { ampRelease, 300 },
            { filtAttack, 10 }, { filtDecay, 400 }, { filtSustain, 0.4f }, { filtRelease, 300 },
            { chorusOn, 1 }, { chorusDepth, 0.4f }, { chorusMix, 0.3f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 11 }, { delayFbk, 0.25f }, { delayMix, 0.2f },
            { reverbOn, 1 }, { reverbSize, 0.5f }, { reverbMix, 0.3f } } },

        { "Pluck House", "Pluck", V {
            { voiceMode, 0 }, { masterGain, -7 },
            { osc1Wave, 0 }, { osc1Unison, 3 }, { osc1Detune, 20 }, { osc1Level, 0.7f },
            { osc2Wave, 0 }, { osc2Fine, 7 }, { osc2Level, 0.5f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 3000 }, { filterReso, 0.3f }, { filterKeytrk, 0.4f }, { filterEnvAmt, 0.6f },
            { ampAttack, 2 }, { ampDecay, 200 }, { ampSustain, 0.0f }, { ampRelease, 150 },
            { filtAttack, 2 }, { filtDecay, 180 }, { filtSustain, 0.15f }, { filtRelease, 150 },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 11 }, { delayFbk, 0.4f }, { delayPingpong, 0.6f }, { delayMix, 0.35f },
            { reverbOn, 1 }, { reverbSize, 0.4f }, { reverbMix, 0.25f } } },

        // ============================ PADS ============================
        { "Aurora Pad", "Pad", V {
            { voiceMode, 0 }, { masterGain, -8 },
            { osc1Wave, 0 }, { osc1Unison, 5 }, { osc1Detune, 22 }, { osc1Spread, 0.9f }, { osc1Level, 0.6f },
            { osc2Wave, 0 }, { osc2Unison, 5 }, { osc2Detune, 25 }, { osc2Fine, 7 }, { osc2Level, 0.5f }, { osc2Spread, 0.9f },
            { subWave, 0 }, { subLevel, 0.2f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 4500 }, { filterReso, 0.15f }, { filterKeytrk, 0.3f }, { filterEnvAmt, 0.2f },
            { ampAttack, 800 }, { ampDecay, 1500 }, { ampSustain, 0.8f }, { ampRelease, 1500 },
            { filtAttack, 600 }, { filtDecay, 1200 }, { filtSustain, 0.5f }, { filtRelease, 1200 },
            { lfo1Wave, 0 }, { lfo1Rate, 0.3f }, { lfo1Dest, 4 }, { lfo1Amount, 0.2f },
            { chorusOn, 1 }, { chorusDepth, 0.5f }, { chorusMix, 0.45f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 8 }, { delayFbk, 0.3f }, { delayMix, 0.2f },
            { reverbOn, 1 }, { reverbSize, 0.8f }, { reverbDamp, 0.4f }, { reverbMix, 0.45f } } },

        { "Warm Analog Pad", "Pad", V {
            { voiceMode, 0 }, { masterGain, -8 },
            { osc1Wave, 4 }, { osc1PW, 0.5f }, { osc1Unison, 3 }, { osc1Detune, 14 }, { osc1Level, 0.6f },
            { osc2Wave, 4 }, { osc2PW, 0.5f }, { osc2Fine, -12 }, { osc2Level, 0.5f },
            { subWave, 0 }, { subLevel, 0.25f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 3500 }, { filterReso, 0.2f }, { filterKeytrk, 0.3f }, { filterEnvAmt, 0.2f },
            { ampAttack, 600 }, { ampDecay, 1000 }, { ampSustain, 0.85f }, { ampRelease, 1200 },
            { filtAttack, 800 }, { filtDecay, 1000 }, { filtSustain, 0.5f }, { filtRelease, 1000 },
            { lfo1Wave, 0 }, { lfo1Rate, 0.2f }, { lfo1Dest, 2 }, { lfo1Amount, 0.3f },
            { chorusOn, 1 }, { chorusDepth, 0.5f }, { chorusMix, 0.4f },
            { reverbOn, 1 }, { reverbSize, 0.7f }, { reverbMix, 0.4f } } },

        { "Space Drone", "Pad", V {
            { voiceMode, 0 }, { masterGain, -9 },
            { osc1Wave, 0 }, { osc1Unison, 5 }, { osc1Detune, 30 }, { osc1Spread, 1.0f }, { osc1Level, 0.5f },
            { osc2Wave, 2 }, { osc2Fine, 7 }, { osc2Level, 0.4f },
            { subWave, 0 }, { subLevel, 0.2f }, { noiseLevel, 0.05f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 2500 }, { filterReso, 0.25f }, { filterEnvAmt, 0.3f },
            { ampAttack, 1500 }, { ampDecay, 2000 }, { ampSustain, 0.8f }, { ampRelease, 2500 },
            { filtAttack, 2000 }, { filtDecay, 2000 }, { filtSustain, 0.5f }, { filtRelease, 2000 },
            { lfo1Wave, 0 }, { lfo1Rate, 0.12f }, { lfo1Dest, 4 }, { lfo1Amount, 0.4f },
            { lfo2Wave, 0 }, { lfo2Rate, 0.1f }, { lfo2Dest, 1 }, { lfo2Amount, 0.04f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 5 }, { delayFbk, 0.45f }, { delayMix, 0.3f },
            { reverbOn, 1 }, { reverbSize, 0.9f }, { reverbDamp, 0.3f }, { reverbWidth, 1.0f }, { reverbMix, 0.5f } } }
        };

        return presets;
    }

    void applyPreset (juce::AudioProcessorValueTreeState& apvts, const Preset& preset)
    {
        // reset everything to its default, then apply the preset's overrides
        for (const auto& pid : getAllParameterIDs())
            if (auto* rp = apvts.getParameter (pid))
                rp->setValueNotifyingHost (rp->getDefaultValue());

        for (const auto& kv : preset.values)
            if (auto* rp = apvts.getParameter (kv.first))
                rp->setValueNotifyingHost (rp->convertTo0to1 (kv.second));
    }
}

#if defined (_MSC_VER)
 #pragma warning (pop)
#endif
