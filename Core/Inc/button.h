//
// Created by ryanm on 12/10/2024.
//

#ifndef RMOSIERFINALLAB_BUTTON_H
#define RMOSIERFINALLAB_BUTTON_H

#include "stdbool.h"
#include "game.h"
#include "stm32f4xx_hal.h"

#define USR_BUTTON_GPIO GPIOA
#define USR_BUTTON_PIN GPIO_PIN_0
#define BUTTON_PRESSED 1
#define BUTTON_NOT_PRESSED 0
#define BUTTON_IRQ_NUMBER EXTI0_IRQn

void BUTTON_Init();

bool BUTTON_is_Pressed();

#endif //RMOSIERFINALLAB_BUTTON_H
