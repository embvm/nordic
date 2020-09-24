#include <platform.hpp>

volatile bool abort_program_ = false;

int main()
{
	auto& platform = VirtualPlatform::inst();
	platform.initProcessor();
	platform.initHWPlatform();
	platform.init();

	// TODO: get all of the LEDs through the driver registry and toggle them in series,
	// Like the nRF program does
	platform.startBlink();

	return 0;
}
