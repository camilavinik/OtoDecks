#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

class MainComponent : public AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
    void releaseResources() override;

    void paint(Graphics &g) override;
    void resized() override;

private:
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100};

    Colour deck1Color = Colour(35, 242, 120);
    Colour deck2Color = Colour(35, 242, 224);

    DJAudioPlayer player1{formatManager, thumbCache, deck1Color};
    DJAudioPlayer player2{formatManager, thumbCache, deck2Color};

    PlaylistComponent playlistComponent{&player1, &player2};

    DeckGUI deckGUI1{&player1, playlistComponent, deck1Color};
    DeckGUI deckGUI2{&player2, playlistComponent, deck2Color};

    MixerAudioSource mixerSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
