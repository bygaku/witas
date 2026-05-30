/**
 * Created by intwi on 2026/05/28.
 * Copyright (c) 2026 All rights reserved.
 */

#ifndef WIT_LOAD_WAV_H
#define WIT_LOAD_WAV_H

#include <optional>

#include "../audio_basics/audio_data.h"

namespace wit {
/**
 * @brief Load the WAV file into memory.
 * @param file_path WAV file path.
 * @return If the WAV file fails to load, std::nullopt is returned.
 */
std::optional<AudioData> LoadWav(const char* file_path);
}
#endif //WIT_LOAD_WAV_H
