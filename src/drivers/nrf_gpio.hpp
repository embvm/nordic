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
class nRFGPIOOutput final : public embvm::gpio::output
{
  public:
	/** Construct a Nordic GPIO output
	 */
	nRFGPIOOutput() noexcept {}

	/// Default destructor
	~nRFGPIOOutput() final = default;

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

  private:
	void start_() noexcept final
	{
		nRFGPIOTranslator::configure_output(TPort, TPin);
	}

	void stop_() noexcept final
	{
		nRFGPIOTranslator::configure_default(TPort, TPin);
	}
};

#if 0
template<uint8_t TPort, uint8_t TPin>
class nRFGPIOInput final : public embvm::gpio::input
{
  public:
	/** Construct a generic GPIO input
	 */
	explicit nRFGPIOInput() : embvm::gpio::input("nRF GPIO Input") {}

	/** Construct a named GPIO input
	 *
	 * @param name The name of the GPIO pin
	 */
	explicit nRFGPIOInput(const char* name) : embvm::gpio::input(name) {}

	/// Default destructor
	~nRFGPIOInput() final = default;

	bool get() final;

  private:
	void start_() final;
	void stop_() final;
};
#endif

#endif // NRF_GPIO_DRIVER_HPP_
