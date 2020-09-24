# Nordic Common Driver Build Definition

nordic_public_include_dirs += include_directories('.')

# TODO: split these into individual driver deps? That way you aren't compiling all of them?
nrf_common_drivers_dep = declare_dependency(
	include_directories: include_directories('.'),
	sources: [
		'helpers/gpio_helper.cpp',
		'helpers/twi_helper.cpp',
		'helpers/twim_helper.cpp',
	],
)
