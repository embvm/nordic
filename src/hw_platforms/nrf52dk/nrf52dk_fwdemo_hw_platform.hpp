#ifndef NRF52_DK_HW_PLATFORM_HPP_
#define NRF52_DK_HW_PLATFORM_HPP_

#include <array>
#include <dispatch/interrupt_queue.hpp>
#include <driver/active_i2c.hpp>
#include <driver/led.hpp>
#include <hw_platform/virtual_hw_platform.hpp>
#include <nRF52840.hpp>
#include <nrf52_i2c_master.hpp>
#include <nrf_gpio.hpp>
#include <solomon_systech/ssd1306/ssd1306.hpp>
#include <st/vl53l1x/vl53l1x.hpp>
#include <thumb_interrupt_lock.hpp>

#define SPARKFUN_VL53L1X_ADDR 0x29
#define SPARKFUN_SSD1306_ADDR 0x3D

class NRF52DKHWPlatform : public embvm::VirtualHwPlatformBase<NRF52DKHWPlatform>
{
	using PlatformBase = embvm::VirtualHwPlatformBase<NRF52DKHWPlatform>;
	using i2c0_t = nRFi2cMaster<NordicTWIM0, nRFPinID<0, 27>, nRFPinID<0, 26>>;

  public:
	/// @brief Default constructor.
	NRF52DKHWPlatform() noexcept : PlatformBase("nRF52840 Development Kit") {}

	/// @brief Default destructor.
	~NRF52DKHWPlatform() noexcept;

#pragma mark - Inherited Functions -

	static void earlyInitHook_() noexcept;
	void init_() noexcept;
	void initProcessor_() noexcept;
	void soft_reset_() noexcept;
	void hard_reset_() noexcept;

#pragma mark - Custom Functions -

	void leds_off() noexcept;

	std::array<embvm::led::base*, 4> getLEDInstances() noexcept
	{
		return {&led1, &led2, &led3, &led4};
	}

	embvm::tof::sensor& tof0_inst() noexcept
	{
		return tof0;
	}

	embvm::basicDisplay& screen0_inst() noexcept
	{
		return screen0;
	}

  private:
	nRF52840 processor_;

	// Can handle 8 queued interrupt functions
	// TODO: this thread needs to be the highest priority - handle that internally.
	// TODO: tune size down to 8 or so when priority is fixed
	embutil::InterruptQueue<ThumbInterruptLock, 156> irq_bottom_half_;

	nRFGPIOOutput<0, 13> led1_pin{};
	nRFGPIOOutput<0, 14> led2_pin{};
	nRFGPIOOutput<0, 15> led3_pin{};
	nRFGPIOOutput<0, 16> led4_pin{};

	embvm::led::gpioActiveLow led1{led1_pin, "led1"};
	embvm::led::gpioActiveLow led2{led2_pin, "led2"};
	embvm::led::gpioActiveLow led3{led3_pin, "led3"};
	embvm::led::gpioActiveLow led4{led4_pin, "led4"};

	i2c0_t i2c0_private_;
	// TODO: tune size down
	embvm::i2c::activeMaster<128> i2c0{i2c0_private_};

	embdrv::vl53l1x tof0{i2c0, SPARKFUN_VL53L1X_ADDR};
	embdrv::ssd1306 screen0{i2c0, SPARKFUN_SSD1306_ADDR};
};

#endif // NRF52_DK_HW_PLATFORM_HPP_
