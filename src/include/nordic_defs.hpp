#ifndef NORDIC_DEFINITIONS_HPP_
#define NORDIC_DEFINITIONS_HPP_

// This is the implicit behavior already, but we want to make it explicit to
// eliminate compiler warnings.
#ifndef __CORTEX_M
#define __CORTEX_M 0
#endif

#if __CORTEX_M == (0x00U)
#define NRF_IRQ_PRIOR_MAX 0
#define NRF_IRQ_PRIOR_HIGH 1
#define NRF_IRQ_PRIOR_MID 2
#define NRF_IRQ_PRIOR_LOW 3
#define NRF_IRQ_PRIOR_LOWEST 3
#define NRF_IRQ_PRIOR_THREAD 4
#elif __CORTEX_M == (0x04U)
#define NRF_IRQ_PRIOR_MAX 0
#define NRF_IRQ_PRIOR_HIGH 2
#define NRF_IRQ_PRIOR_MID 4
#define NRF_IRQ_PRIOR_LOW 6
#define NRF_IRQ_PRIOR_LOWEST 8
#define NRF_IRQ_PRIOR_THREAD 15
#else
#error "No platform defined"
#endif

#endif // NORDIC_DEFINITIONS_HPP_
