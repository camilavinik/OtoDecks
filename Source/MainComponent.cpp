#include "MainComponent.h"

MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(1000, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio) && !RuntimePermissions::isGranted(RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request(RuntimePermissions::recordAudio,
                                    [&](bool granted)
                                    { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(0, 2);
    }

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

void MainComponent::paint(Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); // clear the background
    g.setColour(juce::Colours::grey);
    g.drawRect(10, getHeight() / 2 - 1, getWidth() - 390, 2);
}

void MainComponent::resized()
{
    deckGUI1.setBounds(0, 0, getWidth() - 370, getHeight() / 2 - 1);
    deckGUI2.setBounds(0, getHeight() / 2 + 1, getWidth() - 370, getHeight() / 2);
    playlistComponent.setBounds(getWidth() - 370, 0, 370, getHeight());
}