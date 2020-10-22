#ifndef NRF52840_DONGLE_HW_PLATFORM_HPP_
#define NRF52840_DONGLE_HW_PLATFORM_HPP_

#include <driver/led.hpp>
#include <hw_platform/virtual_hw_platform.hpp>
#include <nRF52840.hpp>
#include <nrf_gpio.hpp>

class NRF52DongleHWPlatform : public embvm::VirtualHwPlatformBase<NRF52DongleHWPlatform>
{
	using PlatformBase = embvm::VirtualHwPlatformBase<NRF52DongleHWPlatform>;

  public:
	/// @brief Default constructor.
	NRF52DongleHWPlatform() noexcept;

	/// @brief Default destructor.
	~NRF52DongleHWPlatform();

#pragma mark - Inherited Functions -

	static void earlyInitHook_() noexcept;

	void init_() noexcept;

	void initProcessor_() noexcept {}

	void soft_reset_() noexcept {}

	void hard_reset_() noexcept
	{
		// We can't perform a hard reset of our computer from the simulator app...
		soft_reset_();
	}

#pragma mark - Custom Functions -

	void leds_off() noexcept
	{
		led1.off();
		led2_r.off();
		led2_g.off();
		led2_b.off();
	}

	void startBlink() noexcept
	{
		while(true)
		{
			for(const auto& led : {&led1, &led2_r, &led2_g, &led2_b})
			{
				led->toggle();
				processor.spin(500 * 1000); // 500ms
			}
		}
	}

  private:
  private:
	nRF52840 processor;

	nRFGPIOOutput<0, 6> led1_g_pin{};
	nRFGPIOOutput<0, 8> led2_r_pin{};
	nRFGPIOOutput<1, 9> led2_g_pin{};
	nRFGPIOOutput<0, 12> led2_b_pin{};

	embvm::led::gpioActiveLow led1{led1_g_pin};
	embvm::led::gpioActiveLow led2_r{led2_r_pin};
	embvm::led::gpioActiveLow led2_g{led2_g_pin};
	embvm::led::gpioActiveLow led2_b{led2_b_pin};

#if 0
// There is only one button for the application
// as the second button is used for a RESET.
#define BUTTONS_NUMBER 1

#define BUTTON_1 NRF_GPIO_PIN_MAP(1, 6)
#define BUTTON_PULL NRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE 0

#define BUTTONS_LIST \
	{                \
		BUTTON_1     \
	}

#define BSP_BUTTON_0 BUTTON_1

#define BSP_SELF_PINRESET_PIN NRF_GPIO_PIN_MAP(0, 19)

#define HWFC true
#endif
};

#endif // NRF52840_DONGLE_HW_PLATFORM_HPP_
