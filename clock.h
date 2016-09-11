#ifndef CLOCK_H
#define CLOCK_H

#include "MKL25Z4.h"

#define SYSTEM_CORE_CLOCK 48000000U

#define SYSTEM_BUS_CLOCK()   (SYSTEM_CORE_CLOCK / ())

#endif // CLOCK_H
