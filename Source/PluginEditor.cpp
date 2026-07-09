#include "PluginEditor.h"
#include "params/Parameters.h"
#include "params/Presets.h"
#include "BinaryData.h"

using namespace prism;
using namespace prism::id;

PrismAudioProcessorEditor::PrismAudioProcessorEditor (PrismAudioProcessor& p)
    : juce::AudioProcessorEditor (&p), proc (p)
{
    setLookAndFeel (&lnf);

    buildSections();

    // preset transport
    auto styleTransport = [] (juce::TextButton& b)
    {
        b.setColour (juce::TextButton::buttonColourId, theme::track);
        b.setColour (juce::TextButton::textColourOffId, theme::text);
        b.setColour (juce::ComboBox::outlineColourId, theme::stroke);
    };
    presetPrev.setButtonText ("<");  styleTransport (presetPrev);  addAndMakeVisible (presetPrev);
    presetNext.setButtonText (">");  styleTransport (presetNext);  addAndMakeVisible (presetNext);
    styleTransport (presetName);     addAndMakeVisible (presetName);
    presetPrev.onClick = [this] { selectProgram (proc.getCurrentProgram() - 1); };
    presetNext.onClick = [this] { selectProgram (proc.getCurrentProgram() + 1); };
    presetName.onClick = [this] { showPresetMenu(); };

    // logo (embedded PNG, black keyed to transparency)
    logoImage = juce::ImageCache::getFromMemory (BinaryData::sf_logo_png, BinaryData::sf_logo_pngSize);

    // top-bar controls
    voiceModeCombo = makeCombo (voiceMode, theme::master);  addAndMakeVisible (*voiceModeCombo);
    glideModeCombo = makeCombo (glideMode, theme::master);  addAndMakeVisible (*glideModeCombo);
    masterKnob = makeKnob (masterGain, "Master", theme::master); addAndMakeVisible (*masterKnob);
    glideKnob  = makeKnob (glideTime,  "Glide",  theme::master); addAndMakeVisible (*glideKnob);
    bendKnob   = makeKnob (bendRange,  "Bend",   theme::master); addAndMakeVisible (*bendKnob);

    refreshPresetName();

    setSize (1120, 720);
    setResizable (true, true);
    setResizeLimits (980, 640, 1680, 1080);
    startTimerHz (30);
}

PrismAudioProcessorEditor::~PrismAudioProcessorEditor()
{
    stopTimer();
    setLookAndFeel (nullptr);
}

prism::GlowKnob* PrismAudioProcessorEditor::makeKnob (const char* paramID, const juce::String& caption, juce::Colour accent)
{
    auto* k = new GlowKnob (caption, accent);
    knobs.add (k);
    sliderAtts.add (new SliderAtt (proc.getAPVTS(), paramID, k->slider));
    return k;
}

juce::ComboBox* PrismAudioProcessorEditor::makeCombo (const char* paramID, juce::Colour accent)
{
    auto* c = new juce::ComboBox();
    combos.add (c);
    if (auto* choice = dynamic_cast<juce::AudioParameterChoice*> (proc.getAPVTS().getParameter (paramID)))
        c->addItemList (choice->choices, 1);
    c->setColour (juce::ComboBox::outlineColourId, accent);
    c->setColour (juce::ComboBox::textColourId, theme::text);
    comboAtts.add (new ComboAtt (proc.getAPVTS(), paramID, *c));
    return c;
}

juce::ToggleButton* PrismAudioProcessorEditor::makeToggle (const char* paramID, const juce::String& text, juce::Colour accent)
{
    auto* t = new juce::ToggleButton (text);
    toggles.add (t);
    t->setColour (juce::ToggleButton::tickColourId, accent);
    buttonAtts.add (new ButtonAtt (proc.getAPVTS(), paramID, *t));
    return t;
}

void PrismAudioProcessorEditor::buildSections()
{
    // ---- OSC 1 ----
    oscP1 = std::make_unique<SectionPanel> ("Oscillator 1", theme::osc);
    oscP1->setColumns (4);
    oscP1->addHeader (makeCombo (id::osc1Wave, theme::osc));
    oscP1->setDisplay (&osc1Disp, 62);
    oscP1->addKnob (makeKnob (osc1Coarse, "Coarse", theme::osc));
    oscP1->addKnob (makeKnob (osc1Fine,   "Fine",   theme::osc));
    oscP1->addKnob (makeKnob (osc1PW,     "PW",     theme::osc));
    oscP1->addKnob (makeKnob (osc1Level,  "Level",  theme::osc));
    oscP1->addKnob (makeKnob (osc1Unison, "Unison", theme::osc));
    oscP1->addKnob (makeKnob (osc1Detune, "Detune", theme::osc));
    oscP1->addKnob (makeKnob (osc1Spread, "Spread", theme::osc));
    addAndMakeVisible (*oscP1);

    // ---- OSC 2 ----
    oscP2 = std::make_unique<SectionPanel> ("Oscillator 2", theme::osc);
    oscP2->setColumns (4);
    oscP2->addHeader (makeCombo (id::osc2Wave, theme::osc));
    oscP2->addHeader (makeToggle (id::osc2Sync, "SYNC", theme::osc));
    oscP2->setDisplay (&osc2Disp, 62);
    oscP2->addKnob (makeKnob (osc2Coarse, "Coarse", theme::osc));
    oscP2->addKnob (makeKnob (osc2Fine,   "Fine",   theme::osc));
    oscP2->addKnob (makeKnob (osc2PW,     "PW",     theme::osc));
    oscP2->addKnob (makeKnob (osc2Level,  "Level",  theme::osc));
    oscP2->addKnob (makeKnob (osc2Unison, "Unison", theme::osc));
    oscP2->addKnob (makeKnob (osc2Detune, "Detune", theme::osc));
    oscP2->addKnob (makeKnob (osc2Spread, "Spread", theme::osc));
    addAndMakeVisible (*oscP2);

    // ---- SUB / NOISE ----
    subP = std::make_unique<SectionPanel> ("Sub / Noise", theme::sub);
    subP->setColumns (3);
    subP->addHeader (makeCombo (subWave, theme::sub));
    subP->addHeader (makeCombo (subOctave, theme::sub));
    subP->addHeader (makeCombo (noiseColor, theme::sub));
    subP->addKnob (makeKnob (subLevel,   "Sub",   theme::sub));
    subP->addKnob (makeKnob (noiseLevel, "Noise", theme::sub));
    addAndMakeVisible (*subP);

    // ---- FILTER ----
    filterP = std::make_unique<SectionPanel> ("Filter", theme::filter);
    filterP->setColumns (3);
    filterP->addHeader (makeCombo (filterType,  theme::filter));
    filterP->addHeader (makeCombo (filterSlope, theme::filter));
    filterP->setDisplay (&filterCurve, 90);
    filterP->addKnob (makeKnob (filterCutoff, "Cutoff",   theme::filter));
    filterP->addKnob (makeKnob (filterReso,   "Reso",     theme::filter));
    filterP->addKnob (makeKnob (filterDrive,  "Drive",    theme::filter));
    filterP->addKnob (makeKnob (filterKeytrk, "Key Trk",  theme::filter));
    filterP->addKnob (makeKnob (filterEnvAmt, "Env Amt",  theme::filter));
    addAndMakeVisible (*filterP);

    // ---- AMP ENV ----
    ampP = std::make_unique<SectionPanel> ("Amp Envelope", theme::ampEnv);
    ampP->setColumns (4);
    ampP->setDisplay (&ampEnvDisp, 76);
    ampP->addKnob (makeKnob (ampAttack,  "Attack",  theme::ampEnv));
    ampP->addKnob (makeKnob (ampDecay,   "Decay",   theme::ampEnv));
    ampP->addKnob (makeKnob (ampSustain, "Sustain", theme::ampEnv));
    ampP->addKnob (makeKnob (ampRelease, "Release", theme::ampEnv));
    addAndMakeVisible (*ampP);

    // ---- FILTER ENV ----
    filtP = std::make_unique<SectionPanel> ("Filter Envelope", theme::filtEnv);
    filtP->setColumns (4);
    filtP->setDisplay (&filtEnvDisp, 76);
    filtP->addKnob (makeKnob (filtAttack,  "Attack",  theme::filtEnv));
    filtP->addKnob (makeKnob (filtDecay,   "Decay",   theme::filtEnv));
    filtP->addKnob (makeKnob (filtSustain, "Sustain", theme::filtEnv));
    filtP->addKnob (makeKnob (filtRelease, "Release", theme::filtEnv));
    addAndMakeVisible (*filtP);

    // ---- LFO 1 ----
    lfo1P = std::make_unique<SectionPanel> ("LFO 1", theme::lfo);
    lfo1P->setColumns (2);
    lfo1P->addHeader (makeCombo (lfo1Wave, theme::lfo));
    lfo1P->addHeader (makeToggle (lfo1Sync, "SYNC", theme::lfo));
    lfo1P->addHeader (makeCombo (lfo1SyncRate, theme::lfo));
    lfo1P->addHeader (makeCombo (lfo1Dest, theme::lfo));
    lfo1P->addKnob (makeKnob (lfo1Rate,   "Rate",   theme::lfo));
    lfo1P->addKnob (makeKnob (lfo1Amount, "Amount", theme::lfo));
    addAndMakeVisible (*lfo1P);

    // ---- LFO 2 ----
    lfo2P = std::make_unique<SectionPanel> ("LFO 2", theme::lfo);
    lfo2P->setColumns (2);
    lfo2P->addHeader (makeCombo (lfo2Wave, theme::lfo));
    lfo2P->addHeader (makeToggle (lfo2Sync, "SYNC", theme::lfo));
    lfo2P->addHeader (makeCombo (lfo2SyncRate, theme::lfo));
    lfo2P->addHeader (makeCombo (lfo2Dest, theme::lfo));
    lfo2P->addKnob (makeKnob (lfo2Rate,   "Rate",   theme::lfo));
    lfo2P->addKnob (makeKnob (lfo2Amount, "Amount", theme::lfo));
    addAndMakeVisible (*lfo2P);

    // ---- CRUSHER ----
    crushP = std::make_unique<SectionPanel> ("Colored Crusher", theme::fx);
    crushP->setColumns (4);
    crushP->addHeader (makeToggle (crushOn, "ON", theme::fx));
    crushP->addKnob (makeKnob (crushDrive, "Drive", theme::fx));
    crushP->addKnob (makeKnob (crushBits,  "Bits",  theme::fx));
    crushP->addKnob (makeKnob (crushSR,    "SR",    theme::fx));
    crushP->addKnob (makeKnob (crushShape, "Shape", theme::fx));
    crushP->addKnob (makeKnob (crushTone,  "Tone",  theme::fx));
    crushP->addKnob (makeKnob (crushReso,  "Reso",  theme::fx));
    crushP->addKnob (makeKnob (crushMix,   "Mix",   theme::fx));
    addAndMakeVisible (*crushP);

    // ---- CHORUS ----
    chorusP = std::make_unique<SectionPanel> ("Chorus", theme::fx);
    chorusP->setColumns (4);
    chorusP->addHeader (makeToggle (chorusOn, "ON", theme::fx));
    chorusP->addKnob (makeKnob (chorusRate,  "Rate",  theme::fx));
    chorusP->addKnob (makeKnob (chorusDepth, "Depth", theme::fx));
    chorusP->addKnob (makeKnob (chorusFbk,   "Fbk",   theme::fx));
    chorusP->addKnob (makeKnob (chorusMix,   "Mix",   theme::fx));
    addAndMakeVisible (*chorusP);

    // ---- DELAY ----
    delayP = std::make_unique<SectionPanel> ("Delay", theme::fx);
    delayP->setColumns (3);
    delayP->addHeader (makeToggle (delayOn, "ON", theme::fx));
    delayP->addHeader (makeToggle (delaySync, "SYNC", theme::fx));
    delayP->addHeader (makeCombo (delaySyncRate, theme::fx));
    delayP->addKnob (makeKnob (delayTime,     "Time",  theme::fx));
    delayP->addKnob (makeKnob (delayFbk,      "Fbk",   theme::fx));
    delayP->addKnob (makeKnob (delayPingpong, "Ping",  theme::fx));
    delayP->addKnob (makeKnob (delayTone,     "Tone",  theme::fx));
    delayP->addKnob (makeKnob (delayMix,      "Mix",   theme::fx));
    addAndMakeVisible (*delayP);

    // ---- REVERB ----
    reverbP = std::make_unique<SectionPanel> ("Reverb", theme::fx);
    reverbP->setColumns (4);
    reverbP->addHeader (makeToggle (reverbOn, "ON", theme::fx));
    reverbP->addKnob (makeKnob (reverbSize,  "Size",  theme::fx));
    reverbP->addKnob (makeKnob (reverbDamp,  "Damp",  theme::fx));
    reverbP->addKnob (makeKnob (reverbWidth, "Width", theme::fx));
    reverbP->addKnob (makeKnob (reverbMix,   "Mix",   theme::fx));
    addAndMakeVisible (*reverbP);
}

void PrismAudioProcessorEditor::selectProgram (int index)
{
    const int n = proc.getNumPrograms();
    index = juce::jlimit (0, n - 1, index);
    proc.setCurrentProgram (index);
    refreshPresetName();
}

void PrismAudioProcessorEditor::refreshPresetName()
{
    const int cur = proc.getCurrentProgram();
    presetName.setButtonText (proc.getProgramName (cur));
    lastProgram = cur;
}

void PrismAudioProcessorEditor::showPresetMenu()
{
    const auto& presets = getFactoryPresets();
    const juce::StringArray categories { "Lead", "Bass", "Keys", "Pluck", "Pad" };

    juce::PopupMenu menu;
    for (const auto& cat : categories)
    {
        juce::PopupMenu sub;
        bool any = false;
        for (int i = 0; i < (int) presets.size(); ++i)
            if (presets[(size_t) i].category == cat)
            {
                sub.addItem (i + 1, presets[(size_t) i].name, true, i == proc.getCurrentProgram());
                any = true;
            }
        if (any) menu.addSubMenu (cat, sub);
    }
    menu.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (presetName),
                        [this] (int result) { if (result > 0) selectProgram (result - 1); });
}

void PrismAudioProcessorEditor::timerCallback()
{
    auto pv = [this] (const char* pid) { return proc.getAPVTS().getRawParameterValue (pid)->load(); };

    osc1Disp.setWave ((int) pv (id::osc1Wave), pv (id::osc1PW));
    osc2Disp.setWave ((int) pv (id::osc2Wave), pv (id::osc2PW));
    filterCurve.setFilter ((int) pv (id::filterType), pv (id::filterCutoff),
                           pv (id::filterReso), pv (id::filterSlope) > 0.5f);
    ampEnvDisp.setADSR (pv (id::ampAttack), pv (id::ampDecay), pv (id::ampSustain), pv (id::ampRelease));
    filtEnvDisp.setADSR (pv (id::filtAttack), pv (id::filtDecay), pv (id::filtSustain), pv (id::filtRelease));

    if (proc.getCurrentProgram() != lastProgram)
        refreshPresetName();
}

void PrismAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::ColourGradient bg (theme::bg0, 0.0f, 0.0f, theme::bg1, 0.0f, (float) getHeight(), false);
    g.setGradientFill (bg);
    g.fillAll();

    // top bar
    auto bar = juce::Rectangle<int> (8, 8, getWidth() - 16, 56).toFloat();
    juce::ColourGradient bgrad (theme::panelHi, 0.0f, 8.0f, theme::panel, 0.0f, 64.0f, false);
    g.setGradientFill (bgrad);
    g.fillRoundedRectangle (bar, 8.0f);
    g.setColour (theme::stroke);
    g.drawRoundedRectangle (bar.reduced (0.5f), 8.0f, 1.0f);

    // logo + wordmark
    if (logoImage.isValid())
        g.drawImage (logoImage, juce::Rectangle<float> (14.0f, 9.0f, 50.0f, 50.0f),
                     juce::RectanglePlacement::centred);

    g.setColour (theme::text);
    g.setFont (juce::Font (juce::FontOptions().withHeight (25.0f).withStyle ("Bold")));
    g.drawText ("STAR", 74, 14, 90, 34, juce::Justification::centredLeft);
    g.setColour (theme::accent);
    g.drawText ("FX", 138, 14, 80, 34, juce::Justification::centredLeft);
}

void PrismAudioProcessorEditor::resized()
{
    const int m = 8, gap = 8;
    const int W = getWidth(), H = getHeight();

    // ---- top bar ----
    auto bar = juce::Rectangle<int> (m, m, W - 2 * m, 56);
    {
        auto b = bar.reduced (6, 6);
        b.removeFromLeft (230);                       // wordmark + icon space

        // preset transport (left of centre)
        auto preset = b.removeFromLeft (260);
        presetPrev.setBounds (preset.removeFromLeft (30).reduced (2));
        presetNext.setBounds (preset.removeFromRight (30).reduced (2));
        presetName.setBounds (preset.reduced (4, 2));

        // right cluster: master/glide/bend + mode combos
        auto right = b;
        masterKnob->setBounds (right.removeFromRight (62));
        glideKnob->setBounds  (right.removeFromRight (58));
        bendKnob->setBounds   (right.removeFromRight (54));
        right.removeFromRight (gap);
        auto modes = right.removeFromRight (100);
        voiceModeCombo->setBounds (modes.removeFromTop (modes.getHeight() / 2).reduced (0, 1));
        glideModeCombo->setBounds (modes.reduced (0, 1));
    }

    // ---- main grid ----
    const int top = 72;
    const int colW = (W - 2 * m - 3 * gap) / 4;
    const int x0 = m, x1 = x0 + colW + gap, x2 = x1 + colW + gap, x3 = x2 + colW + gap;
    const int bottom = H - m;
    const int colH = bottom - top;

    // Col 0 — oscillators
    const int oscH = (colH - gap) / 2;
    oscP1->setBounds (x0, top, colW, oscH);
    oscP2->setBounds (x0, top + oscH + gap, colW, oscH);

    // Col 1 — sub + filter
    const int subH = 168;
    subP->setBounds (x1, top, colW, subH);
    filterP->setBounds (x1, top + subH + gap, colW, colH - subH - gap);

    // Col 2 — envelopes + LFOs
    const int envH = 166;
    ampP->setBounds  (x2, top, colW, envH);
    filtP->setBounds (x2, top + envH + gap, colW, envH);
    const int lfoTop = top + 2 * (envH + gap);
    const int lfoH = (bottom - lfoTop - gap) / 2;
    lfo1P->setBounds (x2, lfoTop, colW, lfoH);
    lfo2P->setBounds (x2, lfoTop + lfoH + gap, colW, lfoH);

    // Col 3 — FX chain
    int y = top;
    const int crushH = 206, chorusH = 118, delayH = 158;
    crushP->setBounds  (x3, y, colW, crushH); y += crushH + gap;
    chorusP->setBounds (x3, y, colW, chorusH); y += chorusH + gap;
    delayP->setBounds  (x3, y, colW, delayH); y += delayH + gap;
    reverbP->setBounds (x3, y, colW, bottom - y);
}
