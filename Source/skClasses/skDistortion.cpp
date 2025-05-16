/*
  ==============================================================================

    distortion.cpp
    Created: 8 Apr 2025 6:29:11pm
    Author:  jwh93

  ==============================================================================
*/

#include "skDistortion.h"
#include "JuceHeader.h"

SKDistortion::SKDistortion()
{
    forwardClip = 1.0;
    reverseClip = -1.0;
    gain = 0.5;

    octave = false;
    more = false;
    style = true;

    sampleRate = 44100.0;
}

void SKDistortion::setupDist(double Fs)
{
    postFilter->calc(1000.0, 1.0, 0.707, Fs);
}

void SKDistortion::setGain(double amount)
{
    gain = amount;
}

void SKDistortion::setMode(bool setting)
{
    style = setting;
}

void SKDistortion::setMore(bool mor)
{
    more = mor;
}

void SKDistortion::setOctave(bool oct)
{
    octave = oct;
}

void SKDistortion::setClipThreshold(double forward, double reverse)
{
    forwardClip = forward;
    reverseClip = reverse;
}

juce::dsp::AudioBlock<float> SKDistortion::clipBlock(juce::dsp::AudioBlock<float> block)
{
    if(!style)
    {
        for (int sample = 0; sample < block.getNumSamples(); sample++)
        {
            for (int channel = 0; channel < block.getNumChannels(); channel++)
            {
                float* data = block.getChannelPointer(channel);

                if (more)
                {
                    forwardClip = 0.5;
                    reverseClip = -0.5;
                }

                else
                {
                    forwardClip = 1.0;
                    reverseClip = -1.0;
                }

                data[sample] *= gain;

                if (data[sample] < 0.0f)
                {
                    data[sample] = -1.0f + exp(data[sample]);
                }
                else if (data[sample] > 0.0f)
                {
                    data[sample] = 1.0f - exp(-data[sample]);
                }
                else if (data[sample] == 0.0f)
                {
                    data[sample] = 0.0f;
                }

                data[sample] *= 2.0;

                if (data[sample] >= forwardClip)
                {
                    data[sample] = forwardClip;
                }
                else if (data[sample] <= reverseClip)
                {
                    data[sample] = reverseClip;
                }

                else {
                    data[sample] = data[sample];
                }


                if (octave)
                {
                    if (data[sample] < 0)
                    {
                        data[sample] = 0.0f;
                    }
                }

                data[sample] = postFilter->process(data[sample]);
            }
        }

        return block;
    }

    else
    {
        for (int sample = 0; sample < block.getNumSamples(); sample++)
        {
            for (int channel = 0; channel < block.getNumChannels(); channel++)
            {
                float* data = block.getChannelPointer(channel);
                data[sample] *= gain;

                data[sample] = doFuzz(data[sample]);

                if (more)
                {
                    data[sample] *= -0.8;
                    data[sample] = doFuzz(data[sample]);
                }

                if (octave)
                {
                    if (data[sample] < 0)
                    {
                        data[sample] = 0.0f;
                    }
                }

                data[sample] = postFilter->process(data[sample]);
            }
        }

        return block;
    }
}

float SKDistortion::doFuzz(float input)
{
    if (input >= 1.0)
    {
        input = 0.97;
    }

    else if (input < 1.0 && input >= 0)
    {
        input = input;
    }

    else if (input < 0 && input >= -0.25)
    {
        input = -0.38423 * pow(input, 3) + 0.59490 * pow(input, 2) + 0.78874 * input;
    }

    else if (input < -0.25 && input >= -0.5)
    {
        input = 0.79233 * pow(input, 3) + 1.4773 * pow(input, 2) + 1.0093 * input + 0.0018384;
    }

    else if (input < -0.5)
    {
        input = -0.231;
    }

    return input;
}



