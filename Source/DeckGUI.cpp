#include <JuceHeader.h>
#include "DeckGUI.h"

DeckGUI::DeckGUI(DJAudioPlayer* _player, PlaylistComponent& _playlistComponent) 
                : player(_player), playlistComponent(_playlistComponent)
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
  posSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0); // Hide the textbox
  posSlider.setColour(Slider::thumbColourId, Colours::lightgreen); // Set the color of the slider thumb
  posSlider.setColour(Slider::trackColourId, Colours::orange); // Set the color of the slider track
    
  // load button
  addAndMakeVisible(loadButton); 
  loadButton.addListener(this);

  // wave display
  addAndMakeVisible(player->waveformDisplay);

  // timer
  startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.setColour (juce::Colours::grey);

    if (player->hasAudioFile()) {
        volSlider.setVisible(true);
        playButton.setVisible(true);
        stopButton.setVisible(true);
        speedSlider.setVisible(true);
        posSlider.setVisible(true);
        player->waveformDisplay.setVisible(true);

        playButton.setEnabled(!player->isPlaying());
        stopButton.setEnabled(player->isPlaying());
    } else {
        volSlider.setVisible(false);
        playButton.setVisible(false);
        stopButton.setVisible(false);
        speedSlider.setVisible(false);
        posSlider.setVisible(false);
        player->waveformDisplay.setVisible(false);
    }
}

void DeckGUI::resized()
{
  double rowH = 40;

  loadButton.setBounds(0, 0, getWidth() / 6, rowH);
  playButton.setBounds(4 * getWidth() / 6, 0, getWidth() / 6, rowH);
  stopButton.setBounds(5 * getWidth() / 6, 0 * 7, getWidth() / 6, rowH);
  volSlider.setBounds(0, rowH * 1, getWidth(), rowH);
  speedSlider.setBounds(0, rowH * 2, getWidth(), rowH);
  posSlider.setBounds(0, rowH * 3, getWidth(), rowH);
  player->waveformDisplay.setBounds(11, rowH * 4, getWidth() - 22, rowH * 3);
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
                playlistComponent.addTrack(selectedFile);
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
        playlistComponent.addTrack(File{files[0]});
    }
}

void DeckGUI::timerCallback() {
    double position = player->getPositionRelative(); //TODO: maybe we should remove this validation after we remove these component when no file
    
    if (!std::isnan(position)) {
        player->waveformDisplay.setPositionRelative(player->getPositionRelative());
        
        posSlider.setValue(player->getPositionRelative());
    }
}
