/*
  ==============================================================================

    distortion.h
    Created: 8 Apr 2025 6:29:22pm
    Author:  jwh93

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "skFilters.h"

class SKDistortion
{
public:
    SKDistortion();

    void setGain(double amount);

    void setupDist(double Fs);
    
    void setMode(bool setting);

    void setMore(bool mor);

    void setOctave(bool oct);
    
    juce::dsp::AudioBlock<float> clipBlock(juce::dsp::AudioBlock<float> block);

    float doFuzz(float input);

    void setClipThreshold(double forward, double reverse);

private:
    
    int clipMode;
    double forwardClip, reverseClip;
    double gain;
    double sampleRate;
    bool octave;
    bool more;
    bool style;//true == face

    std::unique_ptr<skFilters> postFilter = std::make_unique<skFilters>(lowpass);
};
