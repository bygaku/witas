/**
 * Created by intwi on 2026/05/28.
 * Copyright (c) 2026 All rights reserved.
 */

#include <cstdio>
#include <fstream>
#include <map>

#include "four_cc.h"
#include "endian_converter.h"
#include "load_wav.h"

namespace wit {

namespace {
constexpr CHUNK_ID RIFF_ID = Chunk_ID('R', 'I', 'F', 'F');	///< 'F' 'F' 'I' 'R'
constexpr CHUNK_ID WAVE_ID = Chunk_ID('W', 'A', 'V', 'E');	///< 'E' 'V' 'A' 'W'
constexpr CHUNK_ID FMT_ID  = Chunk_ID('f', 'm', 't', ' ');	///< ' ' 't' 'm' 'f'
constexpr CHUNK_ID LIST_ID = Chunk_ID('L', 'I', 'S', 'T');	///< 'T' 'S' 'I' 'L'
constexpr CHUNK_ID DATA_ID = Chunk_ID('d', 'a', 't', 'a');	///< 'a' 't' 'a' 'd'

// ---- Supported WAV file format ---------------------------------------------- ver1.0.0
constexpr std::uint16_t FORMAT_TYPE_PCM			= 1;		///< Linear PCM

constexpr std::uint16_t REQUIRED_CHANNELS		= 2;		///< stereo only
constexpr std::uint16_t REQUIRED_BIT_DEPTH		= 16;		///< 16bit only
constexpr std::uint32_t REQUIRED_SAMPLE_RATE	= 48000;	///< 48kHz only

constexpr std::size_t	RIFF_HEADER_SIZE		= 12;		///< 'RIFF' + size(4) + 'WAVE'
constexpr std::size_t	CHUNK_HEADER_SIZE		= 8;		///< Size of sub chunk header 'id(4) + size(4)'

#pragma region Unsupported WAV file format - /** @version ver2.0.0 or later */
/**
constexpr std::uint16_t FORMAT_TYPE_FLOAT32		= 3;		///< 32bit float
*/
#pragma endregion

/**
 * @struct FmtChunk
 * @brief Holds wav file metadata from FmtChunk.
 */
struct FmtChunk {
	uint16_t	format_type;	///< IEEE, linear PCM.
	uint16_t	num_channels;	///< The number of channels.
	uint32_t	sample_rate;	///< Samples per second.
	uint32_t	byte_rate;		///< Bytes per second.
	uint16_t	block_size;		///< 'channel_num * bit depth / 8'
	uint16_t	bit_depth;		///< Bits per sample.
};

/**
 * The fmt chunk body is exactly 16 bytes in the WAV spec. If the compiler
 * adds padding, reading sizeof(FmtChunk) bytes from the file would misalign.
 * This assertion guarantees the struct matches the on-disk layout.
 */
static_assert(sizeof(FmtChunk) == 16, "FmtChunk must be 16 bytes (no padding)");

/**
 * @brief The file path and error details are output to the console.
 * @param path WAV file path
 * @param reason Error details
 */
void ErrorLog(const char* path, const char* reason) {
	std::fprintf(stderr, "[wit] Wav file load failed: %s (path: %s)\n",
				 reason, path ? path : "(null)");
}

}

std::optional<AudioData> LoadWav(const char* file_path) {
	// Path is null
	if (file_path == nullptr) {
		ErrorLog(file_path, "Path is null");
		return std::nullopt;
	}

	// File open
	std::ifstream fp(file_path, std::ios::binary);
	if (!fp.is_open()) {	///< File open failed
		ErrorLog(file_path, "File not found");
		return std::nullopt;
	}

	// Get a file size
	fp.seekg(0, std::ios::end);	///< Move read pointer to end
	std::size_t file_size = fp.tellg();
	fp.seekg(0, std::ios::beg); ///< Move read pointer to begin

	// The file is incomplete
	if (file_size < static_cast<std::streamoff>(RIFF_HEADER_SIZE)) {
		ErrorLog(file_path, "File size is too small");
		return std::nullopt;
	}

	FmtChunk									fmt_chunk{};
	std::map<CHUNK_ID, std::vector<CHUNK_SIZE>> chunk_map{};
	std::vector<float>							samples_out;
	std::uint32_t								frame_count_out = 0;

	// Reading a file sequentially
	// Loop while a full chunk header (id + size = 8 bytes) can still be read
	while (fp.tellg() >= 0 && fp.tellg() + static_cast<std::streamoff>(CHUNK_HEADER_SIZE) <= file_size) {
		CHUNK_ID chunk_id{};
		fp.read(reinterpret_cast<char*>(&chunk_id), sizeof(CHUNK_ID));

		CHUNK_SIZE chunk_size{};
		fp.read(reinterpret_cast<char*>(&chunk_size), sizeof(CHUNK_SIZE));

		// If either read failed, stop (truncated header)
		if (!fp) break;

		switch (chunk_id) {
			case RIFF_ID: {
				///< Check whether the string following ‘RIFF’ + CHUNK_SIZE is 'WAVE'
				CHUNK_ID wave_id{};
				fp.read(reinterpret_cast<char*>(&wave_id), sizeof(CHUNK_ID));

				if (wave_id != WAVE_ID) {
					ErrorLog(file_path, "This isn't a WAV file");
					return std::nullopt;
				}
				break;
			}
			case FMT_ID: {
				fp.read(reinterpret_cast<char*>(&fmt_chunk), sizeof(FmtChunk));

				// Skip the extension
				if (chunk_size > sizeof(FmtChunk)) {
case FMT_ID: {
    if (chunk_size < sizeof(FmtChunk)) {
        ErrorLog(file_path, "FMT chunk is too small");
        return std::nullopt;
    }
    fp.read(reinterpret_cast<char*>(&fmt_chunk), sizeof(FmtChunk));
    if (chunk_size > sizeof(FmtChunk)) {
        fp.seekg(static_cast<std::size_t>(chunk_size) - sizeof(FmtChunk), std::ios::cur);
    }
    break;
}
				}
				break;
			}
			case DATA_ID: {
				// fmt chunk must have been read before data (standard WAV layout)
				// If num_channels is still empty, the fmt chunk is missing or came after data
				if (!fmt_chunk.num_channels) {
					ErrorLog(file_path, "'data' chunk appeared before a valid 'fmt ' chunk");
					return std::nullopt;
				}

				if (fmt_chunk.format_type != FORMAT_TYPE_PCM ||
					fmt_chunk.bit_depth   != REQUIRED_BIT_DEPTH) {
					ErrorLog(file_path, "Unsupported format (only 16bit linear PCM is supported in this version)");
					return std::nullopt;
				}
				if (fmt_chunk.num_channels != REQUIRED_CHANNELS) {
					ErrorLog(file_path, "Unsupported channel count (only stereo is supported in this version)");
					return std::nullopt;
				}
				if (fmt_chunk.sample_rate != REQUIRED_SAMPLE_RATE) {
					ErrorLog(file_path, "Unsupported sample rate (only 48000 Hz is supported in this version)");
					return std::nullopt;
				}

				/** The declared chunk_size must not exceed the bytes actually remaining in the file.
					Validate before allocating.*/
				const std::streamoff current_pos = fp.tellg();
				if (current_pos < 0 || static_cast<std::streamoff>(chunk_size) > file_size - current_pos) {
					ErrorLog(file_path, "'data' chunk size exceeds file bounds (corrupt or truncated)");
					return std::nullopt;
				}

				// Calculate the total number of samples
				const std::size_t bytes_per_sample = fmt_chunk.bit_depth / 8;
				const std::size_t bytes_per_frame  = bytes_per_sample * fmt_chunk.num_channels;
				const std::size_t total_samples    = chunk_size / bytes_per_sample;	///< All channels total

				if (chunk_size % bytes_per_frame != 0) {
					ErrorLog(file_path, "'data' chunk size is not a multiple of frame size (corrupt or truncated)");
					return std::nullopt;
				}

				const std::uint32_t channels    = fmt_chunk.num_channels;
				const auto			frame_count = static_cast<std::uint32_t>(total_samples / channels);

				// Read the interleaved 16bit samples (LRLRLR...) into a temporary buffer
				std::vector<int16_t> tmp_buffer(total_samples);
				fp.read(reinterpret_cast<char*>(tmp_buffer.data()), chunk_size);

				// De-interleave: LRLR... -> planar (LLL...RRR...) directly into samples_out
				samples_out.resize(total_samples);
				for (std::uint32_t frame = 0; frame < frame_count; ++frame) {
					for (std::uint32_t ch = 0; ch < channels; ++ch) {
						samples_out[ch * frame_count + frame] =
							static_cast<float>(tmp_buffer[frame * channels + ch]) / 32768.0f;
					}
				}

				frame_count_out = frame_count;
				break;
			}
			default: {
				///< (e.g., JUNK chunk, inst chunk)
				// Save the current position (right after the size field)
				auto current_pos = fp.tellg();
				chunk_map[chunk_id].push_back(
					static_cast<uint32_t>(current_pos) - sizeof(uint32_t)
				);

				// Skip the data section
				fp.seekg(chunk_size, std::ios::cur);
				break;
			}
		}
	}

	if (samples_out.empty()) {
		ErrorLog(file_path, "'data' chunk not found or empty");
		return std::nullopt;
	}

	AudioData result{};
	result.sample_rate	= fmt_chunk.sample_rate;
	result.num_channels	= fmt_chunk.num_channels;
	result.frame_count	= frame_count_out;
	result.samples		= std::move(samples_out);
	return result;
}

}