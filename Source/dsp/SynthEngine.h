#pragma once

#include "PrismVoice.h"
#include "Lfo.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <array>
#include <vector>

namespace prism
{
    /**
        Custom polyphonic / monophonic voice manager. Owns the voices, the two
        global LFOs and the SUPERNOVA envelope, handles sample-accurate MIDI and
        renders the summed instrument output (pre-FX). Mono/Legato use a
        last-note-priority stack with configurable portamento.
    */
    class SynthEngine
    {
    public:
        enum Mode { Poly = 0, Mono, Legato };
        enum GlideMode { Always = 0, LegatoOnly };

        static constexpr int kNumVoices = 16;

        void prepare (double sr, int blockSize);
        void reset();

        void setVoiceParams (const VoiceParams& p) { vp = p; }
        void setEnvParams (const juce::ADSR::Parameters& amp, const juce::ADSR::Parameters& filt);
        void setModeParams (int mode, float glideTimeMs, int glideMode);
        void setBendRange (float semitones) { bendRange = semitones; }

        void setLfoParams (int idx, int wave, float rateHz, int dest, float amount);

        float getDisplayFilterEnv() const noexcept { return voices[lastVoiceIndex].getFilterEnvValue(); }

        /** Replace `buffer` with the summed voice output; handle MIDI inline. */
        void render (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi);

    private:
        void handleMidiEvent (const juce::MidiMessage& m);
        void noteOn (int note, float vel);
        void noteOff (int note);
        void allNotesOff();

        int  findFreeVoice();     // -1 if none
        int  findVoiceForNote (int note);

        std::array<PrismVoice, kNumVoices> voices;
        int voiceOrder[kNumVoices] { 0 };
        int startCounter = 0;
        int lastVoiceIndex = 0;

        Lfo lfo[2];
        int   lfoDest[2]   { 0, 0 };
        float lfoAmount[2] { 0.0f, 0.0f };

        VoiceParams vp;
        double sampleRate = 44100.0;

        int   mode = Poly;
        float glideMs = 0.0f;
        int   glideMode = LegatoOnly;

        // mono/legato last-note stack
        std::vector<int> held;
        float lastNote = 60.0f;

        // pitch bend
        float bendRange = 2.0f, bendSemis = 0.0f;
    };
}
