# meson build for nrf52840 USB dongle HW platform

nrf52840_freertos_lib = static_library('nrf52840_freertos',
    dependencies: nrf52_freertos_dep,
    include_directories: inc_framework, # needed for utilities/ include
    native: false
)

nrf52840_freertos_dep = declare_dependency(
    link_with: nrf52840_freertos_lib,
    include_directories: freertos_include_dir,
)

nrf52840 = static_library('nrf52840',
    [
    	'nrf52840.cpp',
    	'../mdk/gcc_startup_nrf52840.S',
    	'../mdk/system_nrf52840.c',
    ],
    include_directories: [
    	nordic_private_include_dirs,
		cmsis_corem_include,
		inc_framework,
        libc_header_include,
    ],
    c_args: [
    	'-DCONFIG_GPIO_AS_PINRESET',
    	'-DNRF52840_XXAA',
    ],
    cpp_args: [
        '-DNRF52840_XXAA',
        # TODO: Refactor so this isn't needed
        '-DDEBUG_ASSERT_NO_STDIO',
    ],
    dependencies: [
        nrf_common_drivers_dep,
        libcxx_header_include_dep,
        arm_dep,
    ],
    native: false
)

nrf52840_processor_dep = declare_dependency(
	include_directories: [
		include_directories('.'),
        nordic_public_include_dirs,
		inc_framework,
        arm_common_includes
	],
	compile_args: ['-DNRF52840_XXAA'],
	link_args: [
		# So the application linker script can pick up gcc_arm_common.ld
		'-L' + arm_directory_root,
	],
	link_with: nrf52840,
)
