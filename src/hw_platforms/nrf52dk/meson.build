# meson build for nrf52840 USB dongle HW platform

nrf52dk_fwdemo_hw_platform = declare_dependency(
	include_directories: include_directories('.'),
	sources: [files('nrf52dk_fwdemo_hw_platform.cpp')],
	dependencies: [
		nrf52840_processor_dep,
		ssd1305_dep,
		st_vl53l1x_driver_dep,
	]
)
