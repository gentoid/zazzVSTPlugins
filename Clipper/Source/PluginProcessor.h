#pragma once

#include <JuceHeader.h>

#include "../../../zazzVSTPlugins/Shared/NonLinearFilters/Clippers.h"
#include "../../../zazzVSTPlugins/Shared/Utilities/Oversampling.h"
#include "../../../zazzVSTPlugins/Shared/Utilities/AudioBuffer.h"

//==============================================================================
class ClipperAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{

public:
    //==============================================================================
    ClipperAudioProcessor();
    ~ClipperAudioProcessor() override;

	static const std::string paramsNames[];
	static const std::string labelNames[];
	static const std::string paramsUnitNames[];

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
	float getPeakReductiondB()
	{
		const float inputMaxdB = juce::Decibels::gainToDecibels(m_inputMax);
		const float outputMaxdB = juce::Decibels::gainToDecibels(m_outputMax);

		m_inputMax = 0.0f;
		m_outputMax = 0.0f;

		return inputMaxdB - outputMaxdB;
	}
	
	//==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	using APVTS = juce::AudioProcessorValueTreeState;
	static APVTS::ParameterLayout createParameterLayout();

	APVTS apvts{ *this, nullptr, "Parameters", createParameterLayout() };
	std::atomic<bool> m_guiIsOpen{ false };

private:	
	//==============================================================================	
	SlopeClipper m_slopeClipper[2];

	float m_lastSample[2];

	std::atomic<float>* typeParameter = nullptr;
	std::atomic<float>* thresholdParameter = nullptr;
	std::atomic<float>* mixParameter = nullptr;
	std::atomic<float>* volumeParameter = nullptr;

	float m_inputMax = 0.0f;
	float m_outputMax = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ClipperAudioProcessor)
};
