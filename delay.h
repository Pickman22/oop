#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>
#include "MKL25Z4.h"
#include "reg.h"

#define IS_TIMER_EXPIRED()  (LPTMR_CSR_TCF_MASK & register32_read(&LPTMR0->CSR))

void delay_ms(uint32_t ms) {
    /* Enable software access to LPTMR.  */
    register32_set_bits(&SIM->SCGC5, SIM_SCGC5_LPTMR_MASK);
    register32_write(&LPTMR0->CSR, 0U);

    /* LPTMR is only 16-bit. TODO: Implement prescaler? */
    ms = (ms > 0xFFFFU) ? 0xFFFFU : ms;
    register32_write(&LPTMR0->CMR, ms);

    /* Bypass glitch filter and select 1Kh clock as input. */
    register32_clear_then_set_bits(&LPTMR0->PSR, LPTMR_PSR_PCS_MASK |
                                   LPTMR_PSR_PBYP_MASK, LPTMR_PSR_PCS(1U) |
                                   LPTMR_PSR_PBYP(1U));

    /* Enable timer. */
    register32_set_bits(&LPTMR0->CSR, LPTMR_CSR_TEN_MASK);
    while(!IS_TIMER_EXPIRED());
}

#endif // DELAY_H
