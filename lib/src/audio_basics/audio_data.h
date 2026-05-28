/**
 * Created by wistd on 2026/05/28.
 * Copyright (c) 2026 All rights reserved.
 */

#ifndef WIT_AUDIO_DATA_H
#define WIT_AUDIO_DATA_H
#include <cstdint>
#include <vector>

namespace wit {
    /**
     * @struct AudioData
     * @brief Holds audio metadata and buffer data.
     * @note The waveform data (`samples`) is stored in a non-interleaved
     */
    struct AudioData {
        std::uint32_t       sample_rate;    ///< Sampling rate in Hz (e.g., 44100, 48000).
        std::uint32_t       num_channels;   ///< Number of audio channels.
        std::uint32_t       frame_count;    ///< Number of audio frames (samples per channel).
        std::vector<float>  samples;        ///< Contiguous sample buffer. Total size is 'num_channels * frame_count'.
    };

    namespace channels {
        constexpr std::uint32_t LEFT             = 0;   ///< Left channel (Stereo / Surround)
        constexpr std::uint32_t RIGHT            = 1;   ///< Right channel (Stereo / Surround)

        /** === Reserved for 5.1ch Surround (will be supported by ver2.0.0 or later) === */
        constexpr std::uint32_t CENTER           = 2;   ///< Center channel @version ver2.0.0
        constexpr std::uint32_t LFE              = 3;   ///< Subwoofer @version ver2.0.0
        constexpr std::uint32_t LEFT_SURROUND    = 4;   ///< Left surround channel @version ver2.0.0
        constexpr std::uint32_t RIGHT_SURROUND   = 5;   ///< Right surround channel @version ver2.0.0
    }

    /**
     * @brief Gets a pointer to the beginning of the sample buffer for a specific channel.
     * @param[in] audio Target AudioData structure.
     * @param[in] ch    The zero-based channel index.
     * @return float* Pointer to the first sample of the specified channel.
     *
     * @note Provides easy access to any channel via the constants in the channels namespace.
     * @attention If the specified ch is out of range, it is automatically set to num_channel.
     */
    inline float* Channel(AudioData& audio, std::uint32_t ch) {
        if (ch > audio.num_channels) ch = audio.num_channels;
        return audio.samples.data() + static_cast<std::size_t>(ch) * audio.frame_count;
    }

    /**
     * @brief Gets a read-only pointer to the beginning of the sample buffer for a specific channel.
     * @param[in] audio Target AudioData structure.
     * @param[in] ch    The zero-based channel index.
     * @return const float* Read-only pointer to the first sample of the specified channel.
     *
     * @note Provides easy access to any channel via the constants in the channels namespace.
     * @attention If the specified ch is out of range, it is automatically set to num_channel.
     */
    inline const float* Channel(const AudioData& audio, std::uint32_t ch) {
        if (ch > audio.num_channels) ch = audio.num_channels;
        return audio.samples.data() + static_cast<std::size_t>(ch) * audio.frame_count;
    }

}

#endif //WIT_AUDIO_DATA_H
