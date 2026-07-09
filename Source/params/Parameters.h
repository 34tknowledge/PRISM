#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

/**
    PRISM — parameter contract.

    Every audio parameter the plugin exposes lives here as a string ID plus a
    factory that builds the APVTS layout. The processor, the editor and the
    preset bank all speak through these IDs, so this file is the single source
    of truth for the plugin's control surface.

    Section colour-coding (used by the UI) mirrors the grouping below:
      OSC (amber) · SUB/NOISE (orange) · FILTER (cyan) · ENV (green)
      LFO (purple) · FX (magenta) · MASTER (white)
*/
namespace prism::id
{
    // ---- Master / voice ---------------------------------------------------
    constexpr auto masterGain   = "masterGain";
    constexpr auto voiceMode     = "voiceMode";    // 0 Poly · 1 Mono · 2 Legato
    constexpr auto glideTime     = "glideTime";    // ms
    constexpr auto glideMode     = "glideMode";    // 0 Always · 1 Legato-only
    constexpr auto bendRange     = "bendRange";    // semitones

    // ---- Oscillator 1 -----------------------------------------------------
    constexpr auto osc1Wave      = "osc1Wave";     // Saw/Square/Tri/Sine/PWM
    constexpr auto osc1Coarse    = "osc1Coarse";   // semitones
    constexpr auto osc1Fine      = "osc1Fine";     // cents
    constexpr auto osc1PW        = "osc1PW";       // pulse width
    constexpr auto osc1Level     = "osc1Level";
    constexpr auto osc1Unison    = "osc1Unison";   // 1..7
    constexpr auto osc1Detune    = "osc1Detune";   // cents
    constexpr auto osc1Spread    = "osc1Spread";   // stereo

    // ---- Oscillator 2 -----------------------------------------------------
    constexpr auto osc2Wave      = "osc2Wave";
    constexpr auto osc2Coarse    = "osc2Coarse";
    constexpr auto osc2Fine      = "osc2Fine";
    constexpr auto osc2PW        = "osc2PW";
    constexpr auto osc2Level     = "osc2Level";
    constexpr auto osc2Unison    = "osc2Unison";
    constexpr auto osc2Detune    = "osc2Detune";
    constexpr auto osc2Spread    = "osc2Spread";
    constexpr auto osc2Sync      = "osc2Sync";     // hard-sync osc2 -> osc1

    // ---- Sub / noise ------------------------------------------------------
    constexpr auto subWave       = "subWave";      // 0 Sine · 1 Square · 2 Tri
    constexpr auto subOctave     = "subOctave";    // 0 -1oct · 1 -2oct
    constexpr auto subLevel      = "subLevel";
    constexpr auto noiseLevel    = "noiseLevel";
    constexpr auto noiseColor    = "noiseColor";   // 0 White · 1 Pink

    // ---- Filter -----------------------------------------------------------
    constexpr auto filterType    = "filterType";   // LP/HP/BP/Notch
    constexpr auto filterSlope   = "filterSlope";  // 0 12dB · 1 24dB
    constexpr auto filterCutoff  = "filterCutoff"; // Hz
    constexpr auto filterReso    = "filterReso";
    constexpr auto filterDrive   = "filterDrive";
    constexpr auto filterKeytrk  = "filterKeytrk"; // key tracking 0..1
    constexpr auto filterEnvAmt  = "filterEnvAmt"; // bipolar env -> cutoff

    // ---- Amp envelope (ADSR 1) -------------------------------------------
    constexpr auto ampAttack     = "ampAttack";
    constexpr auto ampDecay      = "ampDecay";
    constexpr auto ampSustain    = "ampSustain";
    constexpr auto ampRelease    = "ampRelease";

    // ---- Filter envelope (ADSR 2) ----------------------------------------
    constexpr auto filtAttack    = "filtAttack";
    constexpr auto filtDecay     = "filtDecay";
    constexpr auto filtSustain   = "filtSustain";
    constexpr auto filtRelease   = "filtRelease";

    // ---- LFO 1 ------------------------------------------------------------
    constexpr auto lfo1Wave      = "lfo1Wave";     // Sine/Tri/Saw/Square/S&H/Random
    constexpr auto lfo1Rate      = "lfo1Rate";     // Hz (free)
    constexpr auto lfo1Sync      = "lfo1Sync";     // tempo sync on/off
    constexpr auto lfo1SyncRate  = "lfo1SyncRate"; // note division index
    constexpr auto lfo1Dest      = "lfo1Dest";
    constexpr auto lfo1Amount    = "lfo1Amount";

    // ---- LFO 2 ------------------------------------------------------------
    constexpr auto lfo2Wave      = "lfo2Wave";
    constexpr auto lfo2Rate      = "lfo2Rate";
    constexpr auto lfo2Sync      = "lfo2Sync";
    constexpr auto lfo2SyncRate  = "lfo2SyncRate";
    constexpr auto lfo2Dest      = "lfo2Dest";
    constexpr auto lfo2Amount    = "lfo2Amount";

    // ---- Colored bitcrusher (signature FX) -------------------------------
    constexpr auto crushOn       = "crushOn";
    constexpr auto crushDrive    = "crushDrive";
    constexpr auto crushBits     = "crushBits";    // 1..16 (fractional)
    constexpr auto crushSR       = "crushSR";      // downsample factor 1..50
    constexpr auto crushShape    = "crushShape";   // asymmetric waveshape
    constexpr auto crushTone     = "crushTone";    // resonant tone cutoff Hz
    constexpr auto crushReso     = "crushReso";    // resonant tone Q
    constexpr auto crushMix      = "crushMix";

    // ---- Chorus -----------------------------------------------------------
    constexpr auto chorusOn      = "chorusOn";
    constexpr auto chorusRate    = "chorusRate";
    constexpr auto chorusDepth   = "chorusDepth";
    constexpr auto chorusFbk     = "chorusFbk";
    constexpr auto chorusMix     = "chorusMix";

    // ---- Delay ------------------------------------------------------------
    constexpr auto delayOn       = "delayOn";
    constexpr auto delaySync     = "delaySync";
    constexpr auto delayTime     = "delayTime";    // ms (free)
    constexpr auto delaySyncRate = "delaySyncRate";// note division
    constexpr auto delayFbk      = "delayFbk";
    constexpr auto delayPingpong = "delayPingpong";
    constexpr auto delayTone     = "delayTone";    // damping
    constexpr auto delayMix      = "delayMix";

    // ---- Reverb -----------------------------------------------------------
    constexpr auto reverbOn      = "reverbOn";
    constexpr auto reverbSize    = "reverbSize";
    constexpr auto reverbDamp    = "reverbDamp";
    constexpr auto reverbWidth   = "reverbWidth";
    constexpr auto reverbMix     = "reverbMix";
}

namespace prism
{
    /** Note divisions shared by tempo-synced LFOs and the delay. */
    inline juce::StringArray syncDivisions()
    {
        return { "8/1", "4/1", "2/1", "1/1", "1/2.", "1/2", "1/2T",
                 "1/4.", "1/4", "1/4T", "1/8.", "1/8", "1/8T",
                 "1/16.", "1/16", "1/16T", "1/32" };
    }

    /** Multiplier applied to one beat (a quarter note) for each division above. */
    inline double syncDivisionInBeats (int index)
    {
        // beats = fraction-of-whole-note * 4, dotted = *1.5, triplet = *2/3
        static const double beats[] = {
            32.0, 16.0, 8.0, 4.0,        // 8/1 4/1 2/1 1/1
            3.0, 2.0, 4.0 / 3.0,          // 1/2. 1/2 1/2T
            1.5, 1.0, 2.0 / 3.0,          // 1/4. 1/4 1/4T
            0.75, 0.5, 1.0 / 3.0,         // 1/8. 1/8 1/8T
            0.375, 0.25, 1.0 / 6.0,       // 1/16. 1/16 1/16T
            0.125                          // 1/32
        };
        const int n = (int) (sizeof (beats) / sizeof (double));
        return beats[juce::jlimit (0, n - 1, index)];
    }

    inline juce::StringArray lfoDestinations()
    {
        return { "Off", "Pitch", "Osc1 PWM", "Osc2 PWM", "Cutoff",
                 "Resonance", "Amp", "Pan" };
    }

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
}
