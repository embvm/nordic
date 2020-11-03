// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: MIT

#ifndef NORDIC_UTILS_HPP_
#define NORDIC_UTILS_HPP_

#pragma mark - Definitions -

#pragma mark - Helper Functions -

static inline bool is_in_nordic_ram(void const* ptr) noexcept
{
	return (reinterpret_cast<uintptr_t>(ptr) & 0xE0000000u) == 0x20000000u;
}

template<typename TPeripheralType>
static inline uint8_t get_peripheral_id(TPeripheralType* base_addr) noexcept
{
	return static_cast<uint8_t>(reinterpret_cast<uintptr_t>(base_addr) >> 12);
}

#endif // NORDIC_UTILS_HPP_
