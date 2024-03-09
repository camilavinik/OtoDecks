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
WaveformDisplay::WaveformDisplay(AudioFormatManager &formatManagerToUse, AudioThumbnailCache &cacheToUse, Colour &_color) : audioThumb(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0), color(_color)
{
  audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); // clear the background

  g.setColour(color);

  audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);
  g.setColour(Colour(242, 35, 54));
  g.drawRect(position * getWidth(), 0, 2, getHeight());
}

void WaveformDisplay::resized()
{
}

void WaveformDisplay::loadURL(URL audioURL)
{
  audioThumb.clear();
  fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));

  if (fileLoaded)
  {
    std::cout << "wfd: loaded!" << std::endl;
  }
  else
  {
    std::cout << "wfd: not loaded" << std::endl;
  }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster *source)
{
  repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
  if (pos != position)
  {
    position = pos;
    repaint();
  }
}
