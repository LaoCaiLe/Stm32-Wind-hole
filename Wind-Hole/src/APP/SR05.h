#ifndef _SR05_H
#define _SR05_H

#include "stm32f10x_rcc.h"
#include "SysTick.h"
#include "input.h"
// #include "GPIOLIKE51.h"
// #include "sys.h"

#define Trig PBin(0)
#define Echo PBin(1)

void SR05_Init(void);
void SR05_start(void);
double get_distance(void);
#endif


