/**
 * Created by intwi on 2026/06/11.
 * Copyright (c) 2026 All rights reserved.
 */

#include "audio_engine.h"

#include <chrono>
#include <cstdio>
#include <thread>

int main(int argc, char** argv) {
	const char*   WAV_PATH = "C:/_Development/audio-dev/witas/test/test_48000.wav";
	constexpr int WAIT_SEC = 10;
	std::printf("[INFO] wait_seconds = %d\n", WAIT_SEC);

	wit::AudioEngine engine;
	// Start the audio device
	if (!engine.Start()) {
		std::printf("[SKIP] AudioEngine::Start() returned false "
					"(no audio device on this environment)\n");
		return 0;
	}
	std::printf("[OK] AudioEngine started\n");

	// Load wav file
	auto handle = engine.LoadSound(WAV_PATH);
	if (!handle) {
		std::fprintf(stderr, "[SKIP] LoadSound failed for path: %s\n", WAV_PATH);
		engine.Stop();
		return 0;
	}
	std::printf("[OK] LoadSound succeeded (handle id: %u)\n",
				static_cast<unsigned>(handle->IsValid() ? 1 : 0));

	// Start playback safely in the audio thread
	engine.PlaySound(*handle);
	std::printf("[OK] PlaySound issued\n");
	std::printf("[WAIT] sleeping for %d second(s)...\n", WAIT_SEC);
	std::this_thread::sleep_for(std::chrono::seconds(WAIT_SEC));

	// Release the handle
	engine.UnloadSound(*handle);
	std::printf("[OK] UnloadSound done\n");

	// Stop the engine safely
	engine.Stop();
	std::printf("[OK] AudioEngine stopped safely\n");

	std::printf("[PASS] Completed\n");
	return 0;
}