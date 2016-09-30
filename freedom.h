//
// freedom.h -- Definitions for Freescale Freedom development board
//
//  Copyright (c) 2012-2013 Andrew Payne <andy@payne.org>
//

#include "MKL25Z4.h"                    // CPU definitions

#define CORE_CLOCK          48000000    // Core clock speed
#define SystemCoreClock CORE_CLOCK

static inline void RGB_LED(int red, int green, int blue) {
//    TPM2->C0V  = red;
//    TPM2->C1V  = green;
//    TPM0->C1V  = blue;
}
