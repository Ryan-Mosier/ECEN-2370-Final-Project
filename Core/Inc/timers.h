//
// Created by ryanm on 12/9/2024.
//

#ifndef RMOSIERFINALLAB_TIMERS_H
#define RMOSIERFINALLAB_TIMERS_H

#include "game.h"
#include "stm32f4xx_hal.h"

// Initializes the timer that tracks total elapsed time in 0.01-second intervals
void Timer_Elapsed_Init(void);

// Initializes the timer that triggers an interrupt every 3 seconds
void Timer_Interrupt_Init(void);

// Starts the elapsed time timer
void Timer_Elapsed_Start(void);

// Stops the elapsed time timer
void Timer_Elapsed_Stop(void);

// Resets the elapsed time timer to 0
void Timer_Elapsed_Reset(void);

// Returns the total elapsed time in seconds as a float
void Timer_Elapsed_GetTime(uint32_t *seconds, uint32_t *ms);

// Starts the 3-second interrupt timer
void Timer_Interrupt_Start(void);

// Stops the 3-second interrupt timer
void Timer_Interrupt_Stop(void);

void Timer_Interrupt_Reset(void);

// Callback function for the 3-second timer interrupt
void Timer_Interrupt_Callback(void);

#endif //RMOSIERFINALLAB_TIMERS_H
