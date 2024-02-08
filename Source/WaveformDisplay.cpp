/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 7 Feb 2024 9:22:05pm
    Author:  Camila

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager & formatManagerToUse, AudioThumbnailCache & cacheToUse): audioThumb(1000, formatManagerToUse, cacheToUse), fileLoaded(false)
{
  audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::orange);

    if (fileLoaded) {
        audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);
    } else {
      g.setFont (20.0f);
      g.drawText ("File not loaded...", getLocalBounds(),
      juce::Justification::centred, true);
    }
}

void WaveformDisplay::resized()
{
}

void WaveformDisplay::loadURL(URL audioURL) {
  audioThumb.clear();
  fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));

  if (fileLoaded) {
    std::cout << "wfd: loaded!" << std::endl;
  } else {
    std::cout << "wfd: not loaded" << std::endl;
  }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster *source) {
  std::cout << "wfd: change received!" << std::endl;
    repaint();
}
