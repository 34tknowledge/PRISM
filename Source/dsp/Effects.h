#pragma once

#include "ColoredBitcrusher.h"
#include <juce_dsp/juce_dsp.h>
#include <juce_audio_basics/juce_audio_basics.h>

namespace prism
{
    /** Stereo delay with feedback, tone damping and ping-pong crossfeed. */
    class StereoDelay
    {
    public:
        void prepare (double sr, int blockSize);
        void reset();
        void setParams (float timeMs, float feedback, float pingpong, float toneHz);
        void process (juce::AudioBuffer<float>& buffer, float mix);

    private:
        juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> line { 192000 };
        double sampleRate = 44100.0;
        float delaySamples = 22050.0f, fbk = 0.35f, pp = 0.0f;
        float toneCoeff = 1.0f, lpL = 0.0f, lpR = 0.0f;
    };

    /**
        The full mix-bus FX chain: colored crusher -> chorus -> delay -> reverb.
        `nova` (0..1) drives the SUPERNOVA macro, intensifying every stage and
        blooming the reverb even when a stage is switched off.
    */
    class FxChain
    {
    public:
        struct Params
        {
            bool  crushOn = false;
            float crushDrive = 0.3f, bits = 16.0f, srFactor = 1.0f, shape = 0.3f,
                  tone = 8000.0f, reso = 0.25f, crushMix = 1.0f;

            bool  chorusOn = false;
            float chorusRate = 0.6f, chorusDepth = 0.4f, chorusFbk = 0.2f, chorusMix = 0.4f;

            bool  delayOn = false;
            float delayTimeMs = 300.0f, delayFbk = 0.35f, delayPingpong = 0.0f,
                  delayTone = 6000.0f, delayMix = 0.3f;

            bool  reverbOn = false;
            float reverbSize = 0.5f, reverbDamp = 0.4f, reverbWidth = 1.0f, reverbMix = 0.25f;

            float nova = 0.0f;
        };

        void prepare (double sr, int blockSize, int numChannels);
        void reset();
        void process (juce::AudioBuffer<float>& buffer, const Params& p);

    private:
        ColoredBitcrusher crusher;
        juce::dsp::Chorus<float> chorus;
        StereoDelay delay;
        juce::Reverb reverb;
        double sampleRate = 44100.0;
    };
}
