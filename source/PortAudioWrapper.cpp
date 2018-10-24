#include "PortAudioWrapper.h"
#include "BaseException.h"
#include "BasicLogger.h"

#include "pa_util.h"

#include <cstring>
#include <functional>
#include <memory>

using namespace Utils::Logger;
using namespace BaseClass;

namespace Audio {
namespace PortAudio {

static const std::string TAG = "PortAudioWrapper";

PortAudioWrapper::PortAudioWrapper(
    int sampleRate,
    int numChannels,
    int bitsPerSample,
    IOType type,
    std::function<void(const void*, unsigned long)> callbackInterface)
    : m_paInputStream{nullptr},
      m_numLostSamples{0},
      m_callbackInterface{callbackInterface} {
    init(bitsPerSample, numChannels, sampleRate, type);
}

PortAudioWrapper::~PortAudioWrapper() {
    Pa_StopStream(m_paInputStream);
    Pa_CloseStream(m_paInputStream);
    Pa_Terminate();
}

/*
 * This method is deprecated. Right now data should be written into/read from
 * SharedDataStream by Writer/Reader instead of ringbuf provided by portaudio
 */

// [[deprecated]] void PortAudioWrapper::readData(
//     std::vector<int16_t>* paSamples) {
//     if (m_numLostSamples > 0) {
//         BasicLogger::getInstance().log(
//             TAG, LogLevel::INFO,
//             "RingBuffer overflow, number of lost samples:" +
//                 std::to_string(m_numLostSamples));
//         m_numLostSamples = 0;
//     }
//     auto numAvailableSamples =
//         PaUtil_GetRingBufferReadAvailable(&m_paRingBuffer);

//     // There may be no work to do
//     if (0 == numAvailableSamples) {
//         return;
//     }

//     // Get the data out the ringbuffer and into the vector
//     paSamples->resize(numAvailableSamples);
//     auto numReadSamples = PaUtil_ReadRingBuffer(
//         &m_paRingBuffer, paSamples->data(), numAvailableSamples);

//     // Confirm we read the amount of data we expected
//     if (numReadSamples != numAvailableSamples) {
//         BasicLogger::getInstance().log(
//             TAG, LogLevel::ERROR,
//             std::string("Error reading from PortAudio") +
//                 " | available samples:" + std::to_string(numAvailableSamples)
//                 + " | read samples:" + std::to_string(numReadSamples));
//     }
// }

void PortAudioWrapper::init(int bitsPerSample,
                            int numChannels,
                            int sampleRate,
                            IOType type) {
    BasicLogger::getInstance().log(
        TAG, LogLevel::DEBUG,
        std::string("Creating PortAudio library") + " | sample rate " +
            std::to_string(sampleRate) + " | sample size " +
            std::to_string(bitsPerSample) + " | number of channels " +
            std::to_string(numChannels) + " | IOType " +
            std::to_string(
                static_cast<std::underlying_type<IOType>::type>(type)));

    PaError paStatus = Pa_Initialize();
    if (paStatus != paNoError) {
        std::string errorMsg = std::string("Failed to initialize PortAudio. ") +
                               Pa_GetErrorText(paStatus);
        BasicLogger::getInstance().log(TAG, LogLevel::ERROR, errorMsg);

        throw BaseException(errorMsg);
    }

    // Ok, open the stream
    PaStreamParameters inputParameters;
    PaStreamParameters outputParameters;
    std::memset(&inputParameters, 0, sizeof(inputParameters));
    std::memset(&outputParameters, 0, sizeof(outputParameters));

    switch (type) {
        case IOType::INPUT:
            inputParameters.device = Pa_GetDefaultInputDevice();
            inputParameters.channelCount = numChannels;
            inputParameters.sampleFormat = paInt16;
            inputParameters.suggestedLatency =
                Pa_GetDeviceInfo(inputParameters.device)
                    ->defaultLowInputLatency;
            inputParameters.hostApiSpecificStreamInfo = nullptr;

            break;
        case IOType::OUTPUT:
            outputParameters.device = Pa_GetDefaultOutputDevice();
            outputParameters.channelCount = numChannels;
            outputParameters.sampleFormat = paInt16;
            outputParameters.suggestedLatency =
                Pa_GetDeviceInfo(outputParameters.device)
                    ->defaultLowOutputLatency;
            outputParameters.hostApiSpecificStreamInfo = nullptr;
            break;
        default:
            std::string errorMsg =
                "Failed to initialize PortAudio. Invalid IOType";
            BasicLogger::getInstance().log(TAG, LogLevel::ERROR, errorMsg);

            throw BaseException(errorMsg);
    }

    // TODO: Add support for output
    paStatus = Pa_OpenStream(&m_paInputStream, &inputParameters, nullptr,
                             sampleRate, paFramesPerBufferUnspecified, paNoFlag,
                             portAudioCallback, this);
    // paStatus =
    //     Pa_OpenStream(&m_paOutputStream, &outputParameters, nullptr,
    //     sampleRate,
    //                   paFramesPerBufferUnspecified, paNoFlag, nullptr, this);

    if (paStatus != paNoError) {
        std::string errorMsg = "Failed to open PortAudio stream.";
        BasicLogger::getInstance().log(TAG, LogLevel::ERROR,
                                       errorMsg + Pa_GetErrorText(paStatus));

        throw BaseException(errorMsg);
    }
}
void PortAudioWrapper::startStream() {
    std::lock_guard<std::mutex> lock(m_portAudioMtx);
    PaError paStatus = Pa_StartStream(m_paInputStream);
    // Pa_StartStream(m_paOutputStream);

    if (paStatus != paNoError) {
        std::string errorMsg =
            std::string("Failed to start PortAudio stream.") +
            Pa_GetErrorText(paStatus);
        BasicLogger::getInstance().log(TAG, LogLevel::ERROR, errorMsg);
        throw BaseException(errorMsg);
    }
}
void PortAudioWrapper::stopStream() {
    std::lock_guard<std::mutex> lock(m_portAudioMtx);
    PaError paStatus = Pa_StopStream(m_paInputStream);
    if (paStatus != paNoError) {
        std::string errorMsg = std::string("Failed to stop PortAudio stream.") +
                               Pa_GetErrorText(paStatus);
        BasicLogger::getInstance().log(TAG, LogLevel::ERROR, errorMsg);
        throw BaseException(errorMsg);
    }
}
/**
 * PortAudio will call this function once sampling is done
 */
int PortAudioWrapper::portAudioCallback(
    const void* inputBuffer,
    void* outputBuffer,
    unsigned long numSamples,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData) {
    auto paWrapper = static_cast<PortAudioWrapper*>(userData);
    if (paWrapper->m_callbackInterface != nullptr) {
        paWrapper->m_callbackInterface(inputBuffer, numSamples);
    }
    return paContinue;
}

}  // namespace PortAudio
}  // namespace Audio
