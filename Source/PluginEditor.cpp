/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NarkAudioProcessorEditor::NarkAudioProcessorEditor (NarkAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setLookAndFeel(&skVibes);

    vibeParams = skVibes.aspectRatio(50.0, 12, 9);
    setSize (vibeParams->width, vibeParams->height);

    volumeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "VOLUME", mVolumeSlider);
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "FUZZ", mGainSlider);
    trebleSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "TREBLE", mTrebleSlider);
    bassSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "BASS", mBassSlider);
    scoopSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, "SCOOP", mScoopSlider);

    octaveButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.apvts, "OCTAVE", octButton);
    fuzzButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.apvts, "FUZZSTYLE", fuzzButton);
    moreButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.apvts, "AMT", moreButton);

    skVibes.createDivisions();

    skVibes.setupSKSliders(allKnobs, knobCols, allLabels, labelText);
    skVibes.setupSKButtons(buttons);
    skVibes.setupSKTitle(&titleLabel);
    bgParams = skVibes.setupPanels(0.0, 0.0, getWidth(), getHeight(), SKVibes::skPlum);

    unitSize = skVibes.getUnitSize();

    knobSize = skVibes.getKnobSize();
    buttonSize = skVibes.getButtonSize();

    for (int i = 0; i < allKnobs.size(); i++)
    {
        addAndMakeVisible(allKnobs[i]);
        addAndMakeVisible(allLabels[i]);
        allKnobs[1]->addListener(this);
    }
    
    knobPos =
    {
        skVibes.drawOnIntersection(2, 2, allKnobs[0]),
        skVibes.drawOnIntersection(6, 4, allKnobs[1]),
        skVibes.drawOnIntersection(10, 2, allKnobs[2]),
        skVibes.drawOnIntersection(2, 7, allKnobs[3]),
        skVibes.drawOnIntersection(10, 7, allKnobs[4]),
    };

    for (int i = 0; i < buttons.size(); i++)
    {
        addAndMakeVisible(buttons[i]);
    }

    octPos = skVibes.drawOnVertical(2, 4, buttons[0]);
    fuzzPos = skVibes.drawOnVertical(9, 4,buttons[1]);
    morePos = skVibes.drawOnVertical(11, 4, buttons[2]);

    addAndMakeVisible(titleLabel);
    titlePos = skVibes.drawOnVertical(6, 1, &titleLabel);

    scoopVisualPos = skVibes.drawOnHorizontal(4, 7);
    scoopVisual = skVibes.setVisualParams(scoopVisualPos.x, scoopVisualPos.y, unitSize * 3, unitSize * 2);
    updateFrequencyLabel(allKnobs[1]);
}

NarkAudioProcessorEditor::~NarkAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void NarkAudioProcessorEditor::paint (juce::Graphics& g)
{
    if (fuzzButton.getToggleState() == false)
    {
        bgParams.colour = SKVibes::skPlum;
        skVibes.updatePanels(&bgParams);
        titleLabel.setText("SMILING\n" "POLITELY", juce::NotificationType::dontSendNotification);
        titleLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour(SKVibes::skPeach));
        repaint();
    }

    else
    {
        bgParams.colour = SKVibes::skPsychOrange;
        skVibes.updatePanels(&bgParams);
        titleLabel.setText("PLASTIC\n" "HASSLE", juce::NotificationType::dontSendNotification);
        titleLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour(SKVibes::skPlum));
        repaint();
    }

    skVibes.drawPanel(g, &bgParams);

    for (int i = 0; i < allKnobs.size(); i++)
    {
        allKnobs[i]->setBounds(knobPos[i].x, knobPos[i].y, knobSize, knobSize);
        allLabels[i]->setBounds(knobPos[i].x, knobPos[i].y + knobSize, knobSize, 25);

        if (buttons[1]->getToggleState())
        {
            allLabels[i]->setColour(juce::Label::ColourIds::textColourId, juce::Colour(SKVibes::skPlum));
        }

        else
        {
            allLabels[i]->setColour(juce::Label::ColourIds::textColourId, juce::Colour(SKVibes::skPeach));
        }
        
    }

    buttons[0]->setBounds(octPos.x, octPos.y, buttonSize, buttonSize);
    buttons[1]->setBounds(fuzzPos.x, fuzzPos.y, buttonSize, buttonSize);
    buttons[2]->setBounds(morePos.x, morePos.y, buttonSize, buttonSize);

    skVibes.drawMidVisual(g, &scoopVisual, centrePoint, scoopDepthValue);  

    titleLabel.setBounds(titlePos.x, titlePos.y, skVibes.getTitleSize()[0], skVibes.getTitleSize()[1]);

    if (buttons[2]->getToggleState())
    {
        buttons[2]->setButtonText("MORE");
    }

    else
    {
        buttons[2]->setButtonText("LESS");
    }
}

void NarkAudioProcessorEditor::resized()
{
}

void NarkAudioProcessorEditor::updateFrequencyLabel(juce::Slider* slider)
{
    if (slider == &mScoopSlider)
    {
        double sliderVal =  slider->getValue() - 1.0;
        scoopDepthValue = scoopVisual.lineY - (sliderVal * scoopVisual.humpHeight);

        centrePoint = scoopVisual.controlMin + scoopVisual.controlWidth / 2.0;
        repaint();
    }
}

void NarkAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &mScoopSlider)
    {
        updateFrequencyLabel(slider);
    }
}

void NarkAudioProcessorEditor::buttonClicked(juce::Button* button)
{

}
