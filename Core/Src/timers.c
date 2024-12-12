//
// Created by ryanm on 12/9/2024.
//

#include "timers.h"

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

static void Error_Handler() {
    for (;;) {}
}

// Initializes the timer that tracks total elapsed time in 0.001-second intervals
void Timer_Elapsed_Init(void) {
    __HAL_RCC_TIM2_CLK_ENABLE();

    htim2.Instance = TIM2;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    htim2.Init.Prescaler = (SystemCoreClock / 1000) - 1;
    htim2.Init.Period = UINT32_MAX;

    if (HAL_TIM_Base_Init(&htim2) != HAL_OK) Error_Handler();

}

// Initializes the timer that triggers an interrupt every 3 seconds
void Timer_Interrupt_Init(void) {
    __HAL_RCC_TIM3_CLK_ENABLE();

    htim3.Instance = TIM3;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    htim3.Init.Prescaler = (SystemCoreClock / 1000) - 1; // 1 kHz
    htim3.Init.Period = 3000 - 1; // 3 seconds at 1 kHz

    if (HAL_TIM_Base_Init(&htim3) != HAL_OK) Error_Handler();

    // Configure NVIC for TIM3
    HAL_NVIC_SetPriority(TIM3_IRQn, 2, 0); // Set priority as needed
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim3);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM3) {
        move_piece(0, -1);
    }
}

// Starts the elapsed time timer
void Timer_Elapsed_Start(void) {
    HAL_StatusTypeDef status = HAL_TIM_Base_Start(&htim2);
    if (status != HAL_OK) Error_Handler();
}

// Stops the elapsed time timer
void Timer_Elapsed_Stop(void) {
    if (HAL_TIM_Base_Stop(&htim2) != HAL_OK) Error_Handler();

}

// Resets the elapsed time timer to 0
void Timer_Elapsed_Reset(void) {
    __HAL_TIM_SET_COUNTER(&htim2, 0);
}

// Returns the total elapsed time in seconds as two values
void Timer_Elapsed_GetTime(uint32_t *seconds, uint32_t *ms) {
    uint32_t ticks = __HAL_TIM_GET_COUNTER(&htim2);
    *seconds = ticks / 1000;
    *ms = ticks % 1000;
}

// Starts the 3-second interrupt timer
void Timer_Interrupt_Start(void) {
    if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK) Error_Handler();
}

void Timer_Interrupt_Stop(void) {
    if (HAL_TIM_Base_Stop_IT(&htim3) != HAL_OK) Error_Handler();
}

void Timer_Interrupt_Reset(void) {
    __HAL_TIM_SET_COUNTER(&htim3, htim3.Instance->ARR);
}
