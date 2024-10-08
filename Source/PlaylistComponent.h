#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DJAudioPlayer.h"

class PlaylistComponent : public juce::Component, public TableListBoxModel, public Button::Listener
{
public:
  PlaylistComponent(DJAudioPlayer *player1, DJAudioPlayer *player2);
  ~PlaylistComponent() override;

  void paint(juce::Graphics &) override;
  void resized() override;

  int getNumRows() override;
  void paintRowBackground(Graphics &, int rowNumber, int width, int height, bool rowIsSelected) override;
  void paintCell(Graphics &, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
  Component *refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;

  void buttonClicked(Button *button) override;

  /** Receive a File and add it to the recent files table */
  void addTrack(File);

private:
  /** Get the duration of an audio file in the format MM:SS */
  static std::string getDuration(File audioFile);

  TableListBox tableComponent;
  std::vector<File> tracks;

  DJAudioPlayer *player1;
  DJAudioPlayer *player2;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
