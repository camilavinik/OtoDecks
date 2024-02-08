/*
  ==============================================================================

    DeckGUI.h
    Created: 4 Feb 2024 10:02:41pm
    Author:  Camila

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI : public juce::Component, public Button::Listener, public Slider::Listener, public FileDragAndDropTarget
{
  public:
    DeckGUI(DJAudioPlayer* player);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(Button *) override;
    void sliderValueChanged(Slider *slider) override;

    bool isInterestedInFileDrag(const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override;

  private:
    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    TextButton loadButton{"LOAD"};

    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;
    
    DJAudioPlayer* player;

    WaveformDisplay waveformDisplay;
    
    juce::FileChooser fChooser{"Select a file..."};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
