//
// Created by ryanm on 12/10/2024.
//
#include "RNG.h"

static void Error_Handler() {
    for (;;) {}
}

static RNG_HandleTypeDef hrng;

void RNG_Init() {
    __HAL_RCC_RNG_CLK_ENABLE();
    hrng.Instance = RNG;

    if (HAL_RNG_Init(&hrng) != HAL_OK) {
        Error_Handler();
    }
}

uint32_t RNG_GetRandomNumber() {
    uint32_t ret;
    HAL_RNG_GenerateRandomNumber(&hrng, &ret);
    return ret;
}

HAL_StatusTypeDef RNG_IsReady() {
    HAL_RNG_StateTypeDef state = HAL_RNG_GetState(&hrng);
    if (state == HAL_RNG_STATE_READY) {
        return HAL_OK;
    }
    return HAL_BUSY;
}
