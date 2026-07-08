#include "PrismVoice.h"
#include <cmath>

namespace prism
{
    static inline float mtof (float midiNote) noexcept
    {
        return 440.0f * std::pow (2.0f, (midiNote - 69.0f) / 12.0f);
    }

    void PrismVoice::setSampleRate (double sr) noexcept
    {
        sampleRate = sr;
        osc1.setSampleRate (sr);
        osc2.setSampleRate (sr);
        sub.setSampleRate (sr);
        filter.setSampleRate (sr);
        ampEnv.setSampleRate (sr);
        filtEnv.setSampleRate (sr);
    }

    void PrismVoice::reset() noexcept
    {
        filter.reset();
        ampEnv.reset();
        filtEnv.reset();
        midiNote = -1;
        releasing = false;
        pink0 = pink1 = pink2 = 0.0f;
    }

    void PrismVoice::setEnvParams (const juce::ADSR::Parameters& amp,
                                   const juce::ADSR::Parameters& filt) noexcept
    {
        ampEnv.setParameters (amp);
        filtEnv.setParameters (filt);
    }

    void PrismVoice::setGlide (float glideTimeMs) noexcept
    {
        if (glideTimeMs <= 0.0f)
        {
            glideCoeff = 1.0f;
            return;
        }
        const float tau = juce::jmax (1.0f, glideTimeMs * 0.001f * (float) sampleRate);
        glideCoeff = 1.0f - std::exp (-1.0f / tau);
    }

    void PrismVoice::startNote (int note, float vel, bool retrigger, float glideFromSemi) noexcept
    {
        midiNote  = note;
        velocity  = juce::jlimit (0.0f, 1.0f, vel);
        targetSemi = (float) note;
        pitchSemi  = (glideCoeff >= 1.0f) ? (float) note : glideFromSemi;
        releasing  = false;

        if (retrigger)
        {
            osc1.resetPhases (rng);
            osc2.resetPhases (rng);
            sub.resetPhase (rng.nextFloat());
            ampEnv.noteOn();
            filtEnv.noteOn();
        }
    }

    void PrismVoice::changePitch (int note) noexcept
    {
        midiNote   = note;
        targetSemi = (float) note;
    }

    void PrismVoice::stopNote() noexcept
    {
        ampEnv.noteOff();
        filtEnv.noteOff();
        releasing = true;
    }

    void PrismVoice::hardStop() noexcept
    {
        ampEnv.reset();
        filtEnv.reset();
        filter.reset();
        midiNote = -1;
        releasing = false;
    }

    void PrismVoice::updateBlock (const VoiceParams& p) noexcept
    {
        bp = p;

        osc1.setWave (p.osc1Wave);
        osc1.setUnison (p.osc1Uni, p.osc1Detune, p.osc1Spread);
        osc2.setWave (p.osc2Wave);
        osc2.setUnison (p.osc2Uni, p.osc2Detune, p.osc2Spread);
        sub.setWave (p.subWave == 0 ? PolyBlepOsc::Sine
                    : p.subWave == 1 ? PolyBlepOsc::Square : PolyBlepOsc::Triangle);
    }

    float PrismVoice::nextNoise() noexcept
    {
        const float white = rng.nextFloat() * 2.0f - 1.0f;
        if (bp.noiseColor == 0)
            return white;

        // Paul Kellet economy pink filter
        pink0 = 0.99765f * pink0 + white * 0.0990460f;
        pink1 = 0.96300f * pink1 + white * 0.2965164f;
        pink2 = 0.57000f * pink2 + white * 1.0526913f;
        return (pink0 + pink1 + pink2 + white * 0.1848f) * 0.25f;
    }

    void PrismVoice::renderSample (const ModContext& mod, float& outL, float& outR) noexcept
    {
        // --- glide ---
        pitchSemi += (targetSemi - pitchSemi) * glideCoeff;
        const float note = pitchSemi + mod.pitchModSemis + bp.bendSemis;

        // --- oscillator frequencies (cheap; detune multipliers cached) ---
        osc1.setFrequency (mtof (note + bp.osc1Coarse + bp.osc1Fine * 0.01f));
        osc2.setFrequency (mtof (note + bp.osc2Coarse + bp.osc2Fine * 0.01f));
        osc1.setPulseWidth (juce::jlimit (0.05f, 0.95f, bp.osc1PW + mod.pwm1));
        osc2.setPulseWidth (juce::jlimit (0.05f, 0.95f, bp.osc2PW + mod.pwm2));

        const float subSemis = note - 12.0f * (float) (bp.subOctave + 1);
        sub.setFrequency (mtof (subSemis));

        // --- generate ---
        float l1, r1, l2, r2;
        osc1.next (l1, r1);
        if (bp.osc2Sync && osc1.masterWrapped())
            osc2.hardSyncReset();
        osc2.next (l2, r2);

        const float subS   = sub.next() * bp.subLevel;
        const float noiseS = nextNoise() * bp.noiseLevel;
        const float mono   = subS + noiseS;

        float l = l1 * bp.osc1Level + l2 * bp.osc2Level + mono;
        float r = r1 * bp.osc1Level + r2 * bp.osc2Level + mono;

        // --- filter (env-swept per sample) ---
        const float fe = filtEnv.getNextSample();
        lastFiltEnv = fe;

        const float keytrackFactor = std::pow (2.0f, bp.filterKeytrack * (note - 60.0f) / 12.0f);
        const float envFactor      = std::pow (2.0f, bp.filterEnvAmt * fe * 6.0f);
        const float modFactor      = std::pow (2.0f, mod.cutoffModOct);
        const float cutoff = juce::jlimit (20.0f, 20000.0f,
                                           bp.filterCutoff * keytrackFactor * envFactor * modFactor);
        const float reso = juce::jlimit (0.0f, 1.0f, bp.filterReso + mod.resoModAdd);

        filter.setParams (bp.filterType, bp.filterSlope24, cutoff, reso, bp.filterDrive);
        filter.process (l, r);

        // --- amp ---
        const float amp = ampEnv.getNextSample() * (0.3f + 0.7f * velocity);
        outL += l * amp;
        outR += r * amp;
    }
}
