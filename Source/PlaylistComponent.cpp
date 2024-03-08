#include <JuceHeader.h>
#include "PlaylistComponent.h"

PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player1, DJAudioPlayer* _player2)
                                    : player1(_player1), player2(_player2)
{
  // Set columns
  tableComponent.getHeader().addColumn("Recent Files", 1, 245);
  tableComponent.getHeader().addColumn("Deck 1", 2, 50);
  tableComponent.getHeader().addColumn("Deck 2", 3, 50);
  tableComponent.getHeader().addColumn("", 4, 25);

  tableComponent.setModel(this);

  addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent()
{
  tableComponent.setModel(nullptr);
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows () {
  return tracks.size();
};

void PlaylistComponent::paintRowBackground (Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) {
  g.fillAll(Colours::darkgrey);
};
    
void PlaylistComponent::paintCell (Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    g.drawText(tracks[rowNumber].getFileNameWithoutExtension().toStdString(), 2, 0, width - 4, height, Justification::centredLeft, true);
};

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) {
  if (existingComponentToUpdate == nullptr) {
    if (columnId != 1) {
      std::string columnID;
      TextButton* btn = new TextButton();

      if (columnId == 2) {
        btn->setButtonText("play");
        columnID = "_1";
      } else if (columnId == 3) {
        btn->setButtonText("play");
        columnID = "_2";
      } else if (columnId == 4) {
        btn->setButtonText("X");
        btn->setColour(TextButton::buttonColourId, Colours::red);
        columnID = "_0";
      }

      String id{std::to_string(rowNumber) + columnID};
      btn->setComponentID(id);
      btn->addListener(this);
      existingComponentToUpdate = btn;
    }
  }
  
  return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button) {
  std::string id = button->getComponentID().toStdString();
  size_t pos = id.find('_');
  int key = std::stoi(id.substr(0, pos));

  File file = tracks[key];

  if (id.substr(pos + 1) == "1") { 
    player1->loadFile(file);
  } else if (id.substr(pos + 1) == "2") {
    player2->loadFile(file);
  } else {
    tracks.erase(tracks.begin() + key);
    tableComponent.updateContent();
    tableComponent.repaint();
  }
}

void PlaylistComponent::addTrack(File track)
{ 
  auto iterator = std::find(tracks.begin(), tracks.end(), track);
  if (iterator != tracks.end()) {
    tracks.erase(iterator);
  }

  tracks.push_back(track);

  tableComponent.updateContent();
  tableComponent.repaint();
}
