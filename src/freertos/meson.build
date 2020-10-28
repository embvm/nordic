# Nordic FreeRTOS Port

# TODO: How can we override our port/config file? Right now they're forced to use ours
# TODO: I'd like to refactor the way this build is handled so that we're not exposing the nRF MDK
# headers to other modules. This might mean that we need to pull in freertos dependencies into
# this file, where we then build a static library and link to the final application.
#
# The problem is that FreeRTOS Config.h needs nRF52840 headers...

nrf51_freertos_dep = declare_dependency(
    sources: files(
		'nrf51/port.c',
		'nrf51/port_cmsis.c',
		'nrf51/port_cmsis_systick.c',
	),
    include_directories:
    [
    	include_directories('.'),
    	cmsis_cortex_m_include,
    	nordic_private_include_dirs,
    	include_directories('nrf51'),
    ],
    # TODO: is this really needed? Or can we just say: you need to depend on freertos_core_dep and
    # a processor specific dep?
    # dependencies: freertos_core_dep,
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
    	cmsis_cortex_m_include,
    	nordic_private_include_dirs,
        nrfx_includes, # TODO: REMOVE when I figure out how
    	include_directories('nrf52'),
    ],
    # TODO: is this really needed? Or can we just say: you need to depend on freertos_core_dep and
    # a processor specific dep?
    # dependencies: freertos_core_dep,
)
