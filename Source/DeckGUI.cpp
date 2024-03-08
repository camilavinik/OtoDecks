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
  volSlider.setValue(0.5);
  volSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0); // Hide the textbox
  volSlider.setSliderStyle(Slider::SliderStyle::LinearVertical); // Vertical slider
  volSlider.setColour(Slider::thumbColourId, Colours::grey); // Set the color of the slider thumb

  // speed slider
  addAndMakeVisible(speedSlider);
  speedSlider.addListener(this);
  speedSlider.setRange(0.1, 2.0);
  speedSlider.setValue(1.0);
  speedSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0); // Hide the textbox
  speedSlider.setSliderStyle(Slider::SliderStyle::LinearVertical); // Vertical slider
  speedSlider.setColour(Slider::thumbColourId, Colours::grey); // Set the color of the slider thumb

  // position slider
  addAndMakeVisible(posSlider);
  posSlider.addListener(this);
  posSlider.setRange(0.0, 1.0);
  posSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0); // Hide the textbox
  posSlider.setColour(Slider::thumbColourId, Colours::red); // Set the color of the slider thumb
  posSlider.setColour(Slider::trackColourId, Colours::skyblue); // Set the color of the slider track
    
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

        g.setFont (14.0f);
        g.drawText (player->fileName, 10, 62, getWidth(), 20, 0, true);

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
  double waveWidth = getWidth() - 80;

  loadButton.setBounds(10, 10, getWidth() / 6, 35);
  playButton.setBounds(4 * getWidth() / 6 - 20, 10, getWidth() / 6, 35);
  stopButton.setBounds(5 * getWidth() / 6 - 10, 10, getWidth() / 6, 35);

  posSlider.setBounds(0, 80, waveWidth, 40);
  player->waveformDisplay.setBounds(11, 120, waveWidth - 22, getHeight() - 130);

  volSlider.setBounds(waveWidth, 120, 40, getHeight() - 130);
  speedSlider.setBounds(waveWidth + 40, 120, 40, getHeight() - 130);
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
                player->loadFile(selectedFile);
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
    return true;
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y) {
    std::cout << "DeckGUI::filesDropped" << std::endl;

    if (files.size() == 1) {
        File selectedFile = File{files[0]};
        player->loadFile(selectedFile);
        playlistComponent.addTrack(File{selectedFile});
        repaint();
    }
}

void DeckGUI::timerCallback() {
    double position = player->getPositionRelative(); //TODO: maybe we should remove this validation after we remove these component when no file
    
    if (player->forceRepaint) {
        player->forceRepaint = false;
        repaint();
    }

    if (!std::isnan(position)) {
        player->waveformDisplay.setPositionRelative(player->getPositionRelative());
        
        posSlider.setValue(player->getPositionRelative());
    }
}
