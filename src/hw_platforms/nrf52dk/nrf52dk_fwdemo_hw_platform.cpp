#include "nrf52dk_fwdemo_hw_platform.hpp"
#include <cassert>

NRF52DKHWPlatform::~NRF52DKHWPlatform() noexcept {}

void NRF52DKHWPlatform::earlyInitHook_() noexcept {}

void NRF52DKHWPlatform::init_() noexcept
{
	i2c0_private_.setBottomHalfDispatcher(irq_bottom_half_.getBoundDispatch());

	i2c0.start();
	i2c0.configure(embvm::i2c::baud::fast);

	tof0.start();

	screen0.start();

	// start all LEDs
	// turn them off? Or just trust that they start off?
	led1.start();
	led2.start();
	led3.start();
	led4.start();
}

void NRF52DKHWPlatform::leds_off() noexcept
{
	led1.off();
	led2.off();
	led2.off();
	led2.off();
}

void NRF52DKHWPlatform::hard_reset_() noexcept
{
	soft_reset_();
}

void NRF52DKHWPlatform::soft_reset_() noexcept
{
	processor_.reset();
}

void NRF52DKHWPlatform::initProcessor_() noexcept
{
	processor_.init();
}
