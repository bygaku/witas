/**
 * Created by intwi on 2026/06/06.
 * Copyright (c) 2026 All rights reserved.
 */

#ifndef WIT_RING_BUFFER_H
#define WIT_RING_BUFFER_H

#include <atomic>
#include <cassert>
#include <cstdint>
#include <new>
#include <type_traits>

namespace wit {

/**
 * @class RingBuffer
 * @tparam ItemType Type to add to the buffer. Must be trivially copyable.
 * @brief
 *
 */
template<typename ItemType, std::uint32_t Capacity>
class RingBuffer {
public:
	#pragma region /** To improve performance */
	static_assert((Capacity & (Capacity - 1)) == 0,
			  "Capacity must be a power of two");

	static_assert(std::is_trivially_copyable_v<ItemType>,
			  "ItemType must be trivially copyable");
	#pragma endregion

	#pragma region /** Disable copy and move this class */
	RingBuffer(const RingBuffer&)            = delete;
	RingBuffer& operator=(const RingBuffer&) = delete;
	RingBuffer(RingBuffer&&)                 = delete;
	RingBuffer& operator=(RingBuffer&&)      = delete;
	#pragma endregion

	RingBuffer() = default;

	 /**
	  * @brief Adds a single item to the buffer.
      * @param[in] item The item to add (pass-by-value, copied internally).
	  * @return true: successful / false: The buffer was full.
	  */
	bool Enqueue(const ItemType& item) {
		const std::uint32_t write = write_idx_.load(std::memory_order_relaxed);
		const std::uint32_t read  = read_idx_.load(std::memory_order_acquire);

		// Writer's cumulative counter is kCapacity units ahead of the reader's.
		if (write - read == Capacity) {
			return false;
		}

		const std::uint32_t index = write & (Capacity - 1);
		buffer_[index] = item;

		write_idx_.store(write + 1, std::memory_order_release);
		return true;
	}

	/**
	 * @brief Retrieves a single item from the buffer.
	 * @param[out] dest The destination for the retrieved item.
	 * @return true: successful / false: The buffer was empty (out remains unchanged)
	 */
	bool Dequeue(ItemType& dest) {
		const std::uint32_t read  = read_idx_.load(std::memory_order_relaxed);
		const std::uint32_t write = write_idx_.load(std::memory_order_acquire);

		// Reader's cumulative counter catches up to the writer's position.
		if (read == write) {
			return false;
		}

		const std::uint32_t index = read & (Capacity - 1);
		dest = buffer_[index];

		read_idx_.store(read + 1, std::memory_order_release);
		return true;
	}

private:
	/**
	 * @ref https://kumagi.hatenablog.com/entry/ring-buffer
	 * @note Specifying `alignas(...)` pads the memory cache.
	 * Resolve hardware dependencies by specifying `std::hardware_destructive_interference_size`.
	 */
	alignas(std::hardware_destructive_interference_size)std::atomic<std::uint32_t> write_idx_{0};
	alignas(std::hardware_destructive_interference_size)std::atomic<std::uint32_t> read_idx_{0};

	ItemType buffer_[Capacity]{};
};

}

#endif //WIT_RING_BUFFER_H
