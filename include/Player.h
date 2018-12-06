#pragma once

#include "AudioStream.h"
#include "PortAudioWrapper.h"

namespace Audio {
namespace Player {
class Player {
  public:
    Player(const int sampleRate,
           const int bitsPerSample,
           const int numChannels,
           std::shared_ptr<AudioOutputStream::Reader> reader,
           std::shared_ptr<PortAudio::PortAudioWrapper> portAudioWrapper);
    ~Player();

    void startPlay();
    void stopPlay();
    bool isPlaying() const;
    bool hasDataToPlay() const;

    const int m_sampleRate;
    const int m_bitsPerSample;
    const int m_numChannels;

  private:
    // noncopyable
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    std::shared_ptr<PortAudio::PortAudioWrapper> m_portAudioWrapper;
    std::shared_ptr<AudioOutputStream::Reader> m_reader;
    std::atomic<bool> m_isReady;
    std::atomic<bool> m_isPlaying;
    std::atomic<bool> m_hasDataToPlay;
};
}  // namespace Player
}  // namespace Audio
