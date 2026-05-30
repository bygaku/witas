#include <cstdio>
#include <cstring>

#include "miniaudio.h"

int main() {
	const char* version = ma_version_string();

	if (version == nullptr) {
		std::printf("[FAIL] ma_version_string() returned nullptr\n");
		return 1;
	}
	if (std::strlen(version) == 0) {
		std::printf("[FAIL] ma_version_string() returned empty string\n");
		return 1;
	}

	std::printf("[PASS] miniaudio linked successfully. Version: %s\n", version);

	const char* expected = "0.11.25";
	if (std::strcmp(version, expected) != 0) {
		std::printf("[WARN] version mismatch. expected=%s, actual=%s\n",
					expected, version);
	}

	return 0;
}