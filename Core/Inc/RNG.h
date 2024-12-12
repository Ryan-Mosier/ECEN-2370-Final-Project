//
// Created by ryanm on 12/10/2024.
//

#ifndef RMOSIERFINALLAB_RNG_H
#define RMOSIERFINALLAB_RNG_H

#include <stdbool.h>
#include "stm32f4xx_hal.h"

void RNG_Init();

uint32_t RNG_GetRandomNumber();

HAL_StatusTypeDef RNG_IsReady();


#endif //RMOSIERFINALLAB_RNG_H
