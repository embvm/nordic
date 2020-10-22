#ifndef NRF51_I2C_MASTER_BLOCKING_HPP_
#define NRF51_I2C_MASTER_BLOCKING_HPP_

#include "helpers/twi_helper.hpp"
#include "nrf_gpio.hpp"
#include <cstdint>
#include <driver/i2c.hpp>

/**
 *
 * @tparam TTWIIndex The instance of the TWI to use. Can be 0 or 1.
 * @tparam TSclPin A class representing the SCL pin. This is designed for use with nRFPinID<>:
 *	@code
 *	nRFi2cMaster_Blocking<0, nRFPinID<0,27>, nRFPinID<0,26>> i2c0;
 *	@endcode
 * @tparam TSdaPin A class representing the SDA pin. This is designed for use with nRFPinID<>:
 *	@code
 *	nRFi2cMaster_Blocking<0, nRFPinID<0,27>, nRFPinID<0,26>> i2c0;
 *	@endcode
 */
template<unsigned TTWIIndex, typename TSclPin, typename TSdaPin>
class nRF51i2cMaster_Blocking final : public embvm::i2c::master
{
	static_assert(TTWIIndex == 0 || TTWIIndex == 1, "Nordic only supports instances 0 and 1");

  public:
	nRFi2cMaster_Blocking() noexcept {}

	~nRFi2cMaster_Blocking() = default;

  private:
	void start_() noexcept final
	{
		/* To secure correct signal levels on the pins used by the TWI
		   master when the system is in OFF mode, and when the TWI master is
		   disabled, these pins must be configured in the GPIO peripheral.
		*/
		nRFGPIOTranslator::configure_i2c(TSclPin::port(), TSclPin::pin());
		nRFGPIOTranslator::configure_i2c(TSdaPin::port(), TSdaPin::pin());

		nRFTWITranslator::setSCLPin(TTWIIndex, TSclPin::port(), TSclPin::pin());
		nRFTWITranslator::setSDAPin(TTWIIndex, TSdaPin::port(), TSdaPin::pin());

		nRFTWITranslator::enable(TTWIIndex);
	}

	void stop_() noexcept final
	{
		nRFTWITranslator::disable(TTWIIndex);
		nRFGPIOTranslator::configure_default(TSclPin::port(), TSclPin::pin());
		nRFGPIOTranslator::configure_default(TSdaPin::port(), TSdaPin::pin());
	}

	void configure_(embvm::i2c::pullups pullup) noexcept final {}

	embvm::i2c::status transfer_(const embvm::i2c::op_t& op,
								 const embvm::i2c::master::cb_t& cb) noexcept final
	{
		auto status = embvm::i2c::status::ok;

		nRFTWITranslator::set_transfer_address(TTWIIndex, op.address);

		switch(op.op)
		{
			case embvm::i2c::operation::continueWriteStop:
			case embvm::i2c::operation::write: {
				nRFTWITranslator::tx_transfer_blocking_(TTWIIndex, op.tx_buffer, op.tx_size,
														nRFTWITranslator::STOP);
				break;
			}
			case embvm::i2c::operation::writeNoStop:
			case embvm::i2c::operation::continueWriteNoStop: {
				nRFTWITranslator::tx_transfer_blocking_(TTWIIndex, op.tx_buffer, op.tx_size,
														nRFTWITranslator::NO_STOP);
				break;
			}
			case embvm::i2c::operation::read: {
				nRFTWITranslator::rx_transfer_blocking_(TTWIIndex, op.rx_buffer, op.rx_size);
				break;
			}
			case embvm::i2c::operation::writeRead: {
				nRFTWITranslator::txrx_transfer_blocking_(TTWIIndex, op.tx_buffer, op.tx_size,
														  op.rx_buffer, op.rx_size);
				break;
			}
			case embvm::i2c::operation::ping: {
				// TODO
				break;
			}
			case embvm::i2c::operation::stop: {
				nRFTWITranslator::stop_condition(TTWIIndex);
				break;
			}
			case embvm::i2c::operation::restart: {
				// TODO:
				break;
			}
		}

		return status;
	}

	embvm::i2c::baud baudrate_(embvm::i2c::baud baud) noexcept final
	{
		nRFTWITranslator::setFrequency(TTWIIndex, baud);

		return baud;
	}

	embvm::i2c::pullups setPullups_(embvm::i2c::pullups pullups) noexcept final
	{
		return pullups;
	}

  private:
};

#endif // NRF51_I2C_MASTER_BLOCKING_HPP_
