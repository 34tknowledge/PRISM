#include "SynthEngine.h"
#include <algorithm>
#include <limits>
#include <cmath>

namespace prism
{
    void SynthEngine::prepare (double sr, int /*blockSize*/)
    {
        sampleRate = sr;
        for (auto& v : voices) v.setSampleRate (sr);
        for (auto& l : lfo)    l.setSampleRate (sr);

        reset();
    }

    void SynthEngine::reset()
    {
        for (auto& v : voices) v.reset();
        for (auto& l : lfo)    l.reset();
        held.clear();
        bendSemis = 0.0f;
        startCounter = 0;
    }

    void SynthEngine::setEnvParams (const juce::ADSR::Parameters& amp, const juce::ADSR::Parameters& filt)
    {
        for (auto& v : voices) v.setEnvParams (amp, filt);
    }

    void SynthEngine::setModeParams (int m, float glideTimeMs, int gMode)
    {
        mode = m;
        glideMs = glideTimeMs;
        glideMode = gMode;
    }

    void SynthEngine::setLfoParams (int idx, int wave, float rateHz, int dest, float amount)
    {
        idx = juce::jlimit (0, 1, idx);
        lfo[idx].setWave (wave);
        lfo[idx].setRateHz (rateHz);
        lfoDest[idx] = dest;
        lfoAmount[idx] = amount;
    }

    int SynthEngine::findFreeVoice()
    {
        for (int i = 0; i < kNumVoices; ++i)
            if (! voices[i].isActive())
                return i;
        return -1;
    }

    int SynthEngine::findVoiceForNote (int note)
    {
        int best = -1, bestOrder = std::numeric_limits<int>::max();
        for (int i = 0; i < kNumVoices; ++i)
            if (voices[i].isActive() && ! voices[i].isReleasing() && voices[i].getNote() == note)
                if (voiceOrder[i] < bestOrder) { bestOrder = voiceOrder[i]; best = i; }
        return best;
    }

    void SynthEngine::noteOn (int note, float vel)
    {
        const bool overlap = ! held.empty();

        if (mode == Poly)
        {
            int idx = findFreeVoice();
            if (idx < 0)
            {
                // steal a releasing voice first, else the oldest voice
                int oldest = 0, oldestOrder = std::numeric_limits<int>::max();
                int rel = -1, relOrder = std::numeric_limits<int>::max();
                for (int i = 0; i < kNumVoices; ++i)
                {
                    if (voiceOrder[i] < oldestOrder) { oldestOrder = voiceOrder[i]; oldest = i; }
                    if (voices[i].isReleasing() && voiceOrder[i] < relOrder) { relOrder = voiceOrder[i]; rel = i; }
                }
                idx = (rel >= 0) ? rel : oldest;
                voices[idx].hardStop();
            }

            voices[idx].setGlide (glideMode == Always ? glideMs : 0.0f);
            voices[idx].startNote (note, vel, true, lastNote);
            voiceOrder[idx] = ++startCounter;
            lastVoiceIndex = idx;
        }
        else // Mono / Legato — voice 0 is the mono voice
        {
            const bool doGlide = (glideMode == Always) || (glideMode == LegatoOnly && overlap);
            voices[0].setGlide (doGlide ? glideMs : 0.0f);

            if (mode == Legato && overlap)
                voices[0].changePitch (note);                 // glide, no retrigger
            else
                voices[0].startNote (note, vel, true, lastNote);

            voiceOrder[0] = ++startCounter;
            lastVoiceIndex = 0;
        }

        // maintain last-note stack (remove dup, push on top)
        held.erase (std::remove (held.begin(), held.end(), note), held.end());
        held.push_back (note);
        lastNote = (float) note;
    }

    void SynthEngine::noteOff (int note)
    {
        held.erase (std::remove (held.begin(), held.end(), note), held.end());

        if (mode == Poly)
        {
            const int idx = findVoiceForNote (note);
            if (idx >= 0) voices[idx].stopNote();
            return;
        }

        // Mono / Legato
        if (held.empty())
        {
            voices[0].stopNote();
        }
        else
        {
            const int top = held.back();                      // last-note priority
            voices[0].setGlide ((glideMode == Always || glideMode == LegatoOnly) ? glideMs : 0.0f);
            voices[0].changePitch (top);
            lastNote = (float) top;
        }
    }

    void SynthEngine::allNotesOff()
    {
        held.clear();
        for (auto& v : voices) v.stopNote();
    }

    void SynthEngine::handleMidiEvent (const juce::MidiMessage& m)
    {
        if (m.isNoteOn())
            noteOn (m.getNoteNumber(), m.getFloatVelocity());
        else if (m.isNoteOff())
            noteOff (m.getNoteNumber());
        else if (m.isAllNotesOff() || m.isAllSoundOff())
            allNotesOff();
        else if (m.isPitchWheel())
            bendSemis = ((float) m.getPitchWheelValue() - 8192.0f) / 8192.0f * bendRange;
    }

    void SynthEngine::render (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
    {
        const int numSamples = buffer.getNumSamples();
        const int nch = buffer.getNumChannels();
        buffer.clear();

        // push per-block config to all voices
        for (auto& v : voices) v.updateBlock (vp);

        float* out0 = buffer.getWritePointer (0);
        float* out1 = nch > 1 ? buffer.getWritePointer (1) : out0;

        auto midiIt = midi.begin();
        const auto midiEnd = midi.end();

        for (int n = 0; n < numSamples; ++n)
        {
            while (midiIt != midiEnd && (*midiIt).samplePosition <= n)
            {
                handleMidiEvent ((*midiIt).getMessage());
                ++midiIt;
            }

            // --- global modulation from the two LFOs ---
            ModContext mod;
            mod.pitchModSemis = bendSemis;
            float ampMod = 1.0f;
            float panMod = 0.0f;

            for (int li = 0; li < 2; ++li)
            {
                const float v = lfo[li].next();
                const float a = lfoAmount[li];
                switch (lfoDest[li])
                {
                    case 1: mod.pitchModSemis += v * a * 12.0f;          break; // Pitch
                    case 2: mod.pwm1          += v * a * 0.45f;           break; // Osc1 PWM
                    case 3: mod.pwm2          += v * a * 0.45f;           break; // Osc2 PWM
                    case 4: mod.cutoffModOct  += v * a * 4.0f;            break; // Cutoff
                    case 5: mod.resoModAdd    += v * a * 0.5f;            break; // Resonance
                    case 6: ampMod *= 1.0f - a * 0.5f * (1.0f - (v * 0.5f + 0.5f)); break; // Amp (tremolo)
                    case 7: panMod += v * a;                             break; // Pan
                    default: break;                                             // Off
                }
            }

            // --- sum voices ---
            float l = 0.0f, r = 0.0f;
            for (auto& voice : voices)
                if (voice.isActive())
                    voice.renderSample (mod, l, r);

            // --- global amp + pan ---
            panMod = juce::jlimit (-1.0f, 1.0f, panMod);
            const float angle = (panMod + 1.0f) * 0.25f * juce::MathConstants<float>::pi;
            const float gL = std::cos (angle) * juce::MathConstants<float>::sqrt2;
            const float gR = std::sin (angle) * juce::MathConstants<float>::sqrt2;

            out0[n] = l * ampMod * gL;
            if (nch > 1) out1[n] = r * ampMod * gR;
        }

        // drain any remaining events (e.g. beyond last sample)
        while (midiIt != midiEnd) { handleMidiEvent ((*midiIt).getMessage()); ++midiIt; }
    }
}
