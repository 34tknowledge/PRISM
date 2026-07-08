# PRISM

**A mono-focused lead & bass synthesizer.**
Plugin developer: **starfinesse** · Built by **34T** for starfinesse.

One beam in, a full spectrum out. PRISM is a colourful, knob-heavy virtual
analogue synth built for fat monophonic leads and basses — every parameter is a
custom-drawn glowing rotary, colour-coded by section, with live displays for the
oscillator waveforms, filter response and envelope shapes.

Formats: **VST3**, **Standalone** (and **AU** when built on macOS).

---

## Sound engine

- **Voice engine** — Poly / Mono / Legato with a Portamento (Glide) control and
  an *Always* vs *Legato-only* glide mode. Last-note priority in mono/legato.
- **2 oscillators** — Saw / Square / Triangle / Sine / PWM, anti-aliased
  (PolyBLEP). Per-osc coarse, fine, pulse-width, level, and **unison up to 7
  voices** with detune + stereo spread for huge stacked tones. Osc 2 can hard-sync
  to Osc 1.
- **Sub oscillator** (−1 / −2 oct, sine/square/triangle) + **noise** (white/pink).
- **Multi-mode filter** — LP / HP / BP / Notch at **12 or 24 dB/oct** (TPT state
  variable), with **drive**, **key tracking** and bipolar envelope amount.
- **2 ADSR envelopes** (amp + filter) with live curve displays.
- **2 LFOs** with tempo sync and a routing destination each (pitch, PWM, cutoff,
  resonance, amp tremolo, pan).

## Effects chain

Colored Crusher → Chorus → Delay → Reverb.

- **Colored Crusher** — the signature character effect. Not a generic clipper: a
  lo-fi AD/DA path modelled after the Decimort/Krush lineage — bit-depth reduction
  with grid-bias + error-feedback noise-shaping, fractional sample-rate reduction
  (sample & hold), an asymmetric waveshaper for even-harmonic warmth (with DC
  blocker), and a **resonant tone filter** that tames the aliasing and voices the
  grit.
- **Chorus**, tempo-syncable **Delay** (feedback, ping-pong, tone damping) and a
  lush **Reverb**.

## ⭐ SUPERNOVA

The big glowing button. Momentary — engaged while held. It collapses the whole
synth into a star going supernova: a downward pitch dive, the colored crusher
slammed to maximum, a feedback delay spun toward runaway shimmer, and the reverb
blooming wide open. Release it and the tail blooms out into space. It reuses the
engine's own effects driven by a dedicated envelope, so it always sits in key with
whatever you're playing. Instant drop / riser for leads and basses.

## Factory presets (16)

| Leads | Basses | Keys / Plucks | Pads |
|---|---|---|---|
| Solar Flare Lead | Reese Monster | Crystal Pluck | Aurora Pad |
| Screamer | Sub Killer | Prism Keys | Warm Analog Pad |
| Glass Slide | Acid 303 | Pluck House | Space Drone |
| 8-Bit Hero | Growl Bass | | |
| Neon Vintage | Grime Wobble | | |

Every preset has real, hand-tuned parameter values. They also show up in your
DAW's program/preset browser.

---

## Building

See **[BUILD.md](BUILD.md)** for full details.

**Easiest (Windows):** run the driver script, which sets up the MSVC environment
and builds Release automatically:

```powershell
powershell -ExecutionPolicy Bypass -File build.ps1
```

**Manual (from an *x64 Native Tools* prompt with CMake + Ninja on PATH):**

```bat
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

JUCE 8 is fetched automatically by CMake — nothing to install by hand.

The built VST3 is copied to your user VST3 folder automatically
(`COPY_PLUGIN_AFTER_BUILD`), and also lands in
`build/Prism_artefacts/Release/VST3/`.

## Licensing note

This builds against **JUCE 8** under the free **Starter** tier, so the
"Made with JUCE" splash stays on (`JUCE_DISPLAY_SPLASH_SCREEN=1`). Turning it off
requires a paid JUCE Indie/Pro seat or shipping under GPLv3 — don't flip it
without one.
