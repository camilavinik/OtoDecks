#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "PlaylistComponent.h"

class DeckGUI : public juce::Component, public Button::Listener, public Slider::Listener, public FileDragAndDropTarget, public Timer
{
public:
  DeckGUI(DJAudioPlayer *player, PlaylistComponent &playlistComponent, Colour &color);
  ~DeckGUI() override;

  void paint(juce::Graphics &) override;
  void resized() override;

  void buttonClicked(Button *) override;
  void sliderValueChanged(Slider *slider) override;

  bool isInterestedInFileDrag(const StringArray &files) override;
  void filesDropped(const StringArray &files, int x, int y) override;

  void timerCallback() override;

private:
  TextButton playButton{"PLAY"};
  TextButton stopButton{"STOP"};
  TextButton loadButton{"LOAD"};
  TextButton closeButton{"X"};

  Slider volSlider;
  Slider speedSlider;
  Slider posSlider;

  Label fileName;
  Label time;

  Colour &color;

  DJAudioPlayer *player;

  PlaylistComponent &playlistComponent;

  juce::FileChooser fChooser{"Select a file...", File(), "*.wav;*.mp3"};

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
