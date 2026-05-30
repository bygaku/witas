/**
 * Created by intwi on 2026/05/28.
 * Copyright (c) 2026 All rights reserved.
 */

#ifndef WIT_FOUR_CC_H
#define WIT_FOUR_CC_H

#include <cstdint>

namespace wit {
typedef uint32_t CHUNK_ID;		///< FourCC
typedef uint32_t CHUNK_SIZE;	///< after FourCC

/**
 * @brief creates a new 4-character code from four characters.
 * @code
 *  constexpr CHUNK_ID YOU_ID = CHUNK_ID('Y', 'O', 'U', ' ');
 * @endcode
*/
#define Chunk_ID(a, b, c, d) (a + (b << 8) + (c << 16) + (d << 24))

}

#endif //WIT_FOUR_CC_H
