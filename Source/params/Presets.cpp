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
            reverbOn, reverbSize, reverbDamp, reverbWidth, reverbMix
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
            { reverbOn, 1 }, { reverbSize, 0.9f }, { reverbDamp, 0.3f }, { reverbWidth, 1.0f }, { reverbMix, 0.5f } } },

        // ==================== EXTENDED BANK ====================
        // ============================ LEADS ============================
        { "Hyper Saw Lead", "Lead", V {
            { voiceMode, 1 }, { glideTime, 15 }, { masterGain, -8 },
            { osc1Wave, 0 }, { osc1Unison, 7 }, { osc1Detune, 35 }, { osc1Spread, 1.0f }, { osc1Level, 0.75f },
            { osc2Wave, 0 }, { osc2Unison, 7 }, { osc2Detune, 40 }, { osc2Fine, -8 }, { osc2Level, 0.6f }, { osc2Spread, 1.0f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 11000 }, { filterReso, 0.18f }, { filterKeytrk, 0.4f }, { filterEnvAmt, 0.25f },
            { ampAttack, 6 }, { ampDecay, 700 }, { ampSustain, 0.9f }, { ampRelease, 300 },
            { filtAttack, 8 }, { filtDecay, 500 }, { filtSustain, 0.5f }, { filtRelease, 300 },
            { chorusOn, 1 }, { chorusRate, 0.4f }, { chorusDepth, 0.5f }, { chorusMix, 0.35f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 11 }, { delayFbk, 0.32f }, { delayPingpong, 0.5f }, { delayMix, 0.24f },
            { reverbOn, 1 }, { reverbSize, 0.6f }, { reverbMix, 0.25f } } },

        { "Square Wave Duel", "Lead", V {
            { voiceMode, 2 }, { glideTime, 40 }, { glideMode, 1 }, { masterGain, -8 },
            { osc1Wave, 1 }, { osc1Unison, 2 }, { osc1Detune, 10 }, { osc1Level, 0.8f },
            { osc2Wave, 1 }, { osc2Coarse, 7 }, { osc2Level, 0.55f },
            { filterType, 0 }, { filterSlope, 0 }, { filterCutoff, 6500 }, { filterReso, 0.3f }, { filterKeytrk, 0.5f }, { filterEnvAmt, 0.3f },
            { ampAttack, 4 }, { ampDecay, 400 }, { ampSustain, 0.85f }, { ampRelease, 180 },
            { filtAttack, 4 }, { filtDecay, 350 }, { filtSustain, 0.45f }, { filtRelease, 200 },
            { lfo1Wave, 0 }, { lfo1Rate, 5.0f }, { lfo1Dest, 1 }, { lfo1Amount, 0.08f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 10 }, { delayFbk, 0.3f }, { delayMix, 0.2f },
            { reverbOn, 1 }, { reverbSize, 0.4f }, { reverbMix, 0.18f } } },

        { "PWM Cry Lead", "Lead", V {
            { voiceMode, 1 }, { glideTime, 25 }, { masterGain, -8 },
            { osc1Wave, 4 }, { osc1PW, 0.4f }, { osc1Unison, 3 }, { osc1Detune, 14 }, { osc1Level, 0.85f },
            { osc2Wave, 4 }, { osc2PW, 0.6f }, { osc2Fine, 6 }, { osc2Level, 0.45f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 5500 }, { filterReso, 0.35f }, { filterDrive, 0.2f }, { filterKeytrk, 0.45f }, { filterEnvAmt, 0.3f },
            { ampAttack, 12 }, { ampDecay, 600 }, { ampSustain, 0.8f }, { ampRelease, 350 },
            { filtAttack, 15 }, { filtDecay, 450 }, { filtSustain, 0.4f }, { filtRelease, 350 },
            { lfo1Wave, 0 }, { lfo1Rate, 4.5f }, { lfo1Dest, 2 }, { lfo1Amount, 0.35f },
            { lfo2Wave, 0 }, { lfo2Rate, 5.2f }, { lfo2Dest, 1 }, { lfo2Amount, 0.06f },
            { chorusOn, 1 }, { chorusDepth, 0.45f }, { chorusMix, 0.3f },
            { reverbOn, 1 }, { reverbSize, 0.55f }, { reverbMix, 0.28f } } },

        { "Distortion Shred", "Lead", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -9 },
            { osc1Wave, 0 }, { osc1Unison, 4 }, { osc1Detune, 20 }, { osc1Level, 0.85f },
            { osc2Wave, 1 }, { osc2Fine, -5 }, { osc2Level, 0.6f },
            { subWave, 0 }, { subLevel, 0.25f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 4200 }, { filterReso, 0.5f }, { filterDrive, 0.75f }, { filterEnvAmt, 0.45f },
            { ampAttack, 2 }, { ampDecay, 300 }, { ampSustain, 0.9f }, { ampRelease, 120 },
            { filtAttack, 2 }, { filtDecay, 250 }, { filtSustain, 0.35f }, { filtRelease, 150 },
            { crushOn, 1 }, { crushDrive, 0.7f }, { crushBits, 7 }, { crushSR, 3 }, { crushShape, 0.65f }, { crushTone, 5000 }, { crushReso, 0.4f }, { crushMix, 0.55f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 11 }, { delayFbk, 0.35f }, { delayMix, 0.2f },
            { reverbOn, 1 }, { reverbSize, 0.35f }, { reverbMix, 0.15f } } },

        { "Portamento Flute", "Lead", V {
            { voiceMode, 2 }, { glideTime, 90 }, { glideMode, 1 }, { masterGain, -7 },
            { osc1Wave, 2 }, { osc1Unison, 1 }, { osc1Level, 0.8f },
            { osc2Wave, 3 }, { osc2Fine, 4 }, { osc2Level, 0.4f },
            { noiseLevel, 0.06f }, { noiseColor, 1 },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 4000 }, { filterReso, 0.15f }, { filterKeytrk, 0.6f }, { filterEnvAmt, 0.15f },
            { ampAttack, 60 }, { ampDecay, 500 }, { ampSustain, 0.85f }, { ampRelease, 400 },
            { filtAttack, 50 }, { filtDecay, 400 }, { filtSustain, 0.6f }, { filtRelease, 400 },
            { lfo1Wave, 0 }, { lfo1Rate, 5.5f }, { lfo1Dest, 1 }, { lfo1Amount, 0.05f },
            { chorusOn, 1 }, { chorusDepth, 0.3f }, { chorusMix, 0.2f },
            { reverbOn, 1 }, { reverbSize, 0.7f }, { reverbMix, 0.35f } } },

        { "Retro Arcade Lead", "Lead", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -7 },
            { osc1Wave, 1 }, { osc1Unison, 1 }, { osc1Level, 0.9f },
            { osc2Wave, 2 }, { osc2Coarse, -12 }, { osc2Level, 0.35f },
            { filterType, 0 }, { filterSlope, 0 }, { filterCutoff, 12000 }, { filterReso, 0.1f },
            { ampAttack, 2 }, { ampDecay, 120 }, { ampSustain, 0.55f }, { ampRelease, 60 },
            { crushOn, 1 }, { crushDrive, 0.35f }, { crushBits, 4 }, { crushSR, 8 }, { crushShape, 0.4f }, { crushTone, 10000 }, { crushReso, 0.25f }, { crushMix, 0.9f },
            { lfo1Wave, 4 }, { lfo1Sync, 1 }, { lfo1SyncRate, 14 }, { lfo1Dest, 1 }, { lfo1Amount, 0.2f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 12 }, { delayFbk, 0.4f }, { delayMix, 0.25f } } },

        { "Trance Supersaw", "Lead", V {
            { voiceMode, 0 }, { masterGain, -9 },
            { osc1Wave, 0 }, { osc1Unison, 7 }, { osc1Detune, 30 }, { osc1Spread, 0.9f }, { osc1Level, 0.7f },
            { osc2Wave, 0 }, { osc2Unison, 7 }, { osc2Detune, 33 }, { osc2Coarse, 12 }, { osc2Level, 0.45f }, { osc2Spread, 0.9f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 10000 }, { filterReso, 0.2f }, { filterKeytrk, 0.4f }, { filterEnvAmt, 0.3f },
            { ampAttack, 10 }, { ampDecay, 800 }, { ampSustain, 0.85f }, { ampRelease, 400 },
            { filtAttack, 12 }, { filtDecay, 600 }, { filtSustain, 0.5f }, { filtRelease, 400 },
            { chorusOn, 1 }, { chorusDepth, 0.5f }, { chorusMix, 0.4f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 10 }, { delayFbk, 0.35f }, { delayPingpong, 0.7f }, { delayMix, 0.28f },
            { reverbOn, 1 }, { reverbSize, 0.7f }, { reverbMix, 0.3f } } },

        { "Sync Scream", "Lead", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -8 },
            { osc1Wave, 0 }, { osc1Unison, 1 }, { osc1Level, 0.85f },
            { osc2Wave, 0 }, { osc2Coarse, 5 }, { osc2Sync, 1 }, { osc2Level, 0.7f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 8000 }, { filterReso, 0.25f }, { filterDrive, 0.3f }, { filterEnvAmt, 0.4f },
            { ampAttack, 3 }, { ampDecay, 400 }, { ampSustain, 0.8f }, { ampRelease, 150 },
            { filtAttack, 3 }, { filtDecay, 350 }, { filtSustain, 0.4f }, { filtRelease, 200 },
            { lfo1Wave, 1 }, { lfo1Rate, 3.0f }, { lfo1Dest, 1 }, { lfo1Amount, 0.5f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 11 }, { delayFbk, 0.3f }, { delayMix, 0.2f },
            { reverbOn, 1 }, { reverbSize, 0.45f }, { reverbMix, 0.2f } } },

        { "Whistle Lead", "Lead", V {
            { voiceMode, 2 }, { glideTime, 70 }, { glideMode, 1 }, { masterGain, -7 },
            { osc1Wave, 3 }, { osc1Unison, 1 }, { osc1Level, 0.9f },
            { osc2Wave, 3 }, { osc2Fine, 3 }, { osc2Level, 0.3f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 8000 }, { filterReso, 0.1f }, { filterKeytrk, 0.7f },
            { ampAttack, 30 }, { ampDecay, 400 }, { ampSustain, 0.9f }, { ampRelease, 300 },
            { lfo1Wave, 0 }, { lfo1Rate, 6.0f }, { lfo1Dest, 1 }, { lfo1Amount, 0.07f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 11 }, { delayFbk, 0.4f }, { delayPingpong, 0.6f }, { delayMix, 0.3f },
            { reverbOn, 1 }, { reverbSize, 0.75f }, { reverbMix, 0.4f } } },

        { "Fifth Stack Lead", "Lead", V {
            { voiceMode, 1 }, { glideTime, 20 }, { masterGain, -8 },
            { osc1Wave, 0 }, { osc1Unison, 3 }, { osc1Detune, 16 }, { osc1Level, 0.8f },
            { osc2Wave, 0 }, { osc2Coarse, 7 }, { osc2Unison, 3 }, { osc2Detune, 18 }, { osc2Level, 0.55f },
            { subWave, 0 }, { subLevel, 0.3f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 7000 }, { filterReso, 0.2f }, { filterKeytrk, 0.4f }, { filterEnvAmt, 0.25f },
            { ampAttack, 6 }, { ampDecay, 600 }, { ampSustain, 0.85f }, { ampRelease, 250 },
            { filtAttack, 8 }, { filtDecay, 450 }, { filtSustain, 0.5f }, { filtRelease, 250 },
            { chorusOn, 1 }, { chorusDepth, 0.4f }, { chorusMix, 0.3f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 11 }, { delayFbk, 0.3f }, { delayMix, 0.2f },
            { reverbOn, 1 }, { reverbSize, 0.5f }, { reverbMix, 0.22f } } },

        { "Digital Bell Lead", "Lead", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -8 },
            { osc1Wave, 3 }, { osc1Level, 0.8f },
            { osc2Wave, 1 }, { osc2Coarse, 19 }, { osc2Level, 0.35f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 9000 }, { filterReso, 0.15f }, { filterKeytrk, 0.5f }, { filterEnvAmt, 0.4f },
            { ampAttack, 2 }, { ampDecay, 500 }, { ampSustain, 0.4f }, { ampRelease, 300 },
            { filtAttack, 2 }, { filtDecay, 300 }, { filtSustain, 0.2f }, { filtRelease, 300 },
            { crushOn, 1 }, { crushDrive, 0.25f }, { crushBits, 10 }, { crushSR, 2 }, { crushShape, 0.3f }, { crushTone, 9000 }, { crushMix, 0.35f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 12 }, { delayFbk, 0.45f }, { delayPingpong, 0.7f }, { delayMix, 0.3f },
            { reverbOn, 1 }, { reverbSize, 0.6f }, { reverbMix, 0.3f } } },

        { "Vintage Brass Lead", "Lead", V {
            { voiceMode, 2 }, { glideTime, 30 }, { glideMode, 1 }, { masterGain, -8 },
            { osc1Wave, 0 }, { osc1Unison, 2 }, { osc1Detune, 12 }, { osc1Level, 0.8f },
            { osc2Wave, 1 }, { osc2Fine, -6 }, { osc2Level, 0.5f },
            { subWave, 0 }, { subLevel, 0.2f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 3800 }, { filterReso, 0.3f }, { filterDrive, 0.25f }, { filterKeytrk, 0.4f }, { filterEnvAmt, 0.55f },
            { ampAttack, 40 }, { ampDecay, 700 }, { ampSustain, 0.75f }, { ampRelease, 300 },
            { filtAttack, 60 }, { filtDecay, 500 }, { filtSustain, 0.45f }, { filtRelease, 300 },
            { lfo1Wave, 0 }, { lfo1Rate, 4.8f }, { lfo1Dest, 1 }, { lfo1Amount, 0.06f },
            { chorusOn, 1 }, { chorusDepth, 0.4f }, { chorusMix, 0.28f },
            { reverbOn, 1 }, { reverbSize, 0.55f }, { reverbMix, 0.25f } } },

        // ============================ BASSES ============================
        { "Deep House Bass", "Bass", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -6 },
            { osc1Wave, 0 }, { osc1Unison, 1 }, { osc1Level, 0.75f },
            { osc2Wave, 2 }, { osc2Fine, 4 }, { osc2Level, 0.4f },
            { subWave, 0 }, { subOctave, 0 }, { subLevel, 0.6f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 900 }, { filterReso, 0.2f }, { filterKeytrk, 0.3f }, { filterEnvAmt, 0.35f },
            { ampAttack, 3 }, { ampDecay, 350 }, { ampSustain, 0.8f }, { ampRelease, 120 },
            { filtAttack, 3 }, { filtDecay, 280 }, { filtSustain, 0.3f }, { filtRelease, 150 },
            { chorusOn, 1 }, { chorusDepth, 0.25f }, { chorusMix, 0.18f } } },

        { "Neuro Wobble", "Bass", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -7 },
            { osc1Wave, 0 }, { osc1Unison, 3 }, { osc1Detune, 22 }, { osc1Level, 0.8f },
            { osc2Wave, 0 }, { osc2Fine, 10 }, { osc2Level, 0.6f },
            { subWave, 1 }, { subLevel, 0.5f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 1400 }, { filterReso, 0.6f }, { filterDrive, 0.5f }, { filterEnvAmt, 0.3f },
            { ampAttack, 2 }, { ampDecay, 400 }, { ampSustain, 0.95f }, { ampRelease, 120 },
            { lfo1Wave, 3 }, { lfo1Sync, 1 }, { lfo1SyncRate, 12 }, { lfo1Dest, 4 }, { lfo1Amount, 0.7f },
            { lfo2Wave, 1 }, { lfo2Sync, 1 }, { lfo2SyncRate, 14 }, { lfo2Dest, 5 }, { lfo2Amount, 0.4f },
            { crushOn, 1 }, { crushDrive, 0.55f }, { crushBits, 8 }, { crushSR, 4 }, { crushShape, 0.6f }, { crushTone, 4500 }, { crushReso, 0.45f }, { crushMix, 0.5f } } },

        { "Plucky Sub Bass", "Bass", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -6 },
            { osc1Wave, 2 }, { osc1Level, 0.6f },
            { subWave, 0 }, { subOctave, 0 }, { subLevel, 0.7f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 1500 }, { filterReso, 0.25f }, { filterKeytrk, 0.4f }, { filterEnvAmt, 0.5f },
            { ampAttack, 2 }, { ampDecay, 250 }, { ampSustain, 0.0f }, { ampRelease, 100 },
            { filtAttack, 2 }, { filtDecay, 180 }, { filtSustain, 0.1f }, { filtRelease, 120 } } },

        { "Analog 808 Bass", "Bass", V {
            { voiceMode, 2 }, { glideTime, 50 }, { glideMode, 1 }, { masterGain, -5 },
            { osc1Wave, 3 }, { osc1Level, 0.5f },
            { subWave, 0 }, { subOctave, 1 }, { subLevel, 0.95f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 800 }, { filterReso, 0.1f }, { filterKeytrk, 0.2f },
            { ampAttack, 2 }, { ampDecay, 600 }, { ampSustain, 0.7f }, { ampRelease, 300 },
            { crushOn, 1 }, { crushDrive, 0.4f }, { crushBits, 12 }, { crushSR, 1 }, { crushShape, 0.5f }, { crushMix, 0.3f } } },

        { "Square Sub Bass", "Bass", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -6 },
            { osc1Wave, 1 }, { osc1Level, 0.7f },
            { osc2Level, 0.0f },
            { subWave, 1 }, { subOctave, 0 }, { subLevel, 0.7f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 1100 }, { filterReso, 0.2f }, { filterKeytrk, 0.35f }, { filterEnvAmt, 0.25f },
            { ampAttack, 2 }, { ampDecay, 300 }, { ampSustain, 0.9f }, { ampRelease, 100 },
            { filtAttack, 2 }, { filtDecay, 250 }, { filtSustain, 0.4f }, { filtRelease, 120 } } },

        { "Detuned Saw Bass", "Bass", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -6 },
            { osc1Wave, 0 }, { osc1Unison, 4 }, { osc1Detune, 20 }, { osc1Spread, 0.4f }, { osc1Level, 0.8f },
            { osc2Wave, 0 }, { osc2Coarse, -12 }, { osc2Level, 0.6f },
            { subWave, 0 }, { subLevel, 0.4f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 1600 }, { filterReso, 0.3f }, { filterDrive, 0.3f }, { filterEnvAmt, 0.3f },
            { ampAttack, 2 }, { ampDecay, 400 }, { ampSustain, 0.85f }, { ampRelease, 130 },
            { filtAttack, 2 }, { filtDecay, 300 }, { filtSustain, 0.4f }, { filtRelease, 150 },
            { chorusOn, 1 }, { chorusDepth, 0.3f }, { chorusMix, 0.2f } } },

        { "Formant Talk Bass", "Bass", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -7 },
            { osc1Wave, 0 }, { osc1Unison, 2 }, { osc1Detune, 14 }, { osc1Level, 0.8f },
            { osc2Wave, 1 }, { osc2Fine, 8 }, { osc2Level, 0.5f },
            { subWave, 0 }, { subLevel, 0.5f },
            { filterType, 2 }, { filterSlope, 1 }, { filterCutoff, 1200 }, { filterReso, 0.7f }, { filterDrive, 0.4f }, { filterEnvAmt, 0.2f },
            { ampAttack, 2 }, { ampDecay, 350 }, { ampSustain, 0.9f }, { ampRelease, 120 },
            { lfo1Wave, 0 }, { lfo1Sync, 1 }, { lfo1SyncRate, 11 }, { lfo1Dest, 4 }, { lfo1Amount, 0.55f },
            { crushOn, 1 }, { crushDrive, 0.35f }, { crushBits, 9 }, { crushSR, 3 }, { crushShape, 0.55f }, { crushTone, 3500 }, { crushReso, 0.5f }, { crushMix, 0.4f } } },

        { "Dub Bass", "Bass", V {
            { voiceMode, 2 }, { glideTime, 80 }, { glideMode, 1 }, { masterGain, -6 },
            { osc1Wave, 3 }, { osc1Level, 0.6f },
            { osc2Wave, 2 }, { osc2Fine, -6 }, { osc2Level, 0.4f },
            { subWave, 0 }, { subOctave, 0 }, { subLevel, 0.8f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 600 }, { filterReso, 0.3f }, { filterKeytrk, 0.25f }, { filterEnvAmt, 0.2f },
            { ampAttack, 5 }, { ampDecay, 500 }, { ampSustain, 0.85f }, { ampRelease, 250 },
            { filtAttack, 5 }, { filtDecay, 400 }, { filtSustain, 0.35f }, { filtRelease, 250 },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 8 }, { delayFbk, 0.4f }, { delayTone, 3000 }, { delayMix, 0.22f },
            { reverbOn, 1 }, { reverbSize, 0.4f }, { reverbMix, 0.15f } } },

        { "Hoover Bass", "Bass", V {
            { voiceMode, 1 }, { glideTime, 30 }, { masterGain, -8 },
            { osc1Wave, 0 }, { osc1Unison, 5 }, { osc1Detune, 28 }, { osc1Spread, 0.5f }, { osc1Level, 0.75f },
            { osc2Wave, 1 }, { osc2Coarse, -5 }, { osc2Level, 0.55f },
            { subWave, 1 }, { subLevel, 0.45f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 2200 }, { filterReso, 0.4f }, { filterDrive, 0.4f }, { filterEnvAmt, 0.35f },
            { ampAttack, 4 }, { ampDecay, 450 }, { ampSustain, 0.9f }, { ampRelease, 180 },
            { filtAttack, 4 }, { filtDecay, 350 }, { filtSustain, 0.4f }, { filtRelease, 200 },
            { lfo1Wave, 0 }, { lfo1Rate, 5.0f }, { lfo1Dest, 1 }, { lfo1Amount, 0.1f },
            { chorusOn, 1 }, { chorusDepth, 0.4f }, { chorusMix, 0.3f } } },

        { "Metallic Reese", "Bass", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -7 },
            { osc1Wave, 0 }, { osc1Unison, 2 }, { osc1Detune, 40 }, { osc1Level, 0.8f },
            { osc2Wave, 0 }, { osc2Unison, 2 }, { osc2Detune, 45 }, { osc2Fine, 5 }, { osc2Level, 0.8f },
            { subWave, 1 }, { subLevel, 0.4f },
            { filterType, 3 }, { filterSlope, 1 }, { filterCutoff, 1500 }, { filterReso, 0.5f }, { filterDrive, 0.5f }, { filterEnvAmt, 0.2f },
            { ampAttack, 2 }, { ampDecay, 400 }, { ampSustain, 0.95f }, { ampRelease, 120 },
            { lfo1Wave, 1 }, { lfo1Sync, 1 }, { lfo1SyncRate, 12 }, { lfo1Dest, 4 }, { lfo1Amount, 0.45f },
            { crushOn, 1 }, { crushDrive, 0.5f }, { crushBits, 7 }, { crushSR, 4 }, { crushShape, 0.6f }, { crushTone, 4000 }, { crushReso, 0.4f }, { crushMix, 0.45f } } },

        { "Warm Finger Bass", "Bass", V {
            { voiceMode, 2 }, { glideTime, 40 }, { glideMode, 1 }, { masterGain, -6 },
            { osc1Wave, 0 }, { osc1Unison, 1 }, { osc1Level, 0.75f },
            { osc2Wave, 2 }, { osc2Fine, 3 }, { osc2Level, 0.35f },
            { subWave, 0 }, { subLevel, 0.5f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 1000 }, { filterReso, 0.2f }, { filterKeytrk, 0.4f }, { filterEnvAmt, 0.45f },
            { ampAttack, 4 }, { ampDecay, 400 }, { ampSustain, 0.7f }, { ampRelease, 150 },
            { filtAttack, 4 }, { filtDecay, 300 }, { filtSustain, 0.25f }, { filtRelease, 160 },
            { chorusOn, 1 }, { chorusDepth, 0.25f }, { chorusMix, 0.15f } } },

        { "Tremolo Sub", "Bass", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -6 },
            { osc1Wave, 2 }, { osc1Level, 0.55f },
            { subWave, 0 }, { subOctave, 0 }, { subLevel, 0.75f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 1000 }, { filterReso, 0.15f }, { filterKeytrk, 0.3f },
            { ampAttack, 2 }, { ampDecay, 300 }, { ampSustain, 0.9f }, { ampRelease, 100 },
            { lfo1Wave, 0 }, { lfo1Sync, 1 }, { lfo1SyncRate, 14 }, { lfo1Dest, 6 }, { lfo1Amount, 0.5f } } },

        { "Gnarly Growl", "Bass", V {
            { voiceMode, 1 }, { glideTime, 0 }, { masterGain, -7 },
            { osc1Wave, 0 }, { osc1Unison, 4 }, { osc1Detune, 26 }, { osc1Level, 0.8f },
            { osc2Wave, 1 }, { osc2Coarse, -12 }, { osc2Level, 0.55f },
            { subWave, 1 }, { subLevel, 0.5f },
            { filterType, 2 }, { filterSlope, 1 }, { filterCutoff, 1000 }, { filterReso, 0.6f }, { filterDrive, 0.6f }, { filterEnvAmt, 0.3f },
            { ampAttack, 2 }, { ampDecay, 350 }, { ampSustain, 0.9f }, { ampRelease, 120 },
            { filtAttack, 2 }, { filtDecay, 250 }, { filtSustain, 0.4f }, { filtRelease, 150 },
            { lfo1Wave, 5 }, { lfo1Sync, 1 }, { lfo1SyncRate, 13 }, { lfo1Dest, 4 }, { lfo1Amount, 0.6f },
            { crushOn, 1 }, { crushDrive, 0.6f }, { crushBits, 6 }, { crushSR, 5 }, { crushShape, 0.7f }, { crushTone, 3500 }, { crushReso, 0.5f }, { crushMix, 0.5f } } },

        // ======================= KEYS / PLUCKS =======================
        { "Electric Piano", "Keys", V {
            { voiceMode, 0 }, { masterGain, -7 },
            { osc1Wave, 3 }, { osc1Level, 0.8f },
            { osc2Wave, 3 }, { osc2Coarse, 12 }, { osc2Level, 0.3f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 5000 }, { filterReso, 0.12f }, { filterKeytrk, 0.5f }, { filterEnvAmt, 0.4f },
            { ampAttack, 2 }, { ampDecay, 900 }, { ampSustain, 0.35f }, { ampRelease, 400 },
            { filtAttack, 2 }, { filtDecay, 600 }, { filtSustain, 0.2f }, { filtRelease, 400 },
            { chorusOn, 1 }, { chorusDepth, 0.4f }, { chorusMix, 0.3f },
            { reverbOn, 1 }, { reverbSize, 0.5f }, { reverbMix, 0.25f } } },

        { "Bell Keys", "Keys", V {
            { voiceMode, 0 }, { masterGain, -8 },
            { osc1Wave, 3 }, { osc1Level, 0.7f },
            { osc2Wave, 3 }, { osc2Coarse, 19 }, { osc2Fine, 4 }, { osc2Level, 0.4f },
            { subWave, 0 }, { subLevel, 0.15f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 8000 }, { filterReso, 0.1f }, { filterKeytrk, 0.5f }, { filterEnvAmt, 0.3f },
            { ampAttack, 2 }, { ampDecay, 1200 }, { ampSustain, 0.25f }, { ampRelease, 700 },
            { filtAttack, 2 }, { filtDecay, 800 }, { filtSustain, 0.2f }, { filtRelease, 700 },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 12 }, { delayFbk, 0.4f }, { delayPingpong, 0.6f }, { delayMix, 0.28f },
            { reverbOn, 1 }, { reverbSize, 0.7f }, { reverbMix, 0.35f } } },

        { "Marimba Pluck", "Pluck", V {
            { voiceMode, 0 }, { masterGain, -7 },
            { osc1Wave, 2 }, { osc1Level, 0.85f },
            { osc2Wave, 3 }, { osc2Coarse, 12 }, { osc2Level, 0.3f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 4500 }, { filterReso, 0.15f }, { filterKeytrk, 0.6f }, { filterEnvAmt, 0.4f },
            { ampAttack, 2 }, { ampDecay, 350 }, { ampSustain, 0.0f }, { ampRelease, 200 },
            { filtAttack, 2 }, { filtDecay, 250 }, { filtSustain, 0.0f }, { filtRelease, 200 },
            { reverbOn, 1 }, { reverbSize, 0.5f }, { reverbMix, 0.25f } } },

        { "Koto Pluck", "Pluck", V {
            { voiceMode, 0 }, { masterGain, -7 },
            { osc1Wave, 0 }, { osc1Unison, 2 }, { osc1Detune, 10 }, { osc1Level, 0.75f },
            { osc2Wave, 2 }, { osc2Fine, 6 }, { osc2Level, 0.4f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 5500 }, { filterReso, 0.3f }, { filterKeytrk, 0.5f }, { filterEnvAmt, 0.55f },
            { ampAttack, 2 }, { ampDecay, 500 }, { ampSustain, 0.0f }, { ampRelease, 300 },
            { filtAttack, 2 }, { filtDecay, 300 }, { filtSustain, 0.1f }, { filtRelease, 250 },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 11 }, { delayFbk, 0.35f }, { delayMix, 0.25f },
            { reverbOn, 1 }, { reverbSize, 0.55f }, { reverbMix, 0.3f } } },

        { "Music Box", "Keys", V {
            { voiceMode, 0 }, { masterGain, -8 },
            { osc1Wave, 3 }, { osc1Level, 0.8f },
            { osc2Wave, 3 }, { osc2Coarse, 24 }, { osc2Level, 0.35f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 7000 }, { filterReso, 0.1f }, { filterKeytrk, 0.6f },
            { ampAttack, 2 }, { ampDecay, 700 }, { ampSustain, 0.0f }, { ampRelease, 500 },
            { chorusOn, 1 }, { chorusDepth, 0.3f }, { chorusMix, 0.2f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 12 }, { delayFbk, 0.3f }, { delayMix, 0.22f },
            { reverbOn, 1 }, { reverbSize, 0.7f }, { reverbMix, 0.4f } } },

        { "Vibraphone", "Keys", V {
            { voiceMode, 0 }, { masterGain, -7 },
            { osc1Wave, 3 }, { osc1Level, 0.8f },
            { osc2Wave, 3 }, { osc2Coarse, 12 }, { osc2Fine, 2 }, { osc2Level, 0.35f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 5500 }, { filterReso, 0.12f }, { filterKeytrk, 0.5f }, { filterEnvAmt, 0.25f },
            { ampAttack, 3 }, { ampDecay, 1500 }, { ampSustain, 0.4f }, { ampRelease, 800 },
            { lfo1Wave, 0 }, { lfo1Rate, 4.5f }, { lfo1Dest, 6 }, { lfo1Amount, 0.3f },
            { reverbOn, 1 }, { reverbSize, 0.6f }, { reverbMix, 0.3f } } },

        { "Nylon Pluck", "Pluck", V {
            { voiceMode, 0 }, { masterGain, -7 },
            { osc1Wave, 2 }, { osc1Level, 0.8f },
            { osc2Wave, 0 }, { osc2Fine, 5 }, { osc2Level, 0.4f },
            { noiseLevel, 0.05f }, { noiseColor, 1 },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 4000 }, { filterReso, 0.2f }, { filterKeytrk, 0.55f }, { filterEnvAmt, 0.5f },
            { ampAttack, 2 }, { ampDecay, 450 }, { ampSustain, 0.0f }, { ampRelease, 250 },
            { filtAttack, 2 }, { filtDecay, 300 }, { filtSustain, 0.1f }, { filtRelease, 220 },
            { reverbOn, 1 }, { reverbSize, 0.45f }, { reverbMix, 0.22f } } },

        { "Trance Pluck", "Pluck", V {
            { voiceMode, 0 }, { masterGain, -8 },
            { osc1Wave, 0 }, { osc1Unison, 5 }, { osc1Detune, 22 }, { osc1Spread, 0.8f }, { osc1Level, 0.7f },
            { osc2Wave, 1 }, { osc2Coarse, 12 }, { osc2Level, 0.4f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 4500 }, { filterReso, 0.35f }, { filterKeytrk, 0.4f }, { filterEnvAmt, 0.6f },
            { ampAttack, 2 }, { ampDecay, 280 }, { ampSustain, 0.0f }, { ampRelease, 180 },
            { filtAttack, 2 }, { filtDecay, 220 }, { filtSustain, 0.1f }, { filtRelease, 180 },
            { chorusOn, 1 }, { chorusDepth, 0.4f }, { chorusMix, 0.25f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 11 }, { delayFbk, 0.45f }, { delayPingpong, 0.7f }, { delayMix, 0.32f },
            { reverbOn, 1 }, { reverbSize, 0.6f }, { reverbMix, 0.3f } } },

        { "Glass Bells", "Keys", V {
            { voiceMode, 0 }, { masterGain, -8 },
            { osc1Wave, 3 }, { osc1Level, 0.7f },
            { osc2Wave, 1 }, { osc2Coarse, 12 }, { osc2Sync, 1 }, { osc2Level, 0.35f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 9000 }, { filterReso, 0.1f }, { filterKeytrk, 0.6f },
            { ampAttack, 2 }, { ampDecay, 1000 }, { ampSustain, 0.2f }, { ampRelease, 600 },
            { crushOn, 1 }, { crushDrive, 0.2f }, { crushBits, 11 }, { crushSR, 2 }, { crushShape, 0.25f }, { crushTone, 10000 }, { crushMix, 0.3f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 13 }, { delayFbk, 0.4f }, { delayPingpong, 0.6f }, { delayMix, 0.3f },
            { reverbOn, 1 }, { reverbSize, 0.8f }, { reverbDamp, 0.3f }, { reverbMix, 0.4f } } },

        // ============================ PADS ============================
        { "Warm String Pad", "Pad", V {
            { voiceMode, 0 }, { masterGain, -8 },
            { osc1Wave, 0 }, { osc1Unison, 5 }, { osc1Detune, 18 }, { osc1Spread, 0.9f }, { osc1Level, 0.6f },
            { osc2Wave, 0 }, { osc2Unison, 5 }, { osc2Detune, 20 }, { osc2Fine, 5 }, { osc2Level, 0.5f }, { osc2Spread, 0.9f },
            { subWave, 0 }, { subLevel, 0.2f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 3800 }, { filterReso, 0.15f }, { filterKeytrk, 0.3f }, { filterEnvAmt, 0.2f },
            { ampAttack, 700 }, { ampDecay, 1200 }, { ampSustain, 0.85f }, { ampRelease, 1200 },
            { filtAttack, 800 }, { filtDecay, 1000 }, { filtSustain, 0.55f }, { filtRelease, 1000 },
            { lfo1Wave, 0 }, { lfo1Rate, 0.25f }, { lfo1Dest, 4 }, { lfo1Amount, 0.2f },
            { chorusOn, 1 }, { chorusDepth, 0.5f }, { chorusMix, 0.4f },
            { reverbOn, 1 }, { reverbSize, 0.75f }, { reverbMix, 0.4f } } },

        { "Cinematic Sweep Pad", "Pad", V {
            { voiceMode, 0 }, { masterGain, -9 },
            { osc1Wave, 0 }, { osc1Unison, 7 }, { osc1Detune, 28 }, { osc1Spread, 1.0f }, { osc1Level, 0.55f },
            { osc2Wave, 2 }, { osc2Coarse, 12 }, { osc2Level, 0.4f },
            { subWave, 0 }, { subLevel, 0.2f }, { noiseLevel, 0.04f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 2500 }, { filterReso, 0.25f }, { filterEnvAmt, 0.6f },
            { ampAttack, 1200 }, { ampDecay, 2000 }, { ampSustain, 0.8f }, { ampRelease, 2000 },
            { filtAttack, 2500 }, { filtDecay, 2000 }, { filtSustain, 0.6f }, { filtRelease, 1800 },
            { lfo1Wave, 0 }, { lfo1Rate, 0.15f }, { lfo1Dest, 4 }, { lfo1Amount, 0.4f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 5 }, { delayFbk, 0.4f }, { delayMix, 0.25f },
            { reverbOn, 1 }, { reverbSize, 0.9f }, { reverbDamp, 0.3f }, { reverbWidth, 1.0f }, { reverbMix, 0.5f } } },

        { "Glass Choir Pad", "Pad", V {
            { voiceMode, 0 }, { masterGain, -8 },
            { osc1Wave, 3 }, { osc1Unison, 3 }, { osc1Detune, 12 }, { osc1Spread, 0.8f }, { osc1Level, 0.6f },
            { osc2Wave, 4 }, { osc2PW, 0.5f }, { osc2Fine, 7 }, { osc2Level, 0.45f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 5000 }, { filterReso, 0.15f }, { filterKeytrk, 0.4f }, { filterEnvAmt, 0.2f },
            { ampAttack, 900 }, { ampDecay, 1500 }, { ampSustain, 0.85f }, { ampRelease, 1500 },
            { filtAttack, 1000 }, { filtDecay, 1200 }, { filtSustain, 0.6f }, { filtRelease, 1200 },
            { lfo1Wave, 0 }, { lfo1Rate, 0.35f }, { lfo1Dest, 3 }, { lfo1Amount, 0.35f },
            { chorusOn, 1 }, { chorusDepth, 0.6f }, { chorusMix, 0.5f },
            { reverbOn, 1 }, { reverbSize, 0.85f }, { reverbMix, 0.45f } } },

        { "Dark Ambient Pad", "Pad", V {
            { voiceMode, 0 }, { masterGain, -9 },
            { osc1Wave, 0 }, { osc1Unison, 5 }, { osc1Detune, 24 }, { osc1Spread, 1.0f }, { osc1Level, 0.55f },
            { osc2Wave, 1 }, { osc2Coarse, -12 }, { osc2Level, 0.4f },
            { subWave, 0 }, { subOctave, 1 }, { subLevel, 0.3f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 1400 }, { filterReso, 0.3f }, { filterEnvAmt, 0.25f },
            { ampAttack, 1500 }, { ampDecay, 2500 }, { ampSustain, 0.75f }, { ampRelease, 2500 },
            { filtAttack, 2000 }, { filtDecay, 2500 }, { filtSustain, 0.5f }, { filtRelease, 2000 },
            { lfo1Wave, 0 }, { lfo1Rate, 0.08f }, { lfo1Dest, 4 }, { lfo1Amount, 0.3f },
            { lfo2Wave, 5 }, { lfo2Rate, 0.2f }, { lfo2Dest, 7 }, { lfo2Amount, 0.3f },
            { reverbOn, 1 }, { reverbSize, 0.95f }, { reverbDamp, 0.6f }, { reverbWidth, 1.0f }, { reverbMix, 0.5f } } },

        { "Evolving Texture Pad", "Pad", V {
            { voiceMode, 0 }, { masterGain, -9 },
            { osc1Wave, 4 }, { osc1PW, 0.4f }, { osc1Unison, 4 }, { osc1Detune, 20 }, { osc1Spread, 0.9f }, { osc1Level, 0.55f },
            { osc2Wave, 0 }, { osc2Unison, 3 }, { osc2Detune, 26 }, { osc2Fine, -7 }, { osc2Level, 0.45f },
            { noiseLevel, 0.05f }, { noiseColor, 1 },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 3000 }, { filterReso, 0.3f }, { filterEnvAmt, 0.3f },
            { ampAttack, 1000 }, { ampDecay, 2000 }, { ampSustain, 0.8f }, { ampRelease, 2000 },
            { filtAttack, 1500 }, { filtDecay, 2000 }, { filtSustain, 0.5f }, { filtRelease, 1800 },
            { lfo1Wave, 1 }, { lfo1Rate, 0.18f }, { lfo1Dest, 2 }, { lfo1Amount, 0.5f },
            { lfo2Wave, 0 }, { lfo2Rate, 0.12f }, { lfo2Dest, 4 }, { lfo2Amount, 0.4f },
            { chorusOn, 1 }, { chorusDepth, 0.5f }, { chorusMix, 0.4f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 3 }, { delayFbk, 0.5f }, { delayMix, 0.28f },
            { reverbOn, 1 }, { reverbSize, 0.85f }, { reverbMix, 0.45f } } },

        { "Vintage Poly Pad", "Pad", V {
            { voiceMode, 0 }, { masterGain, -8 },
            { osc1Wave, 4 }, { osc1PW, 0.5f }, { osc1Unison, 2 }, { osc1Detune, 12 }, { osc1Level, 0.6f },
            { osc2Wave, 0 }, { osc2Fine, 6 }, { osc2Level, 0.45f },
            { subWave, 0 }, { subLevel, 0.2f },
            { filterType, 0 }, { filterSlope, 0 }, { filterCutoff, 4000 }, { filterReso, 0.2f }, { filterKeytrk, 0.3f }, { filterEnvAmt, 0.25f },
            { ampAttack, 400 }, { ampDecay, 900 }, { ampSustain, 0.8f }, { ampRelease, 900 },
            { filtAttack, 500 }, { filtDecay, 800 }, { filtSustain, 0.5f }, { filtRelease, 800 },
            { lfo1Wave, 0 }, { lfo1Rate, 0.3f }, { lfo1Dest, 2 }, { lfo1Amount, 0.3f },
            { chorusOn, 1 }, { chorusDepth, 0.5f }, { chorusMix, 0.4f },
            { reverbOn, 1 }, { reverbSize, 0.6f }, { reverbMix, 0.3f } } },

        { "Halo Pad", "Pad", V {
            { voiceMode, 0 }, { masterGain, -8 },
            { osc1Wave, 3 }, { osc1Unison, 3 }, { osc1Detune, 10 }, { osc1Spread, 0.7f }, { osc1Level, 0.6f },
            { osc2Wave, 3 }, { osc2Coarse, 12 }, { osc2Fine, 4 }, { osc2Level, 0.4f },
            { filterType, 1 }, { filterSlope, 1 }, { filterCutoff, 400 }, { filterReso, 0.2f }, { filterEnvAmt, 0.3f },
            { ampAttack, 1000 }, { ampDecay, 1500 }, { ampSustain, 0.85f }, { ampRelease, 1800 },
            { filtAttack, 1200 }, { filtDecay, 1500 }, { filtSustain, 0.6f }, { filtRelease, 1500 },
            { lfo1Wave, 0 }, { lfo1Rate, 0.2f }, { lfo1Dest, 4 }, { lfo1Amount, 0.25f },
            { chorusOn, 1 }, { chorusDepth, 0.6f }, { chorusMix, 0.5f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 3 }, { delayFbk, 0.35f }, { delayMix, 0.2f },
            { reverbOn, 1 }, { reverbSize, 0.9f }, { reverbMix, 0.5f } } },

        { "Noise Wash Pad", "Pad", V {
            { voiceMode, 0 }, { masterGain, -9 },
            { osc1Wave, 2 }, { osc1Unison, 3 }, { osc1Detune, 16 }, { osc1Spread, 0.9f }, { osc1Level, 0.5f },
            { osc2Level, 0.0f },
            { noiseLevel, 0.25f }, { noiseColor, 1 },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 3000 }, { filterReso, 0.2f }, { filterEnvAmt, 0.4f },
            { ampAttack, 1500 }, { ampDecay, 2000 }, { ampSustain, 0.7f }, { ampRelease, 2000 },
            { filtAttack, 2000 }, { filtDecay, 2000 }, { filtSustain, 0.5f }, { filtRelease, 2000 },
            { lfo1Wave, 0 }, { lfo1Rate, 0.1f }, { lfo1Dest, 4 }, { lfo1Amount, 0.5f },
            { reverbOn, 1 }, { reverbSize, 0.95f }, { reverbDamp, 0.4f }, { reverbWidth, 1.0f }, { reverbMix, 0.55f } } },

        { "Dream Pad", "Pad", V {
            { voiceMode, 0 }, { masterGain, -8 },
            { osc1Wave, 0 }, { osc1Unison, 5 }, { osc1Detune, 20 }, { osc1Spread, 0.9f }, { osc1Level, 0.6f },
            { osc2Wave, 4 }, { osc2PW, 0.45f }, { osc2Fine, 7 }, { osc2Level, 0.45f },
            { subWave, 0 }, { subLevel, 0.15f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 4200 }, { filterReso, 0.18f }, { filterKeytrk, 0.3f }, { filterEnvAmt, 0.25f },
            { ampAttack, 800 }, { ampDecay, 1500 }, { ampSustain, 0.8f }, { ampRelease, 1600 },
            { filtAttack, 900 }, { filtDecay, 1200 }, { filtSustain, 0.55f }, { filtRelease, 1400 },
            { lfo1Wave, 0 }, { lfo1Rate, 0.28f }, { lfo1Dest, 3 }, { lfo1Amount, 0.3f },
            { chorusOn, 1 }, { chorusDepth, 0.55f }, { chorusMix, 0.45f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 8 }, { delayFbk, 0.4f }, { delayPingpong, 0.6f }, { delayMix, 0.25f },
            { reverbOn, 1 }, { reverbSize, 0.85f }, { reverbMix, 0.45f } } },

        { "Frozen Pad", "Pad", V {
            { voiceMode, 0 }, { masterGain, -9 },
            { osc1Wave, 3 }, { osc1Unison, 4 }, { osc1Detune, 14 }, { osc1Spread, 0.8f }, { osc1Level, 0.55f },
            { osc2Wave, 3 }, { osc2Coarse, 7 }, { osc2Fine, 3 }, { osc2Level, 0.4f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 6000 }, { filterReso, 0.15f }, { filterKeytrk, 0.4f },
            { ampAttack, 1200 }, { ampDecay, 2000 }, { ampSustain, 0.8f }, { ampRelease, 2500 },
            { lfo1Wave, 4 }, { lfo1Sync, 1 }, { lfo1SyncRate, 8 }, { lfo1Dest, 4 }, { lfo1Amount, 0.35f },
            { crushOn, 1 }, { crushDrive, 0.2f }, { crushBits, 12 }, { crushSR, 2 }, { crushShape, 0.2f }, { crushTone, 12000 }, { crushMix, 0.25f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 13 }, { delayFbk, 0.5f }, { delayPingpong, 0.8f }, { delayMix, 0.3f },
            { reverbOn, 1 }, { reverbSize, 0.9f }, { reverbDamp, 0.25f }, { reverbWidth, 1.0f }, { reverbMix, 0.5f } } },

        { "Nebula Pad", "Pad", V {
            { voiceMode, 0 }, { masterGain, -9 },
            { osc1Wave, 0 }, { osc1Unison, 7 }, { osc1Detune, 32 }, { osc1Spread, 1.0f }, { osc1Level, 0.5f },
            { osc2Wave, 2 }, { osc2Coarse, 12 }, { osc2Fine, 5 }, { osc2Level, 0.4f },
            { subWave, 0 }, { subLevel, 0.2f }, { noiseLevel, 0.03f },
            { filterType, 0 }, { filterSlope, 1 }, { filterCutoff, 2800 }, { filterReso, 0.25f }, { filterEnvAmt, 0.35f },
            { ampAttack, 1800 }, { ampDecay, 2500 }, { ampSustain, 0.8f }, { ampRelease, 3000 },
            { filtAttack, 2500 }, { filtDecay, 2500 }, { filtSustain, 0.5f }, { filtRelease, 2500 },
            { lfo1Wave, 0 }, { lfo1Rate, 0.1f }, { lfo1Dest, 4 }, { lfo1Amount, 0.4f },
            { lfo2Wave, 5 }, { lfo2Rate, 0.15f }, { lfo2Dest, 1 }, { lfo2Amount, 0.05f },
            { delayOn, 1 }, { delaySync, 1 }, { delaySyncRate, 2 }, { delayFbk, 0.5f }, { delayMix, 0.3f },
            { reverbOn, 1 }, { reverbSize, 0.95f }, { reverbDamp, 0.3f }, { reverbWidth, 1.0f }, { reverbMix, 0.55f } } },

        { "Solar Wind Pad", "Pad", V {
            { voiceMode, 0 }, { masterGain, -9 },
            { osc1Wave, 0 }, { osc1Unison, 6 }, { osc1Detune, 26 }, { osc1Spread, 1.0f }, { osc1Level, 0.55f },
            { osc2Wave, 4 }, { osc2PW, 0.5f }, { osc2Unison, 3 }, { osc2Detune, 18 }, { osc2Fine, 7 }, { osc2Level, 0.4f },
            { noiseLevel, 0.08f }, { noiseColor, 1 },
            { filterType, 2 }, { filterSlope, 1 }, { filterCutoff, 2500 }, { filterReso, 0.3f }, { filterEnvAmt, 0.3f },
            { ampAttack, 1400 }, { ampDecay, 2000 }, { ampSustain, 0.75f }, { ampRelease, 2200 },
            { filtAttack, 1800 }, { filtDecay, 2000 }, { filtSustain, 0.5f }, { filtRelease, 2000 },
            { lfo1Wave, 0 }, { lfo1Rate, 0.14f }, { lfo1Dest, 4 }, { lfo1Amount, 0.45f },
            { lfo2Wave, 0 }, { lfo2Rate, 0.1f }, { lfo2Dest, 7 }, { lfo2Amount, 0.4f },
            { chorusOn, 1 }, { chorusDepth, 0.5f }, { chorusMix, 0.4f },
            { reverbOn, 1 }, { reverbSize, 0.9f }, { reverbDamp, 0.35f }, { reverbWidth, 1.0f }, { reverbMix, 0.5f } } }
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
