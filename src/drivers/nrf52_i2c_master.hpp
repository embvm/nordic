#ifndef NRF52_I2C_MASTER_HPP_
#define NRF52_I2C_MASTER_HPP_

#include "helpers/gpio_helper.hpp"
#include "helpers/twim_helper.hpp"
#include <cstdint>
#include <driver/hal_driver.hpp>
#include <driver/i2c.hpp>
#include <nordic_defs.hpp>

// TODO: remove
#include <volatile/volatile.hpp>

// TODO: run an experiment where TTWIIndex, TSclPin, TSdaPin are static const members to see
// if there is better optimization possibility (classes are unified)

/** DMA-capable I2C master (TBlocking)
 *
 * @tparam TTWIIndex The instance of the TWI to use. Can be NordicTWIM0 or NordicTWIM1
 * @tparam TSclPin A class representing the SCL pin. This is designed for use with nRFPinID<>:
 *	@code
 *	nRFi2cMaster_TBlocking<0, nRFPinID<0,27>, nRFPinID<0,26>> i2c0;
 *	@endcode
 * @tparam TSdaPin A class representing the SDA pin. This is designed for use with nRFPinID<>:
 *	@code
 *	nRFi2cMaster_TBlocking<0, nRFPinID<0,27>, nRFPinID<0,26>> i2c0;
 *	@endcode
 */
template<NordicTWIM TTWIIndex, typename TSclPin, typename TSdaPin, bool TBlocking = false>
class nRFi2cMaster final : public embvm::i2c::master, public embvm::HALDriverBase
{
	static_assert(TTWIIndex == NordicTWIM0 || TTWIIndex == NordicTWIM1,
				  "This Nordic driver only supports TWIM instances 0 and 1");

  public:
	static constexpr uint8_t PRIORITY_DEFAULT = NRF_IRQ_PRIOR_MID;

  public:
	nRFi2cMaster(uint8_t priority = PRIORITY_DEFAULT) noexcept
		: embvm::i2c::master("nRF I2C Master"), priority_(priority)
	{
	}

	nRFi2cMaster(const char* name, uint8_t priority = PRIORITY_DEFAULT) noexcept
		: embvm::i2c::master(name), priority_(priority)
	{
	}

	~nRFi2cMaster() = default;

  private:
	void start_() noexcept final
	{
		/* To secure correct signal levels on the pins used by the TWI
		   master when the system is in OFF mode, and when the TWI master is
		   disabled, these pins must be configured in the GPIO peripheral.
		*/
		nRFGPIOTranslator::configure_i2c(TSclPin::port(), TSclPin::pin());
		nRFGPIOTranslator::configure_i2c(TSdaPin::port(), TSdaPin::pin());

		nRFTWIMTranslator::setSCLPin(TTWIIndex, TSclPin::port(), TSclPin::pin());
		nRFTWIMTranslator::setSDAPin(TTWIIndex, TSdaPin::port(), TSdaPin::pin());

		nRFTWIMTranslator::enable(TTWIIndex);

		if constexpr(!TBlocking)
		{
			nRFTWIM_cb_t cb = [this](embvm::i2c::status status) noexcept {
				this->twim_callback_(status);
			};

			nRFTWIMTranslator::setCallback(TTWIIndex, cb);
			nRFTWIMTranslator::set_interrupt_priority(TTWIIndex, priority_);
			enableInterrupts();
		}
	}

	void stop_() noexcept final
	{
		if constexpr(!TBlocking)
		{
			disableInterrupts();
		}

		nRFTWIMTranslator::disable(TTWIIndex);
		nRFGPIOTranslator::configure_default(TSclPin::port(), TSclPin::pin());
		nRFGPIOTranslator::configure_default(TSdaPin::port(), TSdaPin::pin());
	}

	void configure_([[maybe_unused]] embvm::i2c::pullups pullup) noexcept final {}

	embvm::i2c::status transfer_(const embvm::i2c::op_t& op,
								 const embvm::i2c::master::cb_t& cb) noexcept final
	{
		return transfer_impl_(op, cb);
	}

	/// Blocking Variant
	template<typename Dummy = embvm::i2c::status>
	auto transfer_impl_(const embvm::i2c::op_t& op, const embvm::i2c::master::cb_t& cb) noexcept
		-> std::enable_if_t<TBlocking, Dummy>
	{
		auto status = embvm::i2c::status::ok;

		nRFTWIMTranslator::set_transfer_address(TTWIIndex, op.address);
		switch(op.op)
		{
			case embvm::i2c::operation::continueWriteStop:
			case embvm::i2c::operation::write: {
				status = nRFTWIMTranslator::tx_transfer_blocking(
					TTWIIndex, op.tx_buffer, op.tx_size, nRFTWIMTranslator::STOP);
				break;
			}
			case embvm::i2c::operation::writeNoStop:
			case embvm::i2c::operation::continueWriteNoStop: {
				status = nRFTWIMTranslator::tx_transfer_blocking(
					TTWIIndex, op.tx_buffer, op.tx_size, nRFTWIMTranslator::NO_STOP);
				break;
			}
			case embvm::i2c::operation::read: {
				status =
					nRFTWIMTranslator::rx_transfer_blocking(TTWIIndex, op.rx_buffer, op.rx_size);
				break;
			}
			case embvm::i2c::operation::writeRead: {
				status = nRFTWIMTranslator::txrx_transfer_blocking(
					TTWIIndex, op.tx_buffer, op.tx_size, op.rx_buffer, op.rx_size);
				break;
			}
			case embvm::i2c::operation::ping: {
				static uint8_t ping_dummy_byte_;

				status = nRFTWIMTranslator::rx_transfer_blocking(TTWIIndex, &ping_dummy_byte_,
																 sizeof(ping_dummy_byte_));
				break;
			}
			case embvm::i2c::operation::stop: {
				nRFTWIMTranslator::stop_condition(TTWIIndex);
				break;
			}
			case embvm::i2c::operation::restart: {
				// TODO: is this enough?
				nRFTWIMTranslator::stop_condition(TTWIIndex);
				break;
			}
		}

		return status;
	}

	/// Non-blocking variant
	template<typename Dummy = embvm::i2c::status>
	auto transfer_impl_(const embvm::i2c::op_t& op, const embvm::i2c::master::cb_t& cb) noexcept
		-> std::enable_if_t<!TBlocking, Dummy>
	{
		auto status = embvm::i2c::status::ok;

		if(__sync_bool_compare_and_swap(&busy_, false, true))
		{
			active_cb_ = cb;
			active_op_ = op;
		}
		else
		{
			// The bus is busy
			return embvm::i2c::status::busy;
		}

		nRFTWIMTranslator::set_transfer_address(TTWIIndex, op.address);
		switch(op.op)
		{
			case embvm::i2c::operation::continueWriteStop:
			case embvm::i2c::operation::write: {
				status = nRFTWIMTranslator::tx_transfer(TTWIIndex, op.tx_buffer, op.tx_size,
														nRFTWIMTranslator::STOP);
				break;
			}
			case embvm::i2c::operation::writeNoStop:
			case embvm::i2c::operation::continueWriteNoStop: {
				status = nRFTWIMTranslator::tx_transfer(TTWIIndex, op.tx_buffer, op.tx_size,
														nRFTWIMTranslator::NO_STOP);
				break;
			}
			case embvm::i2c::operation::read: {
				status = nRFTWIMTranslator::rx_transfer(TTWIIndex, op.rx_buffer, op.rx_size);
				break;
			}
			case embvm::i2c::operation::writeRead: {
				status = nRFTWIMTranslator::txrx_transfer(TTWIIndex, op.tx_buffer, op.tx_size,
														  op.rx_buffer, op.rx_size);
				break;
			}
			case embvm::i2c::operation::ping: {
				static uint8_t ping_dummy_byte_;

				status = nRFTWIMTranslator::rx_transfer(TTWIIndex, &ping_dummy_byte_,
														sizeof(ping_dummy_byte_));
				break;
			}
			case embvm::i2c::operation::stop: {
				nRFTWIMTranslator::stop_condition(TTWIIndex);
				break;
			}
			case embvm::i2c::operation::restart: {
				// TODO: is this enough?
				nRFTWIMTranslator::stop_condition(TTWIIndex);
				break;
			}
		}

		return status;
	}

	embvm::i2c::baud baudrate_(embvm::i2c::baud baud) noexcept final
	{
		nRFTWIMTranslator::setFrequency(TTWIIndex, baud);

		return baud;
	}

	embvm::i2c::pullups setPullups_(embvm::i2c::pullups pullups) noexcept final
	{
		return pullups;
	}

	void enableInterrupts() noexcept final
	{
		nRFTWIMTranslator::enable_interrupts(TTWIIndex);
	}

	void disableInterrupts() noexcept final
	{
		nRFTWIMTranslator::disable_interrupts(TTWIIndex);
	}

  private:
	void twim_callback_(embvm::i2c::status status) noexcept
	{
		static embvm::i2c::master::cb_t cb;
		static embvm::i2c::op_t op;

		cb = std::move(active_cb_);
		op = std::move(active_op_);

		auto r = __sync_bool_compare_and_swap(&busy_, true, false);
		assert(r);

		invokeCallback(cb, op, status);
	}

  private:
	const uint8_t priority_;
	bool busy_ = false;
	embvm::i2c::master::cb_t active_cb_{nullptr};
	embvm::i2c::op_t active_op_{};
};

/// Convenience alias to declare a blocking I2C Master Driver
template<NordicTWIM TTWIIndex, typename TSclPin, typename TSdaPin>
using nRFi2cMaster_Blocking = nRFi2cMaster<TTWIIndex, TSclPin, TSdaPin, true>;

#endif // NRF52_I2C_MASTER_HPP_
