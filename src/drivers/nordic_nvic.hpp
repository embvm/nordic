// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: MIT

#ifndef NORDIC_NVIC_DRIVER_HPP_
#define NORDIC_NVIC_DRIVER_HPP_

#include <nvic.hpp>
#include <processor/interrupt_manager.hpp>

/** Nordic NVIC Controller Interface Expansion
 *
 * This class can be used instead of the ARM controller to provide support for enabling IRQs
 * using a peripheral base pointer
 *
 * This class is not meant to be instantiated directly. Instead, use the static inline methods.
 */
class NordicNVIC
{
  public:
	template<typename TPeripheralType>
	static inline uint32_t priority(const TPeripheralType* peripheral) noexcept
	{
		auto irq = get_peripheral_id(peripheral);
		return NVICControl::priority(irq);
	}

	template<typename TPeripheralType>
	static inline uint32_t priority(const TPeripheralType* peripheral, uint32_t p) noexcept
	{
		auto irq = get_peripheral_id(peripheral);
		return NVICControl::priority(irq, p);
	}

	template<typename TPeripheralType>
	static inline void enable(const TPeripheralType* peripheral) noexcept
	{
		auto irq = get_peripheral_id(peripheral);
		NVICControl::enable(irq);
	}

	template<typename TPeripheralType>
	static inline void disable(const TPeripheralType* peripheral) noexcept
	{
		auto irq = get_peripheral_id(peripheral);
		NVICControl::disable(irq);
	}

	template<typename TPeripheralType>
	static inline bool enabled(const TPeripheralType* peripheral) noexcept
	{
		auto irq = get_peripheral_id(peripheral);
		return NVICControl::enabled(irq);
	}

	template<typename TPeripheralType>
	static inline bool pending(const TPeripheralType* peripheral) noexcept
	{
		auto irq = get_peripheral_id(peripheral);
		return NVICControl::pending(irq);
	}

	template<typename TPeripheralType>
	static inline void trigger(const TPeripheralType* peripheral) noexcept
	{
		auto irq = get_peripheral_id(peripheral);
		NVICControl::trigger(irq);
	}

	template<typename TPeripheralType>
	static inline void clear(const TPeripheralType* peripheral) noexcept
	{
		auto irq = get_peripheral_id(peripheral);
		NVICControl::clear(irq);
	}

	template<typename TPeripheralType>
	static inline uintptr_t handler(const TPeripheralType* peripheral, uintptr_t func) noexcept
	{
		auto irq = get_peripheral_id(peripheral);
		return NVICControl::handler(irq, func);
	}

	template<typename TPeripheralType>
	static inline uintptr_t handler(const TPeripheralType* peripheral) noexcept
	{
		auto irq = get_peripheral_id(peripheral);
		return NVICControl::handler(irq);
	}

  private:
	// This class cannot be instantiated.
	NordicNVIC() = default;
	~NordicNVIC() = default;
};

#endif // NORDIC_NVIC_DRIVER_HPP_
