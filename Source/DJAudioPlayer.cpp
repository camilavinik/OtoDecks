/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 3 Feb 2024 3:13:35pm
    Author:  Camila

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer() {}
DJAudioPlayer::~DJAudioPlayer() {}

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {
  formatManager.registerBasicFormats();
  transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
  resamplingSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) {
  resamplingSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources() {
  transportSource.releaseResources();
  resamplingSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL) {
  auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
  
  if (reader != nullptr) {// good file! 
      std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
      transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
      readerSource.reset(newSource.release());
  }
}
void DJAudioPlayer::setGain(double gain) {
  if (gain < 0 || gain > 1) {
    std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
  } else {
    transportSource.setGain(gain);
  }
}

void DJAudioPlayer::setSpeed(double ratio) {
  if (ratio < 0 || ratio > 100.0) {
    std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
  } else {
    resamplingSource.setResamplingRatio(ratio);
  }
}

void DJAudioPlayer::setPosition(double posInSecs) {}

void DJAudioPlayer::start() {
  transportSource.start();
}

void DJAudioPlayer::stop() {
  transportSource.stop();
}
