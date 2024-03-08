#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component, public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager & formatManagerToUse, AudioThumbnailCache & cacheToUse, Colour& color);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void changeListenerCallback(ChangeBroadcaster *source) override;

    void loadURL(URL audioURL);

    /** set the relative position of the playhead */
    void setPositionRelative(double pos);

private:
    AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;

    Colour color;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
