#include "Parameters.h"

namespace prism
{
    using APF   = juce::AudioParameterFloat;
    using APC   = juce::AudioParameterChoice;
    using Range = juce::NormalisableRange<float>;

    static juce::ParameterID pid (const char* s) { return { s, 1 }; }

    // A skewed range whose midpoint sits at `centre` (musical for time / freq).
    static Range skewed (float lo, float hi, float centre, float step = 0.0f)
    {
        Range r { lo, hi, step };
        r.setSkewForCentre (centre);
        return r;
    }

    static Range lin (float lo, float hi, float step = 0.0f)
    {
        return Range { lo, hi, step };
    }

    static auto choice (const char* id, const char* name,
                        juce::StringArray items, int def)
    {
        return std::make_unique<APC> (pid (id), name, items, def);
    }

    // Toggles are 2-option choices ("Off"/"On"): they snap cleanly and so
    // survive exact state round-trip (a raw juce::AudioParameterBool does not,
    // because its getValue() is unsnapped). ButtonAttachment drives them the
    // same way it would a bool (value 0 / 1).
    static auto boolp (const char* id, const char* name, bool def)
    {
        return std::make_unique<APC> (pid (id), name, juce::StringArray { "Off", "On" }, def ? 1 : 0);
    }

    static auto floatp (const char* id, const char* name, Range r, float def,
                        const juce::String& unit = {})
    {
        // Clean readouts: sensible decimals + unit, instead of raw float noise.
        auto stringFn = [unit] (float v, int) -> juce::String
        {
            if (unit == "ms")  return (v < 100.0f ? juce::String (v, 1)
                                                  : juce::String (juce::roundToInt (v))) + " ms";
            if (unit == "Hz")  return v >= 1000.0f ? juce::String (v / 1000.0f, 2) + " kHz"
                                                   : juce::String (juce::roundToInt (v)) + " Hz";
            if (unit == "dB")  return juce::String (v, 1) + " dB";
            if (unit == "st")  return juce::String (juce::roundToInt (v)) + " st";
            if (unit == "ct")  return juce::String (juce::roundToInt (v)) + " ct";
            if (unit == "x")   return juce::String (v, 2) + "x";
            if (unit == "bit") return juce::String (v, 1) + " bit";
            return juce::String (v, 2);
        };
        return std::make_unique<APF> (pid (id), name, r, def,
                                      juce::AudioParameterFloatAttributes()
                                          .withLabel (unit)
                                          .withStringFromValueFunction (std::move (stringFn)));
    }

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        using namespace prism::id;
        juce::AudioProcessorValueTreeState::ParameterLayout layout;

        const juce::StringArray oscWaves { "Saw", "Square", "Triangle", "Sine", "PWM" };
        const juce::StringArray lfoWaves  { "Sine", "Triangle", "Saw", "Square", "S&H", "Random" };

        // ---- Master / voice ----
        layout.add (floatp (masterGain, "Master", lin (-60.0f, 6.0f), -6.0f, "dB"));
        layout.add (choice (voiceMode, "Voice Mode", { "Poly", "Mono", "Legato" }, 0));
        layout.add (floatp (glideTime, "Glide", skewed (0.0f, 2000.0f, 120.0f), 0.0f, "ms"));
        layout.add (choice (glideMode, "Glide Mode", { "Always", "Legato" }, 1));
        layout.add (floatp (bendRange, "Bend Range", lin (0.0f, 24.0f, 1.0f), 2.0f, "st"));

        // ---- Oscillator 1 ----
        layout.add (choice (osc1Wave, "Osc1 Wave", oscWaves, 0));
        layout.add (floatp (osc1Coarse, "Osc1 Coarse", lin (-24.0f, 24.0f, 1.0f), 0.0f, "st"));
        layout.add (floatp (osc1Fine, "Osc1 Fine", lin (-100.0f, 100.0f), 0.0f, "ct"));
        layout.add (floatp (osc1PW, "Osc1 PW", lin (0.05f, 0.95f), 0.5f));
        layout.add (floatp (osc1Level, "Osc1 Level", lin (0.0f, 1.0f), 0.8f));
        layout.add (floatp (osc1Unison, "Osc1 Unison", lin (1.0f, 7.0f, 1.0f), 1.0f));
        layout.add (floatp (osc1Detune, "Osc1 Detune", lin (0.0f, 100.0f), 20.0f, "ct"));
        layout.add (floatp (osc1Spread, "Osc1 Spread", lin (0.0f, 1.0f), 0.5f));

        // ---- Oscillator 2 ----
        layout.add (choice (osc2Wave, "Osc2 Wave", oscWaves, 1));
        layout.add (floatp (osc2Coarse, "Osc2 Coarse", lin (-24.0f, 24.0f, 1.0f), 0.0f, "st"));
        layout.add (floatp (osc2Fine, "Osc2 Fine", lin (-100.0f, 100.0f), 7.0f, "ct"));
        layout.add (floatp (osc2PW, "Osc2 PW", lin (0.05f, 0.95f), 0.5f));
        layout.add (floatp (osc2Level, "Osc2 Level", lin (0.0f, 1.0f), 0.0f));
        layout.add (floatp (osc2Unison, "Osc2 Unison", lin (1.0f, 7.0f, 1.0f), 1.0f));
        layout.add (floatp (osc2Detune, "Osc2 Detune", lin (0.0f, 100.0f), 20.0f, "ct"));
        layout.add (floatp (osc2Spread, "Osc2 Spread", lin (0.0f, 1.0f), 0.5f));
        layout.add (boolp (osc2Sync, "Osc2 Sync", false));

        // ---- Sub / noise ----
        layout.add (choice (subWave, "Sub Wave", { "Sine", "Square", "Triangle" }, 0));
        layout.add (choice (subOctave, "Sub Octave", { "-1", "-2" }, 0));
        layout.add (floatp (subLevel, "Sub Level", lin (0.0f, 1.0f), 0.0f));
        layout.add (floatp (noiseLevel, "Noise Level", lin (0.0f, 1.0f), 0.0f));
        layout.add (choice (noiseColor, "Noise Color", { "White", "Pink" }, 0));

        // ---- Filter ----
        layout.add (choice (filterType, "Filter Type", { "LP", "HP", "BP", "Notch" }, 0));
        layout.add (choice (filterSlope, "Slope", { "12 dB", "24 dB" }, 1));
        layout.add (floatp (filterCutoff, "Cutoff", skewed (20.0f, 20000.0f, 1000.0f), 14000.0f, "Hz"));
        layout.add (floatp (filterReso, "Resonance", lin (0.0f, 1.0f), 0.15f));
        layout.add (floatp (filterDrive, "Drive", lin (0.0f, 1.0f), 0.0f));
        layout.add (floatp (filterKeytrk, "Key Track", lin (0.0f, 1.0f), 0.0f));
        layout.add (floatp (filterEnvAmt, "Env Amount", lin (-1.0f, 1.0f), 0.0f));

        // ---- Amp envelope ----
        layout.add (floatp (ampAttack,  "Amp Attack",  skewed (0.1f, 5000.0f, 40.0f), 2.0f, "ms"));
        layout.add (floatp (ampDecay,   "Amp Decay",   skewed (1.0f, 5000.0f, 300.0f), 400.0f, "ms"));
        layout.add (floatp (ampSustain, "Amp Sustain", lin (0.0f, 1.0f), 0.8f));
        layout.add (floatp (ampRelease, "Amp Release", skewed (1.0f, 8000.0f, 400.0f), 300.0f, "ms"));

        // ---- Filter envelope ----
        layout.add (floatp (filtAttack,  "Filt Attack",  skewed (0.1f, 5000.0f, 40.0f), 5.0f, "ms"));
        layout.add (floatp (filtDecay,   "Filt Decay",   skewed (1.0f, 5000.0f, 300.0f), 300.0f, "ms"));
        layout.add (floatp (filtSustain, "Filt Sustain", lin (0.0f, 1.0f), 0.3f));
        layout.add (floatp (filtRelease, "Filt Release", skewed (1.0f, 8000.0f, 400.0f), 300.0f, "ms"));

        // ---- LFO 1 ----
        layout.add (choice (lfo1Wave, "LFO1 Wave", lfoWaves, 0));
        layout.add (floatp (lfo1Rate, "LFO1 Rate", skewed (0.01f, 40.0f, 2.0f), 2.0f, "Hz"));
        layout.add (boolp (lfo1Sync, "LFO1 Sync", false));
        layout.add (choice (lfo1SyncRate, "LFO1 Div", syncDivisions(), 8));
        layout.add (choice (lfo1Dest, "LFO1 Dest", lfoDestinations(), 0));
        layout.add (floatp (lfo1Amount, "LFO1 Amount", lin (0.0f, 1.0f), 0.5f));

        // ---- LFO 2 ----
        layout.add (choice (lfo2Wave, "LFO2 Wave", lfoWaves, 0));
        layout.add (floatp (lfo2Rate, "LFO2 Rate", skewed (0.01f, 40.0f, 2.0f), 5.0f, "Hz"));
        layout.add (boolp (lfo2Sync, "LFO2 Sync", false));
        layout.add (choice (lfo2SyncRate, "LFO2 Div", syncDivisions(), 11));
        layout.add (choice (lfo2Dest, "LFO2 Dest", lfoDestinations(), 0));
        layout.add (floatp (lfo2Amount, "LFO2 Amount", lin (0.0f, 1.0f), 0.5f));

        // ---- Colored bitcrusher ----
        layout.add (boolp (crushOn, "Crush On", false));
        layout.add (floatp (crushDrive, "Crush Drive", lin (0.0f, 1.0f), 0.3f));
        layout.add (floatp (crushBits, "Crush Bits", lin (1.0f, 16.0f), 16.0f, "bit"));
        layout.add (floatp (crushSR, "Crush SR", skewed (1.0f, 50.0f, 8.0f), 1.0f, "x"));
        layout.add (floatp (crushShape, "Crush Shape", lin (0.0f, 1.0f), 0.3f));
        layout.add (floatp (crushTone, "Crush Tone", skewed (200.0f, 18000.0f, 3000.0f), 8000.0f, "Hz"));
        layout.add (floatp (crushReso, "Crush Reso", lin (0.0f, 1.0f), 0.25f));
        layout.add (floatp (crushMix, "Crush Mix", lin (0.0f, 1.0f), 1.0f));

        // ---- Chorus ----
        layout.add (boolp (chorusOn, "Chorus On", false));
        layout.add (floatp (chorusRate, "Chorus Rate", skewed (0.05f, 8.0f, 1.0f), 0.6f, "Hz"));
        layout.add (floatp (chorusDepth, "Chorus Depth", lin (0.0f, 1.0f), 0.4f));
        layout.add (floatp (chorusFbk, "Chorus Fbk", lin (0.0f, 0.95f), 0.2f));
        layout.add (floatp (chorusMix, "Chorus Mix", lin (0.0f, 1.0f), 0.4f));

        // ---- Delay ----
        layout.add (boolp (delayOn, "Delay On", false));
        layout.add (boolp (delaySync, "Delay Sync", true));
        layout.add (floatp (delayTime, "Delay Time", skewed (1.0f, 2000.0f, 300.0f), 300.0f, "ms"));
        layout.add (choice (delaySyncRate, "Delay Div", syncDivisions(), 11));
        layout.add (floatp (delayFbk, "Delay Fbk", lin (0.0f, 0.98f), 0.35f));
        layout.add (floatp (delayPingpong, "Ping-Pong", lin (0.0f, 1.0f), 0.0f));
        layout.add (floatp (delayTone, "Delay Tone", skewed (500.0f, 18000.0f, 4000.0f), 6000.0f, "Hz"));
        layout.add (floatp (delayMix, "Delay Mix", lin (0.0f, 1.0f), 0.3f));

        // ---- Reverb ----
        layout.add (boolp (reverbOn, "Reverb On", false));
        layout.add (floatp (reverbSize, "Reverb Size", lin (0.0f, 1.0f), 0.5f));
        layout.add (floatp (reverbDamp, "Reverb Damp", lin (0.0f, 1.0f), 0.4f));
        layout.add (floatp (reverbWidth, "Reverb Width", lin (0.0f, 1.0f), 1.0f));
        layout.add (floatp (reverbMix, "Reverb Mix", lin (0.0f, 1.0f), 0.25f));

        return layout;
    }
}
