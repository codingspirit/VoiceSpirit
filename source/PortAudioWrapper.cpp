#include "PortAudioWrapper.h"
#include "BaseException.h"
#include "BasicLogger.h"

#include "pa_util.h"

#include <cstring>
#include <functional>
#include <memory>

using namespace Utils::Logger;
using BaseClass::BaseException;

namespace Audio {
namespace PortAudio {

static const std::string TAG = "PortAudioWrapper";

PortAudioWrapper::PortAudioWrapper()
    : m_paInputStream{nullptr},
      m_paOutputStream{nullptr},
      m_inputCallbackInterface{nullptr},
      m_outputCallbackInterface{nullptr} {
    BasicLogger::getInstance().log(TAG, LogLevel::INFO,
                                   "Initializing PortAudio library");
    PaError paStatus = Pa_Initialize();
    if (paStatus != paNoError) {
        std::string errorMsg = std::string("Failed to initialize PortAudio. ") +
                               Pa_GetErrorText(paStatus);
        BasicLogger::getInstance().log(TAG, LogLevel::ERROR, errorMsg);

        throw BaseException(errorMsg);
    }
}

PortAudioWrapper::~PortAudioWrapper() {
    Pa_StopStream(m_paInputStream);
    Pa_CloseStream(m_paInputStream);
    Pa_Terminate();
}

void PortAudioWrapper::addStream(const PortAudioWrapperConfig& config) {
    BasicLogger::getInstance().log(
        TAG, LogLevel::DEBUG,
        std::string("Adding PortAudio library stream") + " | IOType " +
            std::to_string(
                static_cast<std::underlying_type<IOType>::type>(config.type)) +
            " | sample rate " + std::to_string(config.sampleRate) +
            " | sample size " + std::to_string(config.bitsPerSample) +
            " | number of channels " + std::to_string(config.numChannels));

    std::lock_guard<std::mutex> lock(m_portAudioMtx);
    // Prepare stream parameters
    PaStreamParameters streamParameters;
    std::memset(&streamParameters, 0, sizeof(streamParameters));

    PaError paStatus;

    switch (config.type) {
        case IOType::INPUT:
            streamParameters.device = Pa_GetDefaultInputDevice();
            streamParameters.channelCount = config.numChannels;
            streamParameters.sampleFormat = paInt16;
            streamParameters.suggestedLatency =
                Pa_GetDeviceInfo(streamParameters.device)
                    ->defaultLowInputLatency;
            streamParameters.hostApiSpecificStreamInfo = nullptr;
            m_inputCallbackInterface = config.inputCallbackInterface;
            break;
        case IOType::OUTPUT:
            streamParameters.device = Pa_GetDefaultOutputDevice();
            streamParameters.channelCount = config.numChannels;
            streamParameters.sampleFormat = paInt16;
            streamParameters.suggestedLatency =
                Pa_GetDeviceInfo(streamParameters.device)
                    ->defaultLowOutputLatency;
            streamParameters.hostApiSpecificStreamInfo = nullptr;
            m_outputCallbackInterface = config.outputCallbackInterface;
            break;
        default:
            std::string errorMsg = "Failed to add stream. Invalid IOType";
            BasicLogger::getInstance().log(TAG, LogLevel::ERROR, errorMsg);

            throw BaseException(errorMsg);
    }

    // Ok, open stream
    if (IOType::INPUT == config.type) {
        paStatus =
            Pa_OpenStream(&(m_paInputStream), &streamParameters, nullptr,
                          config.sampleRate, paFramesPerBufferUnspecified,
                          paNoFlag, portAudioInputCallback, this);
    } else if (IOType::OUTPUT == config.type) {
        paStatus = Pa_OpenStream(&(m_paOutputStream), nullptr,
                                 &streamParameters, config.sampleRate, 4096,
                                 paNoFlag, portAudioOutputCallback, this);
    }

    if (paStatus != paNoError) {
        std::string errorMsg = "Failed to open PortAudio stream.";
        BasicLogger::getInstance().log(TAG, LogLevel::ERROR,
                                       errorMsg + Pa_GetErrorText(paStatus));

        throw BaseException(errorMsg);
    }
}

void PortAudioWrapper::startStream(const IOType& type) {
    std::lock_guard<std::mutex> lock(m_portAudioMtx);
    PaError paStatus;

    switch (type) {
        case IOType::INPUT:
            paStatus = Pa_StartStream(m_paInputStream);
            break;
        case IOType::OUTPUT:
            paStatus = Pa_StartStream(m_paOutputStream);
            break;

        default:
            std::string errorMsg = "Failed to start stream. Invalid IOType";
            BasicLogger::getInstance().log(TAG, LogLevel::ERROR, errorMsg);

            throw BaseException(errorMsg);
            break;
    }
    if (paStatus != paNoError) {
        std::string errorMsg =
            std::string("Failed to start stream.") + Pa_GetErrorText(paStatus);
        BasicLogger::getInstance().log(TAG, LogLevel::ERROR, errorMsg);
        throw BaseException(errorMsg);
    }
}

void PortAudioWrapper::stopStream(const IOType& type) {
    std::lock_guard<std::mutex> lock(m_portAudioMtx);
    PaError paStatus;

    switch (type) {
        case IOType::INPUT:
            paStatus = Pa_StopStream(m_paInputStream);
            break;
        case IOType::OUTPUT:
            paStatus = Pa_StopStream(m_paOutputStream);
            break;

        default:
            std::string errorMsg = "Failed to stop stream. Invalid IOType";
            BasicLogger::getInstance().log(TAG, LogLevel::ERROR, errorMsg);

            throw BaseException(errorMsg);
            break;
    }
    if (paStatus != paNoError) {
        std::string errorMsg = std::string("Failed to stop PortAudio stream.") +
                               Pa_GetErrorText(paStatus);
        BasicLogger::getInstance().log(TAG, LogLevel::ERROR, errorMsg);
        throw BaseException(errorMsg);
    }
}

int PortAudioWrapper::portAudioInputCallback(
    const void* inputBuffer,
    void* outputBuffer,
    unsigned long numSamples,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData) {
    auto paWrapper = static_cast<PortAudioWrapper*>(userData);

    if (paWrapper->m_inputCallbackInterface != nullptr) {
        paWrapper->m_inputCallbackInterface(inputBuffer, numSamples);
    }
    return paContinue;
}

int PortAudioWrapper::portAudioOutputCallback(
    const void* inputBuffer,
    void* outputBuffer,
    unsigned long numSamples,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData) {
    auto paWrapper = static_cast<PortAudioWrapper*>(userData);

    if (paWrapper->m_outputCallbackInterface != nullptr) {
        paWrapper->m_outputCallbackInterface(outputBuffer, numSamples);
    }
    return paContinue;
}

}  // namespace PortAudio
}  // namespace Audio
