/**
 * Created by intwi on 2026/05/30.
 * Copyright (c) 2026 All rights reserved.
 */

#include "audio_engine.h"

#include <cstdio>
#include <atomic>
#include <cstring>

#include "sound_handle.h"
#include "miniaudio.h"

namespace wit {
/**
 * @brief
 * @param device
 * @param output
 * @param input
 * @param frame_count
 */
extern "C" {
static void WitDataCallback(ma_device* device, void* output,
							const void* input, ma_uint32 frame_count);
};

/** AudioEngine::Impl */
struct AudioEngine::Impl {
	Impl() = default;
	~Impl() = default;

	/** TODO */
	ma_device device_{};

	std::atomic<bool> is_started_ = {false};
	std::atomic<bool> is_stopped_ = {false};

	void OnAudioCallback(float* output, ma_uint32 frame_count) {
		// 段階2: 無音を出力する。
		// ma_device_config で channels=2 / format=f32 を指定するため、
		// 出力バッファは「frame_count フレーム × 2ch × float32」=
		// frame_count * 2 * sizeof(float) バイト分。
		std::memset(output, 0, static_cast<std::size_t>(frame_count) * 2 * sizeof(float));
	}
};

extern "C" {
static void WitDataCallback(ma_device* device, void* output,
							const void* input, ma_uint32 frame_count) {
	(void)input;
	auto* impl = static_cast<AudioEngine::Impl*>(device->pUserData);
	if (impl == nullptr) {
		// 通常起きないが、防御的に: pUserData 未設定でコールバックが呼ばれた
		// 場合は出力バッファをゼロ埋めしてリターン (グリッチを最小化)。
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

std::optional<SoundHandle> AudioEngine::LoadSound(const char* path) {
	(void)path;
	std::fprintf(stderr, "[wit] AudioEngine::LoadSound() not yet implemented (stage 1)\n");
	return std::nullopt;
}

SoundHandle AudioEngine::Register(AudioData&& audio) {
	(void)audio;
	std::fprintf(stderr, "[wit] AudioEngine::Register() not yet implemented (stage 1)\n");
	return SoundHandle{};
}

void AudioEngine::Play(SoundHandle handle) {
	(void)handle;
}

void AudioEngine::Unload(SoundHandle handle) {
	(void)handle;
}

}
