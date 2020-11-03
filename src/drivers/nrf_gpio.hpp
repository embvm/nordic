#ifndef NRF_GPIO_DRIVER_HPP_
#define NRF_GPIO_DRIVER_HPP_

#include "helpers/gpio_helper.hpp"
#include <driver/gpio.hpp>

/** Convenience class used to provide a Port and Pin for later ID.
 *
 * Example usage can be found in nrf_i2c.hpp. The nRFi2cMaster class is templated off of two
 * pins: SCL pin and SDA. Using the nRFPinID class, we do not need intermediary GPIO objects. The
 * I2C class can get the port and pin numbers directly. These numbers are used to configure the
 * device and pins.
 *
 * @code
 * nRFi2cMaster<0, nRFPinID<0,27>, nRFPinID<0,26>> i2c0;
 * @endcode
 *
 * @tparam TPort the GPIO Bank.
 * @tparam TPin the pin within the selected GPIO bank.
 */
template<uint8_t TPort, uint8_t TPin>
class nRFPinID
{
  public:
	static constexpr uint8_t port() noexcept
	{
		return TPort;
	}

	static constexpr uint8_t pin() noexcept
	{
		return TPin;
	}
};

template<uint8_t TPort, uint8_t TPin>
class nRFGPIO final : public embvm::gpio::base
{
  public:
	/** Construct a Nordic GPIO
	 */
	nRFGPIO() noexcept : mode_(embvm::gpio::mode::input) {}

	/// Construct a GPIO with a mode
	/// @param [in] mode The desired mode to initialize the GPIO to when
	/// 	starting the pin.
	explicit nRFGPIO(embvm::gpio::mode mode) noexcept : mode_(mode) {}

	/// Construct a Nordic GPIO with a target mode
	/// @param [in] mode Target GPIO mode

	/// Default destructor
	~nRFGPIO() noexcept = default;

	void set(bool v) noexcept final
	{
		if(v)
		{
			nRFGPIOTranslator::set(TPort, TPin);
		}
		else
		{
			nRFGPIOTranslator::clear(TPort, TPin);
		}
	}

	inline void toggle() noexcept final
	{
		nRFGPIOTranslator::toggle(TPort, TPin);
	}

	inline bool get() noexcept final
	{
		return nRFGPIOTranslator::get(TPort, TPin);
	}

	void setMode(embvm::gpio::mode m) noexcept final
	{
		switch(m)
		{
			case embvm::gpio::mode::input:

				break;
			case embvm::gpio::mode::output:
				nRFGPIOTranslator::configure_output(TPort, TPin);
				break;
			case embvm::gpio::mode::special:
				// Currently unsupported mode
			case embvm::gpio::mode::MAX_MODE:
			default:
				assert(false);
		}

		mode_ = m;
	}

	embvm::gpio::mode mode() noexcept final
	{
		return mode_;
	}

  private:
	void start_() noexcept final
	{
		setMode(mode_);
	}

	void stop_() noexcept final
	{
		nRFGPIOTranslator::configure_default(TPort, TPin);
	}

  private:
	embvm::gpio::mode mode_;
};

#endif // NRF_GPIO_DRIVER_HPP_
