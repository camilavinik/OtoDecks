#include <JuceHeader.h>
#include "DeckGUI.h"

DeckGUI::DeckGUI(DJAudioPlayer *_player, PlaylistComponent &_playlistComponent, Colour &_color)
    : color(_color), player(_player), playlistComponent(_playlistComponent)
{
    // Play button
    addAndMakeVisible(playButton);
    playButton.addListener(this);

    // Stop button
    addAndMakeVisible(stopButton);
    stopButton.addListener(this);

    // Volume slider
    addAndMakeVisible(volSlider);
    volSlider.addListener(this);
    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(0.5);
    volSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);     // Hide the textbox
    volSlider.setSliderStyle(Slider::SliderStyle::LinearVertical); // Make it vertical
    volSlider.setColour(Slider::thumbColourId, Colours::grey);     // Set the color of the slider thumb

    // Speed slider
    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setRange(0.1, 2.0);                                  // Set the range of the slider
    speedSlider.setValue(1.0);                                       // Set initial value
    speedSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);     // Hide the textbox
    speedSlider.setSliderStyle(Slider::SliderStyle::LinearVertical); // Make it vertical
    speedSlider.setColour(Slider::thumbColourId, Colours::grey);     // Set the color of the slider thumb

    // Position slider
    addAndMakeVisible(posSlider);
    posSlider.addListener(this);
    posSlider.setRange(0.0, 1.0);                                    // Set the range of the slider
    posSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);       // Hide the textbox
    posSlider.setColour(Slider::thumbColourId, Colour(242, 35, 54)); // Set the color of the slider thumb
    posSlider.setColour(Slider::trackColourId, color);               // Set the color of the slider track

    // Load button
    addAndMakeVisible(loadButton);
    loadButton.addListener(this);

    // Close/Delete button
    addAndMakeVisible(closeButton);
    closeButton.addListener(this);

    // Wave display
    addAndMakeVisible(player->waveformDisplay);

    // File name label
    addAndMakeVisible(fileName);
    fileName.setFont(Font(14.0f));                          // Set the font size
    fileName.setColour(Label::textColourId, Colours::grey); // Set the color of the text

    // Current time label
    addAndMakeVisible(time);
    time.setFont(Font(14.0f));                          // Set the font size
    time.setColour(Label::textColourId, Colours::grey); // Set the color of the text

    // Start timer
    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    // Stop timer when the component is deleted
    stopTimer();
}

void DeckGUI::paint(juce::Graphics &g)
{

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); // clear the background
    g.setColour(juce::Colours::grey);

    if (player->hasAudioFile()) // If audio file loaded show all the controls
    {
        closeButton.setVisible(true);
        volSlider.setVisible(true);
        playButton.setVisible(true);
        stopButton.setVisible(true);
        speedSlider.setVisible(true);
        posSlider.setVisible(true);
        player->waveformDisplay.setVisible(true);
        fileName.setVisible(true);
        time.setVisible(true);

        fileName.setText(player->fileName, dontSendNotification);
        time.setText(player->getCurrentTime(), dontSendNotification);

        playButton.setEnabled(!player->isPlaying()); // Disable the play button if the player is playing
        stopButton.setEnabled(player->isPlaying());  // Disable the stop button if the player is not playing

        loadButton.setBounds(55, 10, getWidth() / 6, 35);
    }
    else // If no audio file loaded hide all the controls and center the load button
    {
        closeButton.setVisible(false);
        volSlider.setVisible(false);
        playButton.setVisible(false);
        stopButton.setVisible(false);
        speedSlider.setVisible(false);
        posSlider.setVisible(false);
        player->waveformDisplay.setVisible(false);
        fileName.setVisible(false);
        time.setVisible(false);

        int buttonWidth = getWidth() / 6;
        loadButton.setBounds((getWidth() - buttonWidth) / 2, (getHeight() - 35) / 2, buttonWidth, 35);
    }
}

void DeckGUI::resized()
{
    double waveWidth = getWidth() - 80;

    closeButton.setBounds(10, 10, 35, 35);
    playButton.setBounds(4 * getWidth() / 6 - 20, 10, getWidth() / 6, 35);
    stopButton.setBounds(5 * getWidth() / 6 - 10, 10, getWidth() / 6, 35);

    posSlider.setBounds(0, 80, waveWidth, 40);
    player->waveformDisplay.setBounds(11, 120, waveWidth - 22, getHeight() - 130);

    volSlider.setBounds(waveWidth, 120, 40, getHeight() - 130);
    speedSlider.setBounds(waveWidth + 40, 120, 40, getHeight() - 130);

    fileName.setBounds(10, 62, getWidth(), 20);
    time.setBounds(getWidth() - 69, 90, 60, 20);
}

void DeckGUI::buttonClicked(Button *button)
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

    if (button == &closeButton)
    {
        std::cout << "Close/Delete button was clicked" << std::endl;
        player->unload();
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
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser &chooser)
                             {
            File selectedFile = chooser.getResult();
            if (selectedFile != File{}) { // If a file was selected load it and add it to the playlist
                player->loadFile(selectedFile);
                playlistComponent.addTrack(selectedFile);
            } });
    }
}

void DeckGUI::sliderValueChanged(Slider *slider)
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

bool DeckGUI::isInterestedInFileDrag(const StringArray &files)
{
    return true;
}

void DeckGUI::filesDropped(const StringArray &files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped" << std::endl;

    if (files.size() == 1)
    {
        File selectedFile = File{files[0]};

        if (selectedFile.hasFileExtension("wav") || selectedFile.hasFileExtension("mp3"))
        {
            player->loadFile(selectedFile);
            playlistComponent.addTrack(File{selectedFile});
            repaint();
        }
        else
        {
            std::cout << "DeckGUI::filesDropped Unsported format, use .wav or .mp3 files" << std::endl;
        }
    }
}

void DeckGUI::timerCallback()
{
    if (player->hasAudioFile()) // If file is loaded
    {
        player->waveformDisplay.setPositionRelative(player->getPositionRelative()); // Update the position of the playhead in the waveform display
        posSlider.setValue(player->getPositionRelative());                          // Update the position slider with current position
        repaint();                                                                  // repaint the component to make sure the time label is updated
    }
}
