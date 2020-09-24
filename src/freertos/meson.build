# Nordic FreeRTOS Port

nrf51_freertos_dep = declare_dependency(
    sources: files(
		'nrf51/port.c',
		'nrf51/port_cmsis.c',
		'nrf51/port_cmsis_systick.c',
	),
    include_directories:
    [
    	include_directories('.'),
    	cmsis_corem_include,
    	nordic_private_include_dirs,
    	include_directories('nrf51'),
    ],
    dependencies: freertos_core_dep,
)

nrf52_freertos_dep = declare_dependency(
    sources: files(
		'nrf52/port.c',
		'nrf52/port_cmsis.c',
		'nrf52/port_cmsis_systick.c',
	),
    include_directories:
    [
	    include_directories('.'),
    	cmsis_corem_include,
    	nordic_private_include_dirs,
    	include_directories('nrf52'),
    ],
    dependencies: freertos_core_dep,
)
