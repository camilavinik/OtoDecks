#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

class DJAudioPlayer : public AudioSource
{
public:
  DJAudioPlayer(AudioFormatManager &_formatManager, AudioThumbnailCache &cacheToUse, Colour &color);
  ~DJAudioPlayer();

  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
  void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
  void releaseResources() override;

  /** Receives an audio file and loads it to the player*/
  void loadFile(File audioFile);

  /** Unload the audio file from the player*/
  void unload();

  /** Change the gain (volume) of the audio file */
  void setGain(double gain);

  /** Change the speed of the audio file */
  void setSpeed(double ratio);

  /** Set the position of the playhead in seconds */
  void setPosition(double posInSecs);

  /** Set the position of the playhead relative to the audio file length*/
  void setPositionRelative(double pos);

  /** Start audio player */
  void start();

  /** Stop audio player */
  void stop();

  /** Get the relative position of the playhead */
  double getPositionRelative();

  /** Get the current time in the format MM:SS */
  std::string getCurrentTime();

  /** Return true if an audio file was loaded, false if not */
  bool hasAudioFile();

  /** Return true if audio is currently playing, false if not */
  bool isPlaying();

  WaveformDisplay waveformDisplay;

  std::string fileName;

private:
  AudioFormatManager &formatManager;
  std::unique_ptr<AudioFormatReaderSource> readerSource;
  AudioTransportSource transportSource;
  ResamplingAudioSource resamplingSource{&transportSource, false, 2};
};
