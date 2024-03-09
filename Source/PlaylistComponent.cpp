#include <JuceHeader.h>
#include "PlaylistComponent.h"

PlaylistComponent::PlaylistComponent(DJAudioPlayer *_player1, DJAudioPlayer *_player2)
    : player1(_player1), player2(_player2)
{
  // Set columns
  tableComponent.getHeader().addColumn("Recent Files", 1, 205);
  tableComponent.getHeader().addColumn("", 2, 40);
  tableComponent.getHeader().addColumn("Deck 1", 3, 50);
  tableComponent.getHeader().addColumn("Deck 2", 4, 50);
  tableComponent.getHeader().addColumn("", 5, 25);

  // Set model to update the table on change
  tableComponent.setModel(this);

  addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent()
{
  // Remove model on destruction to avoid crashes
  tableComponent.setModel(nullptr);
}

void PlaylistComponent::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); // clear the background
}

void PlaylistComponent::resized()
{
  tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows()
{
  return tracks.size();
};

void PlaylistComponent::paintRowBackground(Graphics &g, int rowNumber, int width, int height, bool rowIsSelected)
{
  g.fillAll(Colours::grey);
};

void PlaylistComponent::paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
  File audioFile = tracks[rowNumber];

  if (columnId == 1)
  {
    // Draw the track name in the first column
    g.drawText(audioFile.getFileNameWithoutExtension().toStdString(), 2, 0, width - 4, height, Justification::centredLeft, true);
  }
  else if (columnId == 2)
  {
    // Draw the track duration in the second column
    g.drawText(getDuration(audioFile), 2, 0, width - 4, height, Justification::centred, true);
  }
};

Component *PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate)
{
  if (existingComponentToUpdate == nullptr)
  {
    if (columnId != 1 && columnId != 2) // If it's not the first or second column, it's a button
    {
      // Define button basic properties
      std::string columnID;
      TextButton *btn = new TextButton();
      btn->setColour(ComboBox::outlineColourId, Colours::grey);

      // Set button properties based on column: color, text, and id
      if (columnId == 3)
      {
        btn->setButtonText("PLAY");
        columnID = "_1";
        btn->setColour(TextButton::buttonColourId, Colour(35, 242, 120));
        btn->setColour(TextButton::textColourOffId, Colour(0, 0, 0));
      }
      else if (columnId == 4)
      {
        btn->setButtonText("PLAY");
        columnID = "_2";
        btn->setColour(TextButton::buttonColourId, Colour(35, 242, 224));
        btn->setColour(TextButton::textColourOffId, Colour(0, 0, 0));
      }
      else if (columnId == 5)
      {
        btn->setButtonText("X");
        btn->setColour(TextButton::buttonColourId, Colour(242, 35, 54));
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

void PlaylistComponent::buttonClicked(Button *button)
{
  // Destructure the button id to get the track and deck
  std::string id = button->getComponentID().toStdString();
  size_t pos = id.find('_');
  int key = std::stoi(id.substr(0, pos));

  File file = tracks[key];

  if (id.substr(pos + 1) == "1")
  {
    player1->loadFile(file); // Load the file into the player 1
  }
  else if (id.substr(pos + 1) == "2")
  {
    player2->loadFile(file); // Load the file into the player 2
  }
  else
  {
    // Delete the track from the playlist
    tracks.erase(tracks.begin() + key);
    tableComponent.updateContent();
    tableComponent.repaint();
  }
}

void PlaylistComponent::addTrack(File track)
{
  // If the track is already in the playlist, remove it
  auto iterator = std::find(tracks.begin(), tracks.end(), track);
  if (iterator != tracks.end())
  {
    tracks.erase(iterator);
  }

  // Add the track to the back of the playlist
  tracks.push_back(track);

  // Update the table
  tableComponent.updateContent();
  tableComponent.repaint();
}

std::string PlaylistComponent::getDuration(File file)
{
  // Use format manager to create a reader for the file
  AudioFormatManager formatManager;
  formatManager.registerBasicFormats();
  std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(file));

  if (reader != nullptr) // If file is valid, get the duration
  {
    // Get the duration in seconds
    int duration = reader->lengthInSamples / reader->sampleRate;
    // Convert the duration to MM:SS format
    int minutes = static_cast<int>(duration / 60);
    int seconds = static_cast<int>(duration) % 60;

    std::string min = minutes < 10 ? "0" + std::to_string(minutes) : std::to_string(minutes);
    std::string sec = seconds < 10 ? "0" + std::to_string(seconds) : std::to_string(seconds);

    return min + ":" + sec;
  }

  // If file is invalid, return 00:00
  return "00:00";
}
