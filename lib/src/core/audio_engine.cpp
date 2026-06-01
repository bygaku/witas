/**
 * Created by intwi on 2026/05/30.
 * Copyright (c) 2026 All rights reserved.
 */

#include "audio_engine.h"

#include <cstdio>
#include <atomic>
#include <cstring>
#include <unordered_map>

#include "audio_data.h"
#include "load_wav.h"
#include "sound_handle.h"

#include "miniaudio.h"

namespace wit {
/**
 * @brief
 * @param device
 * @param output
 * @param input 'WIT' don't need it.
 * @param frame_count
 */
extern "C" {
static void WitDataCallback(ma_device* device, void* output,
							const void* input, ma_uint32 frame_count);
};

/** AudioEngine::Impl */
struct AudioEngine::Impl {
	/**
	 */
	Impl() = default;

	/**
	 */
	~Impl() = default;

	/** TODO */

	/**
	 */
	void OnAudioCallback(float* output, ma_uint32 frame_count) {
		std::memset(output, 0, static_cast<std::size_t>(frame_count) * 2 * sizeof(float));
	}

	ma_device device_{};						///< Audio stream
	std::atomic<bool> is_started_ = {false};	///< Is the audio thread started?

	std::unordered_map<uint32_t, AudioData> audio_data_map_;	///< AudioData register
	uint32_t next_audio_data_id_ = 0;	///< 0 is an invalid id
};

extern "C" {
/**
 * @brief
 * @param device
 * @param output
 * @param input 'WIT' don't need it.
 * @param frame_count
 */
static void WitDataCallback(ma_device* device, void* output,
							const void* input, ma_uint32 frame_count) {
	(void)input;	///< 'WIT' don't need it.
	auto* impl = static_cast<AudioEngine::Impl*>(device->pUserData);
	if (impl == nullptr) {
		std::memset(output, 0, static_cast<std::size_t>(frame_count) * 2 * sizeof(float));
		return;
	}
	impl->OnAudioCallback(static_cast<float*>(output), frame_count);
}
}

/** AudioEngine */
AudioEngine::AudioEngine()
	: impl_(std::make_unique<Impl>()) {
}

AudioEngine::~AudioEngine() {
	Stop();
}

bool AudioEngine::Start() {
	// The device has already started
	if (impl_->is_started_.load(std::memory_order_acquire)) {
		return true;
	}

	ma_device_config config		= ma_device_config_init(ma_device_type_playback);
	config.playback.format		= ma_format_f32;  ///< Wit uses float32 internally
	config.playback.channels	= 2;              ///< stereo
	config.sampleRate			= 48000;          ///< 48kHz
	config.dataCallback			= &WitDataCallback;
	config.pUserData			= impl_.get();    ///< Retrieving an 'Impl' in a callback

	// Initialize device
	if (ma_device_init(nullptr, &config, &impl_->device_) != MA_SUCCESS) {
		std::fprintf(stderr, "[wit] AudioEngine::Start() ma_device_init failed\n");
		return false;
	}

	// Start the device. If this fails, discard the initialized device and roll back
	if (ma_device_start(&impl_->device_) != MA_SUCCESS) {
		std::fprintf(stderr, "[wit] AudioEngine::Start() ma_device_start failed\n");
		ma_device_uninit(&impl_->device_);
		return false;
	}

	impl_->is_started_.store(true, std::memory_order_release);
	return true;
}

void AudioEngine::Stop() {
	if (!impl_->is_started_.load(std::memory_order_acquire)) {
		return;
	}

	// Stop the audio thread and call uninit
	ma_device_stop(&impl_->device_);
	ma_device_uninit(&impl_->device_);

	impl_->is_started_.store(false, std::memory_order_release);
}

std::optional<SoundHandle> AudioEngine::LoadSound(const char* file_path) {
	auto audio = LoadWav(file_path);

	if (!audio.has_value()) {
		return std::nullopt;
	}

	return Register(std::move(*audio));
}

SoundHandle AudioEngine::Register(AudioData&& audio) {
	auto id = impl_->next_audio_data_id_++;
	impl_->audio_data_map_[id] = std::move(audio);

	SoundHandle handle;
	handle.id_ = id;
	
	return handle;
}

void AudioEngine::Play(SoundHandle handle) {
	(void)handle;
}

void AudioEngine::Unload(SoundHandle handle) {
	(void)handle;
}

}
