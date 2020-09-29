project('Nordic Semiconductor nRFX',
	version: '2.3.0'
)

nrfx_includes = [
	# Access to nrfx.h
	include_directories('.', is_system: true),
	# Hardware Access Layer
	include_directories('hal', is_system: true),
	# SOC-specific files
	include_directories('soc', is_system: true),
	# nRF MDK Files
	include_directories('mdk', is_system: true),
]

nrfx_driver_includes = [
	# nrfx driver helper files
	include_directories('helpers', is_system: true),
	include_directories('drivers', is_system: true),
	include_directories('drivers/include', is_system: true)
]

# TODO: probably need to split this up based on the matrix??
nrfx_driver_sources = files(
	'drivers/src/prs/nrfx_prs.c',
	'drivers/src/nrfx_adc.c',
	'drivers/src/nrfx_clock.c',
	'drivers/src/nrfx_comp.c',
	'drivers/src/nrfx_dppi.c',
	'drivers/src/nrfx_egu.c',
	'drivers/src/nrfx_gpiote.c',
	'drivers/src/nrfx_i2s.c',
	'drivers/src/nrfx_ipc.c',
	'drivers/src/nrfx_lpcomp.c',
	'drivers/src/nrfx_nfct.c',
	'drivers/src/nrfx_nvmc.c',
	'drivers/src/nrfx_pdm.c',
	'drivers/src/nrfx_power.c',
	'drivers/src/nrfx_ppi.c',
	'drivers/src/nrfx_pwm.c',
	'drivers/src/nrfx_qdec.c',
	'drivers/src/nrfx_qspi.c',
	'drivers/src/nrfx_rng.c',
	'drivers/src/nrfx_rtc.c',
	'drivers/src/nrfx_saadc.c',
	'drivers/src/nrfx_spi.c',
	'drivers/src/nrfx_spim.c',
	'drivers/src/nrfx_spis.c',
	'drivers/src/nrfx_systick.c',
	'drivers/src/nrfx_temp.c',
	'drivers/src/nrfx_timer.c',
	'drivers/src/nrfx_twi.c',
	'drivers/src/nrfx_twi_twim.c',
	'drivers/src/nrfx_twim.c',
	'drivers/src/nrfx_twis.c',
	'drivers/src/nrfx_uart.c',
	'drivers/src/nrfx_uarte.c',
	'drivers/src/nrfx_usbd.c',
	'drivers/src/nrfx_usbreg.c',
	'drivers/src/nrfx_wdt.c',
)

nrfx_nrf52840_dep = declare_dependency(
	sources: [
		'mdk/gcc_startup_nrf52840.S',
		'mdk/system_nrf52840.c',
	],
	compile_args: [
		'-DCONFIG_GPIO_AS_PINRESET',
		'-DNRF52840_XXAA',
	],
	include_directories: [
		nrfx_includes,
		nrfx_driver_includes,
	]
)

# Comment Block - these are files that still need to be processed
'''
mdk/system_nrf51.c
mdk/system_nrf52.c
mdk/system_nrf52805.c
mdk/system_nrf52810.c
mdk/system_nrf52811.c
mdk/system_nrf52820.c
mdk/system_nrf52833.c
mdk/system_nrf5340_application.c
mdk/system_nrf5340_network.c
mdk/system_nrf9160.c



mdk/arm_startup_nrf51.s
mdk/arm_startup_nrf52.s
mdk/arm_startup_nrf52805.s
mdk/arm_startup_nrf52810.s
mdk/arm_startup_nrf52811.s
mdk/arm_startup_nrf52820.s
mdk/arm_startup_nrf52833.s
mdk/arm_startup_nrf52840.s
mdk/arm_startup_nrf5340_application.s
mdk/arm_startup_nrf5340_network.s
mdk/arm_startup_nrf9160.s
mdk/iar_startup_nrf51.s
mdk/iar_startup_nrf52.s
mdk/iar_startup_nrf52805.s
mdk/iar_startup_nrf52810.s
mdk/iar_startup_nrf52811.s
mdk/iar_startup_nrf52820.s
mdk/iar_startup_nrf52833.s
mdk/iar_startup_nrf52840.s
mdk/iar_startup_nrf5340_application.s
mdk/iar_startup_nrf5340_network.s
mdk/iar_startup_nrf9160.s
mdk/ses_startup_nrf51.s
mdk/ses_startup_nrf52.s
mdk/ses_startup_nrf52805.s
mdk/ses_startup_nrf52810.s
mdk/ses_startup_nrf52811.s
mdk/ses_startup_nrf52820.s
mdk/ses_startup_nrf52833.s
mdk/ses_startup_nrf52840.s
mdk/ses_startup_nrf5340_application.s
mdk/ses_startup_nrf5340_network.s
mdk/ses_startup_nrf9160.s
mdk/ses_startup_nrf_common.s


mdk/gcc_startup_nrf51.S
mdk/gcc_startup_nrf52.S
mdk/gcc_startup_nrf52805.S
mdk/gcc_startup_nrf52810.S
mdk/gcc_startup_nrf52811.S
mdk/gcc_startup_nrf52820.S
mdk/gcc_startup_nrf52833.S
mdk/gcc_startup_nrf5340_application.S
mdk/gcc_startup_nrf5340_network.S
mdk/gcc_startup_nrf9160.S

mdk/nrf51422_xxaa.ld
mdk/nrf51422_xxab.ld
mdk/nrf51422_xxac.ld
mdk/nrf51801_xxab.ld
mdk/nrf51802_xxaa.ld
mdk/nrf51822_xxaa.ld
mdk/nrf51822_xxab.ld
mdk/nrf51822_xxac.ld
mdk/nrf51824_xxaa.ld
mdk/nrf51_common.ld
mdk/nrf51_xxaa.ld
mdk/nrf51_xxab.ld
mdk/nrf51_xxac.ld
mdk/nrf52805_xxaa.ld
mdk/nrf52810_xxaa.ld
mdk/nrf52811_xxaa.ld
mdk/nrf52820_xxaa.ld
mdk/nrf52832_xxaa.ld
mdk/nrf52832_xxab.ld
mdk/nrf52833_xxaa.ld
mdk/nrf52840_xxaa.ld
mdk/nrf52_common.ld
mdk/nrf52_xxaa.ld
mdk/nrf5340_xxaa_application.ld
mdk/nrf5340_xxaa_network.ld
mdk/nrf9160_xxaa.ld
mdk/nrf_common.ld
'''
