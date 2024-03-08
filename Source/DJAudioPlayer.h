#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

class DJAudioPlayer : public AudioSource {
  public:
    DJAudioPlayer(AudioFormatManager& _formatManager, AudioThumbnailCache & cacheToUse);
    ~DJAudioPlayer();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadFile(File audioFile);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);

    void start();
    void stop();

    /** get the relative position of the playhead */
    double getPositionRelative();
    
    /** return true if an audio file was loaded */
    bool hasAudioFile();

    /** return true if audio is playing, false if not */
    bool isPlaying();

    WaveformDisplay waveformDisplay;

    std::string fileName;

    bool forceRepaint = false;

  private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    ResamplingAudioSource resamplingSource{&transportSource, false, 2};
};
