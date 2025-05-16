/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "skClasses/skVibes.h"

//==============================================================================
/**
*/
class NarkAudioProcessorEditor : public juce::AudioProcessorEditor,
                                    public juce::Slider::Listener,
                                    public juce::Button::Listener

{
public:
    NarkAudioProcessorEditor (NarkAudioProcessor&);
    ~NarkAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;
    void updateFrequencyLabel(juce::Slider* slider);


private:

    SKVibes skVibes;

    double knobSize, buttonSize;

    struct backGroundColour
    {
        int narkInner;
        int narkOuter;
        int plasticInner;
        int plasticOuter;
    };

    SKVibes::windowParams* vibeParams;
    SKVibes::panelParams bgParams;
    SKVibes::visualParams scoopVisual;

    std::vector<int> knobCols
    {
        SKVibes::skLightBlue,
        SKVibes::skBarleyGreen,
        SKVibes::skLightBlue,
        SKVibes::skMauve,
        SKVibes::skSeville
    };

    //std::vector<float> unitSize;
    double unitSize;

    juce::Slider mVolumeSlider, mGainSlider, mTrebleSlider, mBassSlider, mScoopSlider;

    std::vector<juce::Slider*> allKnobs
    {
        &mTrebleSlider,
        &mScoopSlider,
        &mBassSlider,
        &mGainSlider,
        &mVolumeSlider
    };

    std::vector<juce::Point<float>> knobPos;

    float visualWidth;
    float visualHeight;
    float scoopDepthValue;
    float centrePoint;
    juce::Point<float> scoopVisualPos;

    juce::Label fuzz, treble, scoop, bass,level;

    std::vector<juce::Label*> allLabels
    {
        &treble,
        &scoop,
        &bass,
        &fuzz,
        &level
    };

    std::vector<juce::String> labelText
    {
        "TREBLE",
        "SCOOP",
        "BASS",
        "FUZZ",
        "LEVEL",
    };

    juce::TextButton octButton{ "OCT" }, fuzzButton{ "STYLE" }, moreButton{ "LESS" };
    
    std::vector<juce::TextButton*> buttons
    {
        &octButton,
        &fuzzButton,
        &moreButton
    };

    juce::Point<float> octPos, fuzzPos, morePos;

    juce::Label titleLabel{ "N A R K !" };
    juce::Point<float> titlePos;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trebleSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bassSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> scoopSliderAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> octaveButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> fuzzButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> moreButtonAttachment;

    NarkAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NarkAudioProcessorEditor)
};
