#include <JuceHeader.h>
#include "PlaylistComponent.h"

PlaylistComponent::PlaylistComponent()
{
  // Set columns
  tableComponent.getHeader().addColumn("Track Title", 1, 300);
  tableComponent.getHeader().addColumn("", 2, 1); //1 indicates dynamic width TODO: check documentation
  tableComponent.getHeader().addColumn("", 3, 1);
  tableComponent.getHeader().addColumn("", 4, 1);

  // Set stretch active for dynamic column widths
  tableComponent.getHeader().setStretchToFitActive(true);

  tableComponent.setModel(this);

  addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
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
  if (rowIsSelected) {
    g.fillAll(Colours::orange);
  } else {
    g.fillAll(Colours::darkgrey);
  }
};
    
void PlaylistComponent::paintCell (Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    g.drawText(tracks[rowNumber].getFileNameWithoutExtension().toStdString(), 2, 0, width - 4, height, Justification::centredLeft, true);
};

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) {
  if (existingComponentToUpdate == nullptr) {
    if (columnId == 2) { //TODO: refactor
      TextButton* btn = new TextButton("play on deck 1");
      String id{std::to_string(rowNumber) + "_deck_1"};
      btn->setComponentID(id);
      btn->addListener(this);
      existingComponentToUpdate = btn;
    } else if (columnId == 3) {
      TextButton* btn = new TextButton("play on deck 2");
      String id{std::to_string(rowNumber) + "_deck_2"};
      btn->setComponentID(id);
      btn->addListener(this);
      existingComponentToUpdate = btn;
    } else if (columnId == 4) {
      TextButton* btn = new TextButton("delete");
      String id{std::to_string(rowNumber) + "_delete"};
      btn->setComponentID(id);
      btn->addListener(this);
      existingComponentToUpdate = btn;
    }
  }
  
  return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button) {
  std::cout << "PlaylistComponent::buttonClicked " << button->getComponentID() << std::endl;
}

void PlaylistComponent::addTrack(File track)
{ 
  tracks.push_back(track);

  tableComponent.updateContent();
  tableComponent.repaint();
}
