/*
  ==============================================================================

    WaveformDisplay.h
    Created: 7 Feb 2024 9:22:05pm
    Author:  Camila

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component, public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager & formatManagerToUse, AudioThumbnailCache & cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void changeListenerCallback(ChangeBroadcaster *source) override;

    void loadURL(URL audioURL);

private:
    AudioThumbnail audioThumb;
    bool fileLoaded;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
