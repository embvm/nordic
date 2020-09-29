# meson build for nrf52840 USB dongle HW platform

# TODO
#nrf52840_freertos_lib = static_library('nrf52840_freertos',
#    dependencies: [
#        nrf52_freertos_dep,
#        framework_include_dep
#    ],
#    native: false
#)

# TODO
#nrf52840_freertos_dep = declare_dependency(
#    link_with: nrf52840_freertos_lib,
#    include_directories: freertos_include_dir,
#)

nrf52840 = static_library('nrf52840',
    sources: [
    	'nrf52840.cpp',
    ],
    include_directories: [
    	nordic_private_include_dirs,
		cmsis_cortex_m_include,
    ],
    dependencies: [
        nrf_common_drivers_dep,
        arm_dep,
        nrfx_nrf52840_dep,
        framework_include_dep
    ],
    native: false
)

# TODO: does this need to include framework_include_dep?
nrf52840_processor_dep = declare_dependency(
	include_directories: [
		include_directories('.'),
        nordic_public_include_dirs,
	],
    dependencies: [
        # TODO: Needed? Replaced "arm_common_includes" - which is the arm utilities folder
        arm_common_utilties_dep
    ],
	compile_args: ['-DNRF52840_XXAA'],
	link_with: nrf52840,
)
