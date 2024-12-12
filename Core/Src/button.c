//
// Created by ryanm on 12/10/2024.
//
#include "button.h"

void BUTTON_Init() {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef config = {0};
    config.Pin = USR_BUTTON_PIN;
    config.Mode = GPIO_MODE_IT_RISING;
    config.Pull = GPIO_PULLDOWN;
    config.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(USR_BUTTON_GPIO, &config);

    HAL_NVIC_SetPriority(BUTTON_IRQ_NUMBER, 1, 0);
    HAL_NVIC_EnableIRQ(BUTTON_IRQ_NUMBER);
}

bool BUTTON_is_Pressed() {
    return HAL_GPIO_ReadPin(USR_BUTTON_GPIO, USR_BUTTON_PIN) == GPIO_PIN_SET;
}

void EXTI0_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(USR_BUTTON_PIN);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == USR_BUTTON_PIN) {
        rotate_piece(ROTATION_90);
    }
}
