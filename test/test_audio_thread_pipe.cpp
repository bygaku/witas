/**
 * Created by intwi on 2026/05/30.
 * Copyright (c) 2026 All rights reserved.
 */

/**
 * @brief miniaudio's Audio thread test.
 * Target OS: Windows
 */

/**
 * Contracts to be verified:
 *   1. Does not crash during default initialization or destruction (no need to call `Start`)
 *   2. Calling `Start` returns a boolean and control is returned (success or failure depends on the environment)
 *   3. Calling `Stop` without first calling `Start` is safe (idempotent)
 *   4. Calling `Start` twice in succession is safe (idempotent; the second call inherits the result of the previous one)
 *   5. Calling `Stop` twice in succession is safe (idempotent)
 *   6. The destructor implicitly calls `Stop`
 */

#include "audio_engine.h"

#include <cstdio>

namespace {
int g_pass = 0;
int g_fail = 0;
void Check(bool cond, const char* label) {
	if (cond) {
		std::printf("  [PASS] %s\n", label);
		++g_pass;
	} else {
		std::printf("  [FAIL] %s\n", label);
		++g_fail;
	}
}
}

int main() {
	std::printf("=== AudioEngine lifecycle test ===\n\n");

	std::printf("Test 1: construct and destruct without Start\n");
	{
		wit::AudioEngine engine;
		Check(true, "no crash on construct + destruct");
	}

	std::printf("\nTest 2: Stop without Start (idempotent)\n");
	{
		wit::AudioEngine engine;
		engine.Stop();  ///< Nothing to do here
		Check(true, "no crash on Stop without Start");
	}

	std::printf("\nTest 3: Start returns and Stop after it\n");
	{
		wit::AudioEngine engine;
		bool is_started = engine.Start();
		std::printf("    Start() returned: %s\n", is_started ? "true" : "false");
		
		Check(true, "Start() returned without crashing");
		engine.Stop();
		Check(true, "Stop() after Start() returned without crashing");
	}

	std::printf("\nTest 4: Start twice is idempotent\n");
	{
		wit::AudioEngine engine;
		bool fst = engine.Start();
		bool sec = engine.Start();
		Check(true, "Start() x 2 did not crash");

		if (fst) {
			Check(sec, "2nd Start returns true when 1st succeeded");
		} else {
			std::printf("    (1st Start failed -- environment without audio device)\n");
		}
		engine.Stop();
	}

	std::printf("\nTest 5: Stop twice is idempotent\n");
	{
		wit::AudioEngine engine;
		engine.Start();
		engine.Stop();
		engine.Stop();  ///< Nothing to do here
		Check(true, "Stop() x 2 did not crash");
	}

	std::printf("\nTest 6: destructor implicitly stops\n");
	{
		{
			wit::AudioEngine engine;
			engine.Start();
			/** The destructor calls Stop(). */
		}
		Check(true, "destructor without explicit Stop did not crash");
	}

	std::printf("\n=== Result: %d passed, %d failed ===\n", g_pass, g_fail);
	return g_fail == 0 ? 0 : 1;
}