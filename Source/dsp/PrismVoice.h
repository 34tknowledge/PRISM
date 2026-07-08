#pragma once

#include "Oscillator.h"
#include "MultiModeFilter.h"
#include <juce_audio_basics/juce_audio_basics.h>

namespace prism
{
    /** Per-block snapshot of every voice-level parameter (read-only in render). */
    struct VoiceParams
    {
        // Osc 1
        int   osc1Wave = 0;   float osc1Coarse = 0, osc1Fine = 0, osc1PW = 0.5f, osc1Level = 0.8f;
        int   osc1Uni = 1;    float osc1Detune = 20, osc1Spread = 0.5f;
        // Osc 2
        int   osc2Wave = 1;   float osc2Coarse = 0, osc2Fine = 0, osc2PW = 0.5f, osc2Level = 0.0f;
        int   osc2Uni = 1;    float osc2Detune = 20, osc2Spread = 0.5f; bool osc2Sync = false;
        // Sub / noise
        int   subWave = 0, subOctave = 0; float subLevel = 0.0f;
        float noiseLevel = 0.0f; int noiseColor = 0;
        // Filter
        int   filterType = 0; bool filterSlope24 = true;
        float filterCutoff = 14000, filterReso = 0.15f, filterDrive = 0, filterKeytrack = 0, filterEnvAmt = 0;
        // Pitch
        float bendSemis = 0.0f;
    };

    /** Per-sample global modulation applied to every voice. */
    struct ModContext
    {
        float pitchModSemis = 0.0f;
        float cutoffModOct  = 0.0f;
        float resoModAdd    = 0.0f;
        float pwm1          = 0.0f;
        float pwm2          = 0.0f;
    };

    class PrismVoice
    {
    public:
        void setSampleRate (double sr) noexcept;
        void reset() noexcept;

        void setEnvParams (const juce::ADSR::Parameters& amp,
                           const juce::ADSR::Parameters& filt) noexcept;

        void startNote (int midiNote, float velocity, bool retrigger, float glideFromSemi) noexcept;
        void changePitch (int midiNote) noexcept;      // legato glide, no retrigger
        void stopNote() noexcept;                      // enter release
        void hardStop() noexcept;                      // immediate silence (voice steal)

        void setGlide (float glideTimeMs) noexcept;

        /** Configure per-block osc/filter settings (waves, unison, types). */
        void updateBlock (const VoiceParams& p) noexcept;

        bool isActive() const noexcept { return ampEnv.isActive(); }
        bool isReleasing() const noexcept { return releasing; }
        int  getNote() const noexcept { return midiNote; }
        float getFilterEnvValue() const noexcept { return lastFiltEnv; }

        /** Produce one stereo sample and add it to (l, r). */
        void renderSample (const ModContext& mod, float& l, float& r) noexcept;

    private:
        float nextNoise() noexcept;

        VoiceParams bp;    // current block params

        UnisonOsc osc1, osc2;
        PolyBlepOsc sub;
        MultiModeFilter filter;
        juce::ADSR ampEnv, filtEnv;
        juce::Random rng;

        double sampleRate = 44100.0;
        int   midiNote = -1;
        float velocity = 1.0f;
        float pitchSemi = 60.0f, targetSemi = 60.0f, glideCoeff = 1.0f;
        bool  releasing = false;
        float lastFiltEnv = 0.0f;

        // pink noise state (Paul Kellet economy filter)
        float pink0 = 0, pink1 = 0, pink2 = 0;
    };
}
