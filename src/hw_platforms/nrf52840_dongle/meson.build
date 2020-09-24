# meson build for nrf52840 USB dongle HW platform

nf52840_dongle_hw_platform_dep = declare_dependency(
	include_directories: include_directories('.'),
	sources: [files('nrf52_dongle_hw_platform.cpp')],
	dependencies: [
		nrf52840_processor_dep,
	]
)
