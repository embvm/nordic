// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: MIT

#include "twi_helper.hpp"
#include "twi_helper_.hpp"
#include <processor_includes.hpp>
#include <volatile/volatile.hpp>

using namespace embvm;
using namespace embutil;

#pragma mark - Definitions -

namespace
{
enum twi_task_t
{
	TWI_TASK_STARTRX = offsetof(NRF_TWI_Type, TASKS_STARTRX), ///< Start TWI receive sequence.
	TWI_TASK_STARTTX = offsetof(NRF_TWI_Type, TASKS_STARTTX), ///< Start TWI transmit sequence.
	TWI_TASK_STOP = offsetof(NRF_TWI_Type, TASKS_STOP), ///< Stop TWI transaction.
	TWI_TASK_SUSPEND = offsetof(NRF_TWI_Type, TASKS_SUSPEND), ///< Suspend TWI transaction.
	TWI_TASK_RESUME = offsetof(NRF_TWI_Type, TASKS_RESUME) ///< Resume TWI transaction.
};

enum twi_event_t
{
	TWI_EVENT_STOPPED = offsetof(NRF_TWI_Type, EVENTS_STOPPED), ///< TWI stopped.
	TWI_EVENT_RXDREADY = offsetof(NRF_TWI_Type, EVENTS_RXDREADY), ///< TWI RXD byte received.
	TWI_EVENT_TXDSENT = offsetof(NRF_TWI_Type, EVENTS_TXDSENT), ///< TWI TXD byte sent.
	TWI_EVENT_ERROR = offsetof(NRF_TWI_Type, EVENTS_ERROR), ///< TWI error.
	TWI_EVENT_BB = offsetof(
		NRF_TWI_Type,
		EVENTS_BB), ///< TWI byte boundary, generated before each byte that is sent or received.
	TWI_EVENT_SUSPENDED =
		offsetof(NRF_TWI_Type, EVENTS_SUSPENDED) ///< TWI entered the suspended state.
};

/// All TWI interrupts.
constexpr uint32_t ALL_INTS_MASK = static_cast<uint32_t>(
	TWI_INTENSET_STOPPED_Msk | TWI_INTENSET_RXDREADY_Msk | TWI_INTENSET_TXDSENT_Msk |
	TWI_INTENSET_ERROR_Msk | TWI_INTENSET_BB_Msk | TWI_INTENSET_SUSPENDED_Msk);

/// All TWI shortcuts.
constexpr uint32_t ALL_SHORTCUTS_MASK =
	static_cast<uint32_t>(TWI_SHORTS_BB_SUSPEND_Msk | TWI_SHORTS_BB_STOP_Msk);

#pragma mark - Local Helpers -

// TODO: is it cheaper to store a void* in our base class and reinterpret cast every time we use it
// rather than getting the TWI instance with the inst number?
static inline NRF_TWI_Type* getTWIInst(uint8_t inst) noexcept
{
	if(inst == 0)
	{
		return NRF_TWI0;
	}
	else if(inst == 1)
	{
		return NRF_TWI1;
	}
	else
	{
		return nullptr;
	}
}

static inline uint32_t getTWIFrequency(embvm::i2c::baud baudrate) noexcept
{
	uint32_t setting = 0;

	// TODO: TWI_FREQUENCY_FREQUENCY_K250
	switch(baudrate)
	{
		case embvm::i2c::baud::lowSpeed:
			assert(0 && "lowspeed is not supported on NR I2C");
			break;
		case embvm::i2c::baud::standard:
			setting = TWI_FREQUENCY_FREQUENCY_K100;
			break;
		case embvm::i2c::baud::fast:
			setting = TWI_FREQUENCY_FREQUENCY_K400;
			break;
		default:
			assert(0 && "Unknown baudrate");
	}

	return setting;
}

static inline void disable_interrupts_(NRF_TWI_Type* twi) noexcept
{
	volatile_store(&twi->INTENCLR, ALL_INTS_MASK);
}

static inline void enable_interrupts_(NRF_TWI_Type* twi, uint32_t mask) noexcept
{
	volatile_store(&twi->INTENSET, mask);
}

static inline void disable_shortcuts_(NRF_TWI_Type* twi) noexcept
{
	auto shorts = volatile_load(&twi->SHORTS);
	shorts &= ~(ALL_SHORTCUTS_MASK);
	volatile_store(&twi->SHORTS, shorts);
}

static inline void set_shortcuts_(NRF_TWI_Type* twi, uint32_t mask) noexcept
{
	volatile_store(&twi->SHORTS, mask);
}

static inline void clear_event_(NRF_TWI_Type* twi, twi_event_t event) noexcept
{
	uint32_t* offset = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(twi) +
												   static_cast<uintptr_t>(event));
	volatile_store(offset, UINT32_C(0));
#if __CORTEX_M == 0x04
	auto dummy = volatile_load(offset);
	(void)dummy;
#endif
}

static inline bool check_event_(NRF_TWI_Type* twi, twi_event_t event) noexcept
{
	uint32_t* offset = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(twi) +
												   static_cast<uintptr_t>(event));
	return static_cast<bool>(volatile_load(offset));
}

static inline void task_trigger_(NRF_TWI_Type* twi, twi_task_t task) noexcept
{
	uint32_t* offset = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(twi) +
												   static_cast<uintptr_t>(task));
	volatile_store(offset, UINT32_C(1));
}

static inline void set_tx_byte_(NRF_TWI_Type* twi, uint8_t tx_data) noexcept
{
	volatile_store(&twi->TXD, static_cast<uint32_t>(tx_data));
}

static inline uint8_t get_rx_byte_(NRF_TWI_Type* twi) noexcept
{
	return static_cast<uint8_t>(volatile_load(&twi->RXD));
}

}; // anonymous namespace

#pragma mark - TWI Translator Functions -

void nRFTWITranslator::setSCLPin(uint8_t inst, uint8_t port, uint8_t pin) noexcept
{
	auto ctrl = getTWIInst(inst);
	uint32_t value = static_cast<uint32_t>(port << TWI_PSEL_SCL_PORT_Pos) | pin;

#if defined(TWI_PSEL_SCL_CONNECT_Pos)
	volatile_store(&(ctrl->PSEL.SCL), value);
#else
	volatile_store(&ctrl->PSELSCL, value);
#endif
}

void nRFTWITranslator::setSDAPin(uint8_t inst, uint8_t port, uint8_t pin) noexcept
{
	auto ctrl = getTWIInst(inst);
	uint32_t value = static_cast<uint32_t>(port << TWI_PSEL_SCL_PORT_Pos) | pin;

#if defined(TWI_PSEL_SDA_CONNECT_Pos)
	volatile_store(&(ctrl->PSEL.SDA), value);
#else
	volatile_store(&ctrl->PSELSDA, value);
#endif
}

void nRFTWITranslator::setFrequency(uint8_t inst, embvm::i2c::baud baudrate) noexcept
{
	auto ctrl = getTWIInst(inst);
	auto setting = getTWIFrequency(baudrate);

	volatile_store(&ctrl->FREQUENCY, setting);
}

void nRFTWITranslator::disable_interrupts(uint8_t inst) noexcept
{
	auto ctrl = getTWIInst(inst);
	disable_interrupts_(ctrl);
}

void nRFTWITranslator::enable(uint8_t inst) noexcept
{
	auto ctrl = getTWIInst(inst);

	// Enable TWI
	volatile_store(&ctrl->ENABLE,
				   static_cast<uint32_t>(TWI_ENABLE_ENABLE_Enabled << TWI_ENABLE_ENABLE_Pos));
}

void nRFTWITranslator::disable(uint8_t inst) noexcept
{
	auto ctrl = getTWIInst(inst);

	disable_interrupts_(ctrl);
	disable_shortcuts_(ctrl);

	// Disable TWI
	volatile_store(&ctrl->ENABLE,
				   static_cast<uint32_t>(TWI_ENABLE_ENABLE_Disabled << TWI_ENABLE_ENABLE_Pos));
}

void nRFTWITranslator::set_transfer_address(uint8_t inst, uint8_t address) noexcept
{
	auto ctrl = getTWIInst(inst);
	volatile_store(&ctrl->ADDRESS, static_cast<uint32_t>(address));
}

// TODO: do we report bytes transferred? inout param?
// TODO: timeout
/** Blocking Transfer Implementation
 *
 * Implementation note:
 *	If you clear the TWI_EVENET_TXDSENT event, the data that is already loaded into the TX buffer
 *	will be re-sent. You must load the next TX byte and then clear the data.
 */
void nRFTWITranslator::tx_transfer(uint8_t inst, const uint8_t* data, size_t length,
								   bool no_stop) noexcept
{
	auto ctrl = getTWIInst(inst);
	size_t sent = 0;
	assert(data);

	clear_event_(ctrl, TWI_EVENT_STOPPED);
	clear_event_(ctrl, TWI_EVENT_ERROR);
	clear_event_(ctrl, TWI_EVENT_TXDSENT);
	set_shortcuts_(ctrl, 0);

	task_trigger_(ctrl, TWI_TASK_RESUME);
	task_trigger_(ctrl, TWI_TASK_STARTTX);

	set_tx_byte_(ctrl, data[sent]);
	sent++;

	do
	{
		auto error = check_event_(ctrl, TWI_EVENT_ERROR);

		if(error)
		{
			sent--; // Our previous byte failed - reduce count
			clear_event_(ctrl, TWI_EVENT_ERROR);
			clear_event_(ctrl, TWI_EVENT_TXDSENT);
			clear_event_(ctrl, TWI_EVENT_RXDREADY);
			task_trigger_(ctrl, TWI_TASK_STOP);
			// TODO: set error code;
			break;
		}
		else
		{
			if(check_event_(ctrl, TWI_EVENT_TXDSENT))
			{
				set_tx_byte_(ctrl, data[sent]);
				sent++;
				clear_event_(ctrl, TWI_EVENT_TXDSENT);
			}
		}
	} while(sent < length);

	// TODO: handle cb variant properly
	// TODO: error detection

	// TODO: does this handle differently on error? (sent < length)
	if(no_stop)
	{
		task_trigger_(ctrl, TWI_TASK_SUSPEND);
	}
	else
	{
		task_trigger_(ctrl, TWI_TASK_STOP);
	}
	// If timeout <= 0: disable + enable + return error
}

// TODO: timeout
// TODO:
void nRFTWITranslator::rx_transfer(uint8_t inst, uint8_t* data, size_t length) noexcept
{
	auto ctrl = getTWIInst(inst);
	size_t received = 0;

	assert(data);

	clear_event_(ctrl, TWI_EVENT_STOPPED);
	clear_event_(ctrl, TWI_EVENT_ERROR);
	// clear_event_(ctrl, TWI_EVENT_TXDSENT);
	clear_event_(ctrl, TWI_EVENT_RXDREADY);

	if(length == 1)
	{
		set_shortcuts_(ctrl, TWI_SHORTS_BB_STOP_Msk);
	}
	else
	{
		// TODO: does this really need to be set?
		set_shortcuts_(ctrl, TWI_SHORTS_BB_SUSPEND_Msk);
	}

	task_trigger_(ctrl, TWI_TASK_RESUME);
	task_trigger_(ctrl, TWI_TASK_STARTRX);

	do
	{
		auto error = check_event_(ctrl, TWI_EVENT_ERROR);

		if(error)
		{
			received--; // Our previous byte failed - reduce count
			clear_event_(ctrl, TWI_EVENT_ERROR);
			// clear_event_(ctrl, TWI_EVENT_TXDSENT);
			clear_event_(ctrl, TWI_EVENT_RXDREADY);
			task_trigger_(ctrl, TWI_TASK_STOP);
			// TODO: set error code;
			break;
		}
		else
		{
			if(check_event_(ctrl, TWI_EVENT_RXDREADY))
			{
				data[received] = get_rx_byte_(ctrl);
				received++;

				if(received == (length - 1))
				{
					set_shortcuts_(ctrl, TWI_SHORTS_BB_STOP_Msk);
				}

				if(received != length)
				{
					clear_event_(ctrl, TWI_EVENT_RXDREADY);
					task_trigger_(ctrl, TWI_TASK_RESUME);
				}
			}
		}
	} while(received < length);

#if 0
	TODO: error detection
	            uint32_t errorsrc =  nrf_twi_errorsrc_get_and_clear(p_twi);

            if (errorsrc)
            {
                ret_code = twi_process_error(errorsrc);
            }
#endif

	task_trigger_(ctrl, TWI_TASK_STOP);

	// If timeout <= 0: disable + enable + return error
}

void nRFTWITranslator::stop_condition(uint8_t inst) noexcept
{
	auto ctrl = getTWIInst(inst);
	task_trigger_(ctrl, TWI_TASK_STOP);
}

#pragma mark - Interrupt Handlers -

extern "C" void twi_0_irq_handler() {}

extern "C" void twi_1_irq_handler() {}
