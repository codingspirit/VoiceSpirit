#pragma once

#include "SharedDataStream.h"

namespace Audio {
using AudioInputStreamSize = int16_t;
using AudioOutputStreamSize = int16_t;
using AudioInputStream =
    Utils::DataStructures::SharedDataStream<AudioInputStreamSize>;
using AudioOutputStream =
    Utils::DataStructures::SharedDataStream<AudioOutputStreamSize>;
}  // namespace Audio
