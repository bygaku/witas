/**
 * Created by intwi on 2026/05/29.
 * Copyright (c) 2026 All rights reserved.
 */

#ifndef WIT_SOUND_HANDLE_H
#define WIT_SOUND_HANDLE_H

#include <cstdint>

namespace wit {
/**
 * @class SoundHandle
 * @brief
 *
 * @code
 * ^
 * @endcode
 */
class SoundHandle {
	friend class AudioEngine;
public:
	/**
	 */
	SoundHandle() = default;

	/**
	 * @brief Check whether SoundHandle is enabled.
	 */
	bool IsValid() const { return id_ != 0; }

private:
	std::uint32_t id_ = 0;		///< 0 is an invalid handle.

};

}

#endif //WIT_SOUND_HANDLE_H
