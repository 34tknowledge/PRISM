#include "Effects.h"

namespace prism
{
    static inline float lerp (float a, float b, float t) noexcept { return a + (b - a) * t; }

    // ---------------------------------------------------------------- StereoDelay
    void StereoDelay::prepare (double sr, int blockSize)
    {
        sampleRate = sr;
        juce::dsp::ProcessSpec spec { sr, (juce::uint32) blockSize, 2 };
        line.prepare (spec);
        reset();
    }

    void StereoDelay::reset()
    {
        line.reset();
        lpL = lpR = 0.0f;
    }

    void StereoDelay::setParams (float timeMs, float feedback, float pingpong, float toneHz)
    {
        delaySamples = juce::jlimit (1.0f, 191000.0f, timeMs * 0.001f * (float) sampleRate);
        fbk = juce::jlimit (0.0f, 0.98f, feedback);
        pp  = juce::jlimit (0.0f, 1.0f, pingpong);
        toneCoeff = 1.0f - std::exp (-juce::MathConstants<float>::twoPi * toneHz / (float) sampleRate);
    }

    void StereoDelay::process (juce::AudioBuffer<float>& buffer, float mix)
    {
        const int nch = buffer.getNumChannels();
        const int n   = buffer.getNumSamples();
        float* L = buffer.getWritePointer (0);
        float* R = nch > 1 ? buffer.getWritePointer (1) : L;

        for (int i = 0; i < n; ++i)
        {
            const float inL = L[i], inR = R[i];

            float pL = line.popSample (0, delaySamples, true);
            float pR = line.popSample (1, delaySamples, true);

            // tone damping (one-pole LP) in the wet/feedback path
            lpL += toneCoeff * (pL - lpL);
            lpR += toneCoeff * (pR - lpR);
            const float dampedL = lpL, dampedR = lpR;

            const float pushL = inL + (dampedL * (1.0f - pp) + dampedR * pp) * fbk;
            const float pushR = inR + (dampedR * (1.0f - pp) + dampedL * pp) * fbk;
            line.pushSample (0, pushL);
            line.pushSample (1, pushR);

            L[i] = inL + dampedL * mix;
            if (nch > 1) R[i] = inR + dampedR * mix;
        }
    }

    // ------------------------------------------------------------------- FxChain
    void FxChain::prepare (double sr, int blockSize, int numChannels)
    {
        sampleRate = sr;
        juce::dsp::ProcessSpec spec { sr, (juce::uint32) blockSize,
                                      (juce::uint32) juce::jmax (1, numChannels) };
        crusher.setSampleRate (sr);
        chorus.prepare (spec);
        chorus.setCentreDelay (7.0f);
        delay.prepare (sr, blockSize);
        reverb.setSampleRate (sr);
        reset();
    }

    void FxChain::reset()
    {
        crusher.reset();
        chorus.reset();
        delay.reset();
        reverb.reset();
    }

    void FxChain::process (juce::AudioBuffer<float>& buffer, const Params& p)
    {
        const int nch = buffer.getNumChannels();
        const int n   = buffer.getNumSamples();
        float* L = buffer.getWritePointer (0);
        float* R = nch > 1 ? buffer.getWritePointer (1) : L;

        const float nova = juce::jlimit (0.0f, 1.0f, p.nova);

        // 1. colored crusher (also engaged by SUPERNOVA)
        if (p.crushOn || nova > 0.001f)
        {
            const float baseDrive = p.crushOn ? p.crushDrive : 0.0f;
            const float baseBits  = p.crushOn ? p.bits       : 16.0f;
            const float baseSR    = p.crushOn ? p.srFactor   : 1.0f;
            const float baseShape = p.crushOn ? p.shape      : 0.3f;
            const float baseTone  = p.crushOn ? p.tone       : 8000.0f;
            const float baseReso  = p.crushOn ? p.reso       : 0.25f;
            const float baseMix   = p.crushOn ? p.crushMix   : 0.0f;

            crusher.setParams (lerp (baseDrive, 0.85f, nova),
                               lerp (baseBits,  3.0f,  nova),
                               lerp (baseSR,    22.0f, nova),
                               lerp (baseShape, 0.9f,  nova),
                               lerp (baseTone,  3500.0f, nova),
                               lerp (baseReso,  0.7f,  nova),
                               juce::jmax (baseMix, nova));

            for (int i = 0; i < n; ++i)
            {
                float l = L[i], r = R[i];
                crusher.process (l, r);
                L[i] = l;
                if (nch > 1) R[i] = r;
            }
        }

        // 2. chorus
        if (p.chorusOn)
        {
            chorus.setRate (p.chorusRate);
            chorus.setDepth (p.chorusDepth);
            chorus.setFeedback (juce::jlimit (-0.95f, 0.95f, p.chorusFbk));
            chorus.setMix (p.chorusMix);
            juce::dsp::AudioBlock<float> block (buffer);
            juce::dsp::ProcessContextReplacing<float> ctx (block);
            chorus.process (ctx);
        }

        // 3. delay (also engaged by SUPERNOVA)
        if (p.delayOn || nova > 0.001f)
        {
            const float fbk = juce::jmax (p.delayOn ? p.delayFbk : 0.0f, nova * 0.85f);
            const float mix = juce::jmax (p.delayOn ? p.delayMix : 0.0f, nova * 0.6f);
            const float pp  = juce::jmax (p.delayOn ? p.delayPingpong : 0.0f, nova);
            delay.setParams (p.delayTimeMs, fbk, pp, p.delayTone);
            delay.process (buffer, mix);
        }

        // 4. reverb (blooms wide open under SUPERNOVA)
        if (p.reverbOn || nova > 0.001f)
        {
            juce::Reverb::Parameters rp;
            rp.roomSize = juce::jlimit (0.0f, 1.0f, juce::jmax (p.reverbOn ? p.reverbSize : 0.5f, nova));
            rp.damping  = juce::jlimit (0.0f, 1.0f, p.reverbDamp);
            rp.width    = juce::jlimit (0.0f, 1.0f, p.reverbWidth);
            const float mix = juce::jmax (p.reverbOn ? p.reverbMix : 0.0f, nova * 0.85f);
            rp.wetLevel = mix;
            rp.dryLevel = 1.0f - mix;
            rp.freezeMode = 0.0f;
            reverb.setParameters (rp);
            reverb.processStereo (L, R, n);
        }
    }
}
