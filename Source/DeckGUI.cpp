/*
  ==============================================================================

    DeckGUI.cpp
    Created: 4 Feb 2024 10:02:41pm
    Author:  Camila

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, AudioFormatManager & formatManagerToUse, AudioThumbnailCache & cacheToUse, PlaylistComponent& _playlistComponent) 
                : player(_player), waveformDisplay(formatManagerToUse, cacheToUse), playlistComponent(_playlistComponent)
{
  // play button
  addAndMakeVisible(playButton);
  playButton.addListener(this);

  // stop button
  addAndMakeVisible(stopButton);
  stopButton.addListener(this);

  // volume slider
  addAndMakeVisible(volSlider);
  volSlider.addListener(this);
  volSlider.setRange(0.0, 1.0);

  // speed slider
  addAndMakeVisible(speedSlider);
  speedSlider.addListener(this);

  // position slider
  addAndMakeVisible(posSlider);
  posSlider.addListener(this);
  posSlider.setRange(0.0, 1.0);

  // load button
  addAndMakeVisible(loadButton); 
  loadButton.addListener(this);

  // wave display
  addAndMakeVisible(waveformDisplay);

  // timer
  startTimer(500);

}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("DeckGUI", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
  double rowH = getHeight() / 8;

  playButton.setBounds(0, 0, getWidth(), rowH);
  stopButton.setBounds(0, rowH, getWidth(), rowH);
  volSlider.setBounds(0, rowH * 2, getWidth(), rowH);
  speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);
  posSlider.setBounds(0, rowH * 4, getWidth(), rowH);
  waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH * 2);
  loadButton.setBounds(0, rowH * 7, getWidth(), rowH);
}

void DeckGUI::buttonClicked(Button* button) 
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked" << std::endl;
        player->start();
    }

    if (button == &stopButton)
    {
        std::cout << "Stop button was clicked" << std::endl;
        player->stop();
    }

    if (button == &loadButton)
    {
        // this does work in 6.1 but the syntax is a little funky
        // https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
        // - configure the dialogue
        auto fileChooserFlags = 
        FileBrowserComponent::canSelectFiles;
        // - launch out of the main thread
        // - note how we use a lambda function which you've probably
        // not seen before. Please do not worry too much about that. 
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            File selectedFile = chooser.getResult();
            if (selectedFile != File{}) { // solved bug with this, include on report
                player->loadURL(URL{selectedFile});
                waveformDisplay.loadURL(URL{selectedFile});
                
                playlistComponent.addTrack(selectedFile.getFileNameWithoutExtension().toStdString());
            }
        });
    }
}

void DeckGUI::sliderValueChanged(Slider* slider) 
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const StringArray &files) {
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y) {
    std::cout << "DeckGUI::filesDropped" << std::endl;

    if (files.size() == 1) {
        player->loadURL(URL(File{files[0]}));
        //TODO: update playlist
    }
}

void DeckGUI::timerCallback() {
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}
