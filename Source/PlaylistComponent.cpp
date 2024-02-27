/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 22 Feb 2024 7:37:07pm
    Author:  Camila

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 3");
    trackTitles.push_back("Track 4");
    trackTitles.push_back("Track 5");
    trackTitles.push_back("Track 6");

    tableComponent.getHeader().addColumn("Track Title", 1, 400);
    tableComponent.getHeader().addColumn("", 2, 400);
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
  return trackTitles.size();
};

void PlaylistComponent::paintRowBackground (Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) {
  if (rowIsSelected) {
    g.fillAll(Colours::orange);
  } else {
    g.fillAll(Colours::darkgrey);
  }
};
    
void PlaylistComponent::paintCell (Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
};

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) {
  if (columnId == 2 && existingComponentToUpdate == nullptr) {
    existingComponentToUpdate = new TextButton("play");
  }
  
    return existingComponentToUpdate;
}
