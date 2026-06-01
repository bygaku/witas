/**
 * Created by intwi on 2026/05/29.
 * Copyright (c) 2026 All rights reserved.
 */

#ifndef WIT_AUDIO_ENGINE_H
#define WIT_AUDIO_ENGINE_H

#include <memory>
#include <optional>

#include "audio_data.h"

namespace wit {

class SoundHandle;

/**
 * @class AudioEngine
 * @brief
 *
 */
class AudioEngine {
public:
	/**
	 */
	AudioEngine();

#pragma region /** Disable copy and move this class */
	AudioEngine(const AudioEngine&)            = delete;
	AudioEngine& operator=(const AudioEngine&) = delete;
	AudioEngine(AudioEngine&&)                 = delete;
	AudioEngine& operator=(AudioEngine&&)      = delete;
#pragma endregion

	/**
	 */
	~AudioEngine();

	/**
	 */
	bool Start();

	/**
	 */
	void Stop();

	/**
	 */
	std::optional<SoundHandle> LoadSound(const char* file_path);

	/**
	 */
	SoundHandle Register(AudioData&& audio);

	/**
	 * Play the sound from 'SoundHandle'.
	 * @param handle
	 */
	void Play(SoundHandle handle);

	/**
	 * Release 'SoundHandle' from memory.
	 * @param handle
	 * @note A sound handle that is currently playing will be unloaded after it stops.
	 */
	void UnloadSound(SoundHandle handle);

	/**
	 */
	struct Impl;	///< Implemented in audio_engine.cpp

private:
	std::unique_ptr<Impl> impl_;	///< Add implementation not exposed to users

};

}

#endif //WIT_AUDIO_ENGINE_H
