#include "nrf52840.hpp"
#include <cassert>
#include <processor_architecture.hpp>
#include <processor_includes.hpp>

// TODO: volatile load-store-ify

#pragma mark - Definitions -

static const uint32_t CPU_FREQ_MHZ = UINT32_C(64);

// TODO: implement? why was this here?
// extern "C" void _putchar(char c) {}

#pragma mark - Helpers -

static inline void setWEN() noexcept
{
	NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen;
	while(NRF_NVMC->READY == NVMC_READY_READY_Busy)
	{
	}
}

static inline void setREN() noexcept
{
	NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren;
	while(NRF_NVMC->READY == NVMC_READY_READY_Busy)
	{
	}
}

static inline void setVOUT(uint32_t setting) noexcept
{
	setWEN();
	NRF_UICR->REGOUT0 = (NRF_UICR->REGOUT0 & ~((uint32_t)UICR_REGOUT0_VOUT_Msk)) |
						(setting << UICR_REGOUT0_VOUT_Pos);
	setREN();

	// System reset is needed to update UICR registers.
	ProcessorArch::systemReset();
}

static inline bool checkVOUT(uint32_t setting) noexcept
{
	return (NRF_UICR->REGOUT0 & UICR_REGOUT0_VOUT_Msk) == (setting << UICR_REGOUT0_VOUT_Pos);
}

#pragma mark - Interface Functions -

nRF52840::~nRF52840() {}

void nRF52840::init_() noexcept {}

void nRF52840::reset_() noexcept
{
	ProcessorArch::systemReset();
}

bool nRF52840::highVoltageMode() noexcept
{
	return (NRF_POWER->MAINREGSTATUS &
			(POWER_MAINREGSTATUS_MAINREGSTATUS_High << POWER_MAINREGSTATUS_MAINREGSTATUS_Pos));
}

bool nRF52840::highVoltageMode(bool en) noexcept
{
	// Configure UICR_REGOUT0 register only if it is set to default value.
	if(en && checkVOUT(UICR_REGOUT0_VOUT_DEFAULT))
	{
		setVOUT(UICR_REGOUT0_VOUT_3V0);
	}
	// else if disabling high voltage mode and we _aren't_ default, we need to restore it
	else if(!checkVOUT(UICR_REGOUT0_VOUT_DEFAULT))
	{
		setVOUT(UICR_REGOUT0_VOUT_DEFAULT);
	}

	return en;
}

void nRF52840::spin(unsigned int usecs) noexcept
{
	if(usecs != 0)
	{
		uint32_t time_cycles = usecs * CPU_FREQ_MHZ;

		// Save the current state of the DEMCR register to be able to restore it before exiting
		// this function. Enable the trace and debug blocks (including DWT).
		uint32_t core_debug = CoreDebug->DEMCR;
		CoreDebug->DEMCR = core_debug | CoreDebug_DEMCR_TRCENA_Msk;

		// Save the current state of the CTRL register in the DWT block. Make sure
		// that the cycle counter is enabled.
		uint32_t dwt_ctrl = DWT->CTRL;
		DWT->CTRL = dwt_ctrl | DWT_CTRL_CYCCNTENA_Msk;

		// Store start value of the cycle counter.
		uint32_t cyccnt_initial = DWT->CYCCNT;

		// Delay required time.
		while((DWT->CYCCNT - cyccnt_initial) < time_cycles)
		{
		}

		// Restore preserved registers.
		DWT->CTRL = dwt_ctrl;
		CoreDebug->DEMCR = core_debug;
	}
}
