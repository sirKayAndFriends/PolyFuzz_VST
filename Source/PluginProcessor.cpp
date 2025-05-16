/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
NarkAudioProcessor::NarkAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    aaf = std::make_unique<SKAntiAlias>();
    
    inputHP = std::make_unique<skFilters>(highpass);
    inputLP = std::make_unique<skFilters>(lowpass);
    trebleHS = std::make_unique<skFilters>(highshelf);
    bassLS = std::make_unique<skFilters>(lowshelf);
    scoopFilter = std::make_unique<skFilters>(peak);

    fuzz = std::make_unique<SKDistortion>();
}

NarkAudioProcessor::~NarkAudioProcessor()
{
}

//==============================================================================
const juce::String NarkAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NarkAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NarkAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NarkAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NarkAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NarkAudioProcessor::getNumPrograms()
{
    return 1;
}

int NarkAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NarkAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NarkAudioProcessor::getProgramName (int index)
{
    return {};
}

void NarkAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NarkAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();

    aaf->setup(samplesPerBlock, getSampleRate());

    inputHP->calc(50.0, 1.0, 0.707, getSampleRate());
    inputLP->calc(3500.0, 1.0, 0.707, getSampleRate());

    fuzz->setupDist(getSampleRate());

    updateParameters(getSampleRate());
}

void NarkAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NarkAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NarkAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    buffer.copyFrom(1, 0, buffer.getReadPointer(0), buffer.getNumSamples());
    auto inputBlock = juce::dsp::AudioBlock<float>(buffer);
    juce::dsp::AudioBlock<float> overBlock;

    updateParameters(getSampleRate());

    inputBlock = inputHP->processBlock(inputBlock, 2);

    inputBlock = inputLP->processBlock(inputBlock, 2);

    overBlock = aaf->processUp(inputBlock);

    overBlock = fuzz->clipBlock(overBlock);

    inputBlock = aaf->processDown(overBlock, inputBlock);

    inputBlock = scoopFilter->processBlock(inputBlock, 2);

    inputBlock = trebleHS->processBlock(inputBlock, 2);

    inputBlock = bassLS->processBlock(inputBlock, 2);

    inputBlock = gainScale(inputBlock, 2);
}

juce::dsp::AudioBlock<float> NarkAudioProcessor::gainScale(juce::dsp::AudioBlock<float> block, int numChannels)
{
    for (int sample = 0; sample < block.getNumSamples(); sample++)
    {
        for (int channel = 0; channel < numChannels; channel++)
        {
            float* data = block.getChannelPointer(channel);

            data[sample] *= 0.25;

            data[sample] *= volume;
        }
    }

    return block;
}


void NarkAudioProcessor::updateParameters(double Fs)
{
    volume = apvts.getRawParameterValue("VOLUME")->load();
    fuzzGain = apvts.getRawParameterValue("FUZZ")->load() * 1000.0;
    trebleGain = apvts.getRawParameterValue("TREBLE")->load() * 36.0 - 24.0;
    bassGain = apvts.getRawParameterValue("BASS")->load() * 12.0 - 6.0;
    scoopDepth = apvts.getRawParameterValue("SCOOP")->load() * 12.0 - 12.0;
    
    octaveUp = apvts.getRawParameterValue("OCTAVE")->load();
    fuzzStyle = apvts.getRawParameterValue("FUZZSTYLE")->load();
    amount = apvts.getRawParameterValue("AMT")->load();

    trebleHS->calc(2000.0, trebleGain, 0.707, Fs);
    bassLS->calc(250.0, bassGain, 0.707, Fs);
    scoopFilter->calc(1000.0, scoopDepth, 0.707, Fs);

    fuzz->setGain(fuzzGain);
    fuzz->setMode(fuzzStyle);
    fuzz->setOctave(octaveUp);
    fuzz->setMore(amount);
}

juce::AudioProcessorValueTreeState::ParameterLayout NarkAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME", "volume", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FUZZ", "fuzz", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("TREBLE", "treble", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("BASS", "bass", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SCOOP", "scoop", 0.0f, 1.0f, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterBool>("OCTAVE", "octave", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("FUZZSTYLE", "fuzzstyle", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("AMT", "amt", false));

    return { params.begin(), params.end() };
}

//==============================================================================
bool NarkAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NarkAudioProcessor::createEditor()
{
    return new NarkAudioProcessorEditor (*this);
}

//==============================================================================
void NarkAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void NarkAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NarkAudioProcessor();
}
