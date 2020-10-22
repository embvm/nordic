#include "nrf52_dongle_hw_platform.hpp"
#include <cassert>
#include <malloc.h>

extern int __HeapBase;
extern int __HeapLimit;

NRF52DongleHWPlatform::NRF52DongleHWPlatform() noexcept
{
	registerDriver("led1", &led1);
	registerDriver("led2_r", &led2_r);
	registerDriver("led2_g", &led2_g);
	registerDriver("led2_b", &led2_b);
}

NRF52DongleHWPlatform::~NRF52DongleHWPlatform() noexcept {}

void NRF52DongleHWPlatform::earlyInitHook_() noexcept
{
	// TODO: this doesn't actually belong in HW platform
	malloc_addblock(&__HeapBase, &__HeapLimit - &__HeapBase);
}

void NRF52DongleHWPlatform::init_() noexcept
{
	// If nRF52 USB Dongle is powered from USB (high voltage mode),
	// GPIO output voltage is set to 1.8 V by default, which is not
	// enough to turn on green and blue LEDs. Therefore, GPIO voltage
	// needs to be increased to 3.0 V by configuring the UICR register.
	if(!processor.highVoltageMode())
	{
		processor.highVoltageMode(true);
	}

	// start all LEDs
	// turn them off? Or just trust that they start off?
	led1.start();
	led2_r.start();
	led2_g.start();
	led2_b.start();
}
