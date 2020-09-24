# NRF52 Examples

vendor_sdk_path = get_option('vendor-sdk-path')

assert(vendor_sdk_path != '', 'vendor-sdk-path must point to the nordic SDK. ' +
	'Please unzip the SDK in the sdk/ folder, and configure meson with ' +
	' -Dvendor-sdk-path=../../../sdk/nRF5_SDK_15.3.0_59ac345')

blinky_app = executable('nrf5284_dongle_blinky',
	sources: [
		'blinky.c',
		vendor_sdk_path + '/modules/nrfx/mdk/gcc_startup_nrf52840.S',
		vendor_sdk_path + '/components/libraries/log/src/nrf_log_frontend.c',
		vendor_sdk_path + '/components/libraries/log/src/nrf_log_str_formatter.c',
		vendor_sdk_path + '/components/boards/boards.c',
		vendor_sdk_path + '/components/libraries/util/app_error.c',
		vendor_sdk_path + '/components/libraries/util/app_error_handler_gcc.c',
		vendor_sdk_path + '/components/libraries/util/app_error_weak.c',
		vendor_sdk_path + '/components/libraries/util/app_util_platform.c',
		vendor_sdk_path + '/components/libraries/util/nrf_assert.c',
		vendor_sdk_path + '/components/libraries/atomic/nrf_atomic.c',
		vendor_sdk_path + '/components/libraries/balloc/nrf_balloc.c',
		vendor_sdk_path + '/external/fprintf/nrf_fprintf.c',
		vendor_sdk_path + '/external/fprintf/nrf_fprintf_format.c',
		vendor_sdk_path + '/components/libraries/memobj/nrf_memobj.c',
		vendor_sdk_path + '/components/libraries/ringbuf/nrf_ringbuf.c',
		vendor_sdk_path + '/components/libraries/strerror/nrf_strerror.c',
		vendor_sdk_path + '/modules/nrfx/soc/nrfx_atomic.c',
		vendor_sdk_path + '/modules/nrfx/mdk/system_nrf52840.c',
	],
	include_directories: include_directories(
		  vendor_sdk_path + '/components',
		  vendor_sdk_path + '/modules/nrfx/mdk',
		  vendor_sdk_path + '/components/softdevice/mbr/nrf52840/headers',
		  vendor_sdk_path + '/components/libraries/strerror',
		  vendor_sdk_path + '/components/toolchain/cmsis/include',
		  vendor_sdk_path + '/components/libraries/util',
		  vendor_sdk_path + '/components/libraries/balloc',
		  vendor_sdk_path + '/components/libraries/ringbuf',
		  vendor_sdk_path + '/modules/nrfx/hal',
		  vendor_sdk_path + '/components/libraries/bsp',
		  vendor_sdk_path + '/components/libraries/log',
		  vendor_sdk_path + '/modules/nrfx',
		  vendor_sdk_path + '/components/libraries/experimental_section_vars',
		  vendor_sdk_path + '/components/libraries/delay',
		  vendor_sdk_path + '/integration/nrfx',
		  vendor_sdk_path + '/components/drivers_nrf/nrf_soc_nosd',
		  vendor_sdk_path + '/components/libraries/atomic',
		  vendor_sdk_path + '/components/boards',
		  vendor_sdk_path + '/components/libraries/memobj',
		  vendor_sdk_path + '/external/fprintf',
		  vendor_sdk_path + '/components/libraries/log/src',
	),
	c_args: [
		'-DBOARD_PCA10059',
		'-DBSP_DEFINES_ONLY',
		'-DCONFIG_GPIO_AS_PINRESET',
		'-DFLOAT_ABI_HARD',
		'-DMBR_PRESENT',
		'-DNRF52840_XXAA',
		'-ffunction-sections',
		'-fdata-sections',
		'-fno-strict-aliasing',
		'-fshort-enums',
		'-fno-builtin',
		'-g3',
		'-D__HEAP_SIZE=8192',
		'-D__STACK_SIZE=8192',
	],
	link_args: [
		'-L' + vendor_sdk_path + '/modules/nrfx/mdk',
		'-L' + meson.current_source_dir(),
		'-Tblinky_gcc_nrf52.ld',
		# let linker dump unused sections
		'-Wl,--gc-sections',
		# use newlib in nano version
		'--specs=nano.specs',
		'-lc',
		'-lnosys',
		'-lm',
		'-Wl,-Map=nrf5284_dongle_blinky.map'
	],
	native: false,
	build_by_default: false
)

blinky_migratory_dongle_app = executable('blinky_migratory_dongle',
	sources: [
		'blinky_migratory.cpp',
		vendor_sdk_path + '/modules/nrfx/mdk/gcc_startup_nrf52840.S',
		vendor_sdk_path + '/modules/nrfx/mdk/system_nrf52840.c'
	],
	include_directories: include_directories(
		  vendor_sdk_path + '/components',
		  vendor_sdk_path + '/modules/nrfx/mdk',
		  vendor_sdk_path + '/components/softdevice/mbr/nrf52840/headers',
		  vendor_sdk_path + '/components/libraries/strerror',
		  vendor_sdk_path + '/components/toolchain/cmsis/include',
		  vendor_sdk_path + '/components/libraries/util',
		  vendor_sdk_path + '/components/libraries/balloc',
		  vendor_sdk_path + '/components/libraries/ringbuf',
		  vendor_sdk_path + '/modules/nrfx/hal',
		  vendor_sdk_path + '/components/libraries/bsp',
		  vendor_sdk_path + '/components/libraries/log',
		  vendor_sdk_path + '/modules/nrfx',
		  vendor_sdk_path + '/components/libraries/experimental_section_vars',
		  vendor_sdk_path + '/components/libraries/delay',
		  vendor_sdk_path + '/integration/nrfx',
		  vendor_sdk_path + '/components/drivers_nrf/nrf_soc_nosd',
		  vendor_sdk_path + '/components/libraries/atomic',
		  vendor_sdk_path + '/components/boards',
		  vendor_sdk_path + '/components/libraries/memobj',
		  vendor_sdk_path + '/external/fprintf',
		  vendor_sdk_path + '/components/libraries/log/src',
	),
	c_args: [
		'-DBOARD_PCA10059',
		'-DBSP_DEFINES_ONLY',
		'-DCONFIG_GPIO_AS_PINRESET',
		'-DFLOAT_ABI_HARD',
		'-DMBR_PRESENT',
		'-DNRF52840_XXAA',
		'-ffunction-sections',
		'-fdata-sections',
		'-fno-strict-aliasing',
		'-fshort-enums',
		'-fno-builtin',
		'-g3',
		'-D__HEAP_SIZE=8192',
		'-D__STACK_SIZE=8192',
	],
	cpp_args: [
		'-DBOARD_PCA10059',
		'-DBSP_DEFINES_ONLY',
		'-DCONFIG_GPIO_AS_PINRESET',
		'-DFLOAT_ABI_HARD',
		'-DMBR_PRESENT',
		'-DNRF52840_XXAA',
		'-ffunction-sections',
		'-fdata-sections',
		'-fno-strict-aliasing',
		'-fshort-enums',
		'-fno-builtin',
		'-g3',
		'-D__HEAP_SIZE=8192',
		'-D__STACK_SIZE=8192',
	],
	link_args: [
		'-L' + vendor_sdk_path + '/modules/nrfx/mdk',
		'-L' + meson.current_source_dir(),
		'-Tblinky_gcc_nrf52.ld',
		# let linker dump unused sections
		'-Wl,--gc-sections',
		'-Wl,-Map=nrf5284_dongle_blinky.map',
				'--specs=nano.specs',
		'-lc',
		'-lnosys',
		'-lm',
	],
	dependencies: [
		framework_migratory_dep,
		blinky_nrf52dongle_migratory_platform_dep
	],
)

target_objcopy = find_program(meson.get_cross_property('objcopy'))

blinky_app_hex = custom_target('nrf5284_dongle_blinky.hex',
	input: blinky_app,
	output: 'nrf5284_dongle_blinky.hex',
	command: [target_objcopy, '-O', 'ihex', '@INPUT@', '@OUTPUT@'],
	depends: blinky_app,
	build_by_default: false
)

blinky_app_bins = custom_target('nrf5284_dongle_blinky.bin',
	input: blinky_app,
	output: 'nrf5284_dongle_blinky.bin',
	command: [target_objcopy, '-O', 'binary', '@INPUT@', '@OUTPUT@'],
	depends: blinky_app,
	build_by_default: false
)

blinky_migratory_dongle_hex = custom_target('blinky_migratory_dongle.hex',
	input: blinky_migratory_dongle_app,
	output: 'blinky_migratory_dongle.hex',
	command: [target_objcopy, '-O', 'ihex', '@INPUT@', '@OUTPUT@'],
	depends: blinky_migratory_dongle_app,
	build_by_default: false
)

blinky_migratory_dongle_bin = custom_target('blinky_migratory_dongle.bin',
	input: blinky_migratory_dongle_app,
	output: 'blinky_migratory_dongle.bin',
	command: [target_objcopy, '-O', 'binary', '@INPUT@', '@OUTPUT@'],
	depends: blinky_migratory_dongle_app,
	build_by_default: false
)
