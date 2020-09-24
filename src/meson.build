# Nordic Processor Build Definitions

nordic_private_include_dirs = [
	include_directories('include'),
	include_directories('mdk', is_system: true)
]

nordic_public_include_dirs = []

# Pick up common drivers first
subdir('drivers')

subdir('freertos')
subdir('nrf52840')
