/**
 * Created by intwi on 2026/05/28.
 * Copyright (c) 2026 All rights reserved.
 */

#ifndef WIT_ENDIAN_CONVERTER_H
#define WIT_ENDIAN_CONVERTER_H

#include <cstdint>

namespace wit {
/**
 * @brief swaps the byte order of a 32-bit data value.
 * @param data reference to the 32-bit data whose byte order will be swapped.
 */
inline void Swap32Bit(uint32_t& data) {
	data = ((data & 0x000000FF) << 24)
		 + ((data & 0x0000FF00) << 8)
		 + ((data & 0x00FF0000) >> 8)
		 + ((data & 0xFF000000) >> 24);
}

/**
 * @brief swaps the byte order of a 16-bit data value.
 * @param data reference to the 16-bit data whose byte order will be swapped.
 */
inline void Swap16Bit(uint16_t& data) {
	data = ((data & 0x00FF) << 8)
		 + ((data & 0xFF00) >> 8);
}
}


#endif //WIT_ENDIAN_CONVERTER_H
