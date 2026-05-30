/**
 * Created by intwi on 2026/05/30.
 * Copyright (c) 2026 All rights reserved.
 */

#include "audio_engine.h"

#include <cstdio>

#include "sound_handle.h"
#include "miniaudio.h"

namespace wit {

/** AudioEngine::Impl */
class AudioEngine::Impl {
public:
	Impl() = default;
	~Impl() = default;

	/** TODO */
};

/** AudioEngine */
AudioEngine::AudioEngine()
	: impl_(std::make_unique<Impl>()) {
}

AudioEngine::~AudioEngine() {
	if (true /** TODO */ ) {
		Stop();	///< If AudioEngine::Stop() has already been called in the user code, skip this step
	}
}

bool AudioEngine::Start() {
	std::fprintf(stderr, "[wit] AudioEngine::Start() not yet implemented (stage 1)\n");
	return false;
}

void AudioEngine::Stop() {
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
