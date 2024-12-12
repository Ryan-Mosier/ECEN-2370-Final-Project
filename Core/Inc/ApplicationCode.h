/*
 * ApplicationCode.h
 *
 *  Created on: Dec 30, 2023
 *      Author: Xavion
 */

#include "LCD_Driver.h"
#include "stm32f4xx_hal.h"
#include "game.h"
#include "timers.h"
#include "button.h"

#include <stdio.h>

#ifndef INC_APPLICATIONCODE_H_
#define INC_APPLICATIONCODE_H_


typedef enum {
    GAME_STATE_MAIN_MENU, // Main menu or splash screen
    GAME_STATE_PLAYING,   // Game is in progress
    GAME_STATE_END        // Game has ended
} GameState;

extern GameState state;

void ApplicationInit(void);

void Splash_Screen();

void End_Screen();

void gameLoop();

void LCD_Visual_Demo(void);

#if (COMPILE_TOUCH_FUNCTIONS == 1) && (COMPILE_TOUCH_INTERRUPT_SUPPORT == 0)
void LCD_Touch_Polling_Demo(void);

#endif // (COMPILE_TOUCH_FUNCTIONS == 1) && (COMPILE_TOUCH_INTERRUPT_SUPPORT == 0)

#endif /* INC_APPLICATIONCODE_H_ */
