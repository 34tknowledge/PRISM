#pragma once

#include "PluginProcessor.h"
#include "ui/PrismLookAndFeel.h"
#include "ui/GlowKnob.h"
#include "ui/SectionPanel.h"
#include "ui/Displays.h"
#include "ui/SupernovaButton.h"
#include <memory>

class PrismAudioProcessorEditor : public juce::AudioProcessorEditor,
                                  private juce::Timer
{
public:
    explicit PrismAudioProcessorEditor (PrismAudioProcessor&);
    ~PrismAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    using SliderAtt = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboAtt  = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAtt = juce::AudioProcessorValueTreeState::ButtonAttachment;

    void timerCallback() override;

    prism::GlowKnob*      makeKnob   (const char* paramID, const juce::String& caption, juce::Colour accent);
    juce::ComboBox*       makeCombo  (const char* paramID, juce::Colour accent);
    juce::ToggleButton*   makeToggle (const char* paramID, const juce::String& text, juce::Colour accent);

    void buildSections();
    void showPresetMenu();
    void selectProgram (int index);
    void refreshPresetName();

    PrismAudioProcessor& proc;
    prism::PrismLookAndFeel lnf;

    // section panels
    std::unique_ptr<prism::SectionPanel> oscP1, oscP2, subP, filterP,
                                         ampP, filtP, lfo1P, lfo2P,
                                         crushP, chorusP, delayP, reverbP;

    // live displays
    prism::WaveformDisplay    osc1Disp { prism::theme::osc };
    prism::WaveformDisplay    osc2Disp { prism::theme::osc };
    prism::FilterCurveDisplay filterCurve { prism::theme::filter };
    prism::EnvelopeDisplay    ampEnvDisp  { prism::theme::ampEnv };
    prism::EnvelopeDisplay    filtEnvDisp { prism::theme::filtEnv };

    // hero + transport
    prism::SupernovaButton nova;
    juce::TextButton presetPrev, presetNext, presetName;

    // top-bar controls
    juce::ComboBox* voiceModeCombo = nullptr;
    juce::ComboBox* glideModeCombo = nullptr;
    prism::GlowKnob* masterKnob = nullptr;
    prism::GlowKnob* glideKnob = nullptr;
    prism::GlowKnob* bendKnob = nullptr;

    // ownership
    juce::OwnedArray<prism::GlowKnob>    knobs;
    juce::OwnedArray<juce::ComboBox>     combos;
    juce::OwnedArray<juce::ToggleButton> toggles;
    juce::OwnedArray<SliderAtt> sliderAtts;
    juce::OwnedArray<ComboAtt>  comboAtts;
    juce::OwnedArray<ButtonAtt> buttonAtts;

    int lastProgram = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PrismAudioProcessorEditor)
};
