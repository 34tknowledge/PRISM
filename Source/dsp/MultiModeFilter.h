#pragma once

#include "Svf.h"
#include <cmath>

namespace prism
{
    /**
        Stereo multi-mode filter: LP / HP / BP / Notch at 12 or 24 dB/oct,
        with input drive (soft saturation) and makeup. 24 dB is a cascade of
        two identical TPT SVF stages. Key tracking is applied by the caller by
        scaling the cutoff before setParams().
    */
    class MultiModeFilter
    {
    public:
        enum Type { LP = 0, HP, BP, Notch };

        void setSampleRate (double sr) noexcept
        {
            for (auto* s : { &s1L, &s1R, &s2L, &s2R }) s->setSampleRate (sr);
        }

        void reset() noexcept
        {
            for (auto* s : { &s1L, &s1R, &s2L, &s2R }) s->reset();
        }

        void setParams (int t, bool is24dB, float cutoffHz, float reso, float drive) noexcept
        {
            type    = t;
            slope24 = is24dB;
            driveAmt = drive;

            const float Q = 0.5f + reso * reso * 17.5f;   // 0.5 .. 18 (near self-osc at 1)
            s1L.setCutoffQ (cutoffHz, Q);
            s1R.setCutoffQ (cutoffHz, Q);
            s2L.setCutoffQ (cutoffHz, Q);
            s2R.setCutoffQ (cutoffHz, Q);
        }

        void process (float& l, float& r) noexcept
        {
            if (driveAmt > 0.0f)
            {
                const float d = 1.0f + driveAmt * 6.0f;
                l = std::tanh (l * d);
                r = std::tanh (r * d);
            }

            l = pick (s1L.process (l));
            r = pick (s1R.process (r));

            if (slope24)
            {
                l = pick (s2L.process (l));
                r = pick (s2R.process (r));
            }

            const float makeup = 1.0f / (1.0f + driveAmt * 1.5f);
            l *= makeup;
            r *= makeup;
        }

    private:
        float pick (SvfStage::Outputs o) const noexcept
        {
            switch (type)
            {
                case HP:    return o.hp;
                case BP:    return o.bp;
                case Notch: return o.notch;
                case LP:
                default:    return o.lp;
            }
        }

        SvfStage s1L, s1R, s2L, s2R;
        int type = LP;
        bool slope24 = true;
        float driveAmt = 0.0f;
    };
}
