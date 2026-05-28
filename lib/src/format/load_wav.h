/**
 * Created by intwi on 2026/05/28.
 * Copyright (c) 2026 All rights reserved.
 */

#ifndef WIT_LOAD_WAV_H
#define WIT_LOAD_WAV_H
#include <optional>

#include "../audio_basics/audio_data.h"

namespace wit {

std::optional<AudioData> LoadWav(const char* file_path);
}
#endif //WIT_LOAD_WAV_H
