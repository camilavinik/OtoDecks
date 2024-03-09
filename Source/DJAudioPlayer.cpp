#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager &_formatManager, AudioThumbnailCache &cacheToUse, Colour &color) : waveformDisplay(_formatManager, cacheToUse, color), formatManager(_formatManager) {}
DJAudioPlayer::~DJAudioPlayer() {}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
  transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
  resamplingSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
  resamplingSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
  transportSource.releaseResources();
  resamplingSource.releaseResources();
}

void DJAudioPlayer::loadFile(File audioFile)
{
  URL audioURL = URL{audioFile};
  auto *reader = formatManager.createReaderFor(audioURL.createInputStream(false));

  if (reader != nullptr) // If file is valid, load it
  {
    std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
    readerSource.reset(newSource.release());

    fileName = audioFile.getFileNameWithoutExtension().toStdString();
    waveformDisplay.loadURL(audioURL);
  }
}

void DJAudioPlayer::unload()
{
  // Clear the audio from the transport source
  transportSource.setSource(nullptr);
}

void DJAudioPlayer::setGain(double gain)
{
  if (gain < 0 || gain > 1.0) // Gain should be between 0 and 1
  {
    std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
  }
  else
  {
    transportSource.setGain(gain);
  }
}

void DJAudioPlayer::setSpeed(double ratio)
{
  if (ratio <= 0 || ratio > 100.0) // Speed should be between 0+ and 100
  {
    std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
  }
  else
  {
    resamplingSource.setResamplingRatio(ratio);
  }
}

void DJAudioPlayer::setPosition(double posInSecs)
{
  transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
  if (pos < 0 || pos > 1.0) // Position should be between 0 and 1
  {
    std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
  }
  else
  {
    double posInSecs = transportSource.getLengthInSeconds() * pos;
    setPosition(posInSecs);
  }
}

void DJAudioPlayer::start()
{
  transportSource.start();
}

void DJAudioPlayer::stop()
{
  transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
  return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

std::string DJAudioPlayer::getCurrentTime()
{
  double positionInSeconds = transportSource.getCurrentPosition(); // Get the current position in seconds

  // Convert the position to MM:SS format
  int minutes = static_cast<int>(positionInSeconds / 60);
  int seconds = static_cast<int>(positionInSeconds) % 60;

  std::string min = minutes < 10 ? "0" + std::to_string(minutes) : std::to_string(minutes);
  std::string sec = seconds < 10 ? "0" + std::to_string(seconds) : std::to_string(seconds);

  return min + ":" + sec;
}

bool DJAudioPlayer::hasAudioFile()
{
  // An audio file is loaded if the total length is greater than 0
  return transportSource.getTotalLength() > 0;
}

bool DJAudioPlayer::isPlaying()
{
  return transportSource.isPlaying();
}
