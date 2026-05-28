#include <iostream>
#include <cassert>
#include <cmath>
#include "../lib/src/audio_basics/audio_data.h"

bool is_close(float a, float b, float epsilon = 1e-5f) {
    return std::fabs(a - b) < epsilon;
}

int main() {
    std::cout << "[RUN] AudioDataTest.ChannelPointerOffsets" << std::endl;

    wit::AudioData audio;
    audio.sample_rate  = 48000;
    audio.num_channels = 2;
    audio.frame_count  = 3;
    audio.samples      = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f};

    float* left_ch = wit::Channel(audio, wit::channels::LEFT);
    if (left_ch != audio.samples.data()) {
        std::cerr << "FAIL: Left channel pointer mismatch!" << std::endl;
        return 1;
    }
    if (!is_close(left_ch[0], 0.1f) || !is_close(left_ch[2], 0.3f)) {
        std::cerr << "FAIL: Left channel data mismatch!" << std::endl;
        return 1;
    }

    float* right_ch = wit::Channel(audio, wit::channels::RIGHT);
    if (right_ch != audio.samples.data() + audio.frame_count) {
        std::cerr << "FAIL: Right channel pointer mismatch!" << std::endl;
        return 1;
    }
    if (!is_close(right_ch[0], 0.4f) || !is_close(right_ch[2], 0.6f)) {
        std::cerr << "FAIL: Right channel data mismatch!" << std::endl;
        return 1;
    }

    const wit::AudioData& const_audio = audio;
    const float* const_left_ch = wit::Channel(const_audio, wit::channels::LEFT);
    if (!is_close(const_left_ch[0], 0.1f)) {
        std::cerr << "FAIL: Const left channel data mismatch!" << std::endl;
        return 1;
    }

    std::cout << "[SUCCESS] AudioDataTest Passed!" << std::endl;
    return 0;
}