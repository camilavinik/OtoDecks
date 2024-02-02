/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent,
                        public Button::Listener,
                        public Slider::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    /** Implement Button::Listener*/
    void buttonClicked (Button *) override;

        /** Implement Slider::Listener*/
    void sliderValueChanged (Slider *slider) override;

private:
    //==============================================================================
    // Your private member variables go here...

    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    TextButton loadButton{"LOAD"};

    Slider volSlider; 

    Random rand;

    double phase;
    double dphase;

    AudioFormatManager formatManager;

    std::unique_ptr<AudioFormatReaderSource> readerSource;

    AudioTransportSource transportSource;

    void loadURL(URL audioURL);

    juce::FileChooser fChooser{"Select a file..."};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
