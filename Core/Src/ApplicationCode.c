/*
 * ApplicationCode.c
 *
 *  Created on: Dec 30, 2023 (updated 11/12/2024) Thanks Donavon! 
 *      Author: Xavion
 */

#include "ApplicationCode.h"


/* Static variables */
GameState state;

extern void initialise_monitor_handles(void);

#if COMPILE_TOUCH_FUNCTIONS == 1
static STMPE811_TouchData StaticTouchData;
#if TOUCH_INTERRUPT_ENABLED == 1
static EXTI_HandleTypeDef LCDTouchIRQ;

void LCDTouchScreenInterruptGPIOInit(void);

#endif // TOUCH_INTERRUPT_ENABLED
#endif // COMPILE_TOUCH_FUNCTIONS

void ApplicationInit(void) {
    initialise_monitor_handles(); // Allows printf functionality
    LTCD__Init();
    LTCD_Layer_Init(0);
    LCD_Clear(0, LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_SetFont(&Font16x24);
    state = GAME_STATE_MAIN_MENU;
    BUTTON_Init();

#if COMPILE_TOUCH_FUNCTIONS == 1
    InitializeLCDTouch();

    // This is the orientation for the board to be direclty up where the buttons are vertically above the screen
    // Top left would be low x value, high y value. Bottom right would be low x value, low y value.
    StaticTouchData.orientation = STMPE811_Orientation_Portrait_2;

#if TOUCH_INTERRUPT_ENABLED == 1
    LCDTouchScreenInterruptGPIOInit();
#endif // TOUCH_INTERRUPT_ENABLED

#endif // COMPILE_TOUCH_FUNCTIONS
}

//essential code for rendering a game board
//it is all inline, so it doesn't really matter that it is in multiple places
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define SQUARE_WIDTH (LCD_PIXEL_WIDTH / BOARD_WIDTH)
#define SQUARE_HEIGHT (LCD_PIXEL_HEIGHT / BOARD_HEIGHT)
#define SQUARE_SIZE (MIN((LCD_PIXEL_WIDTH) / (BOARD_WIDTH), (LCD_PIXEL_HEIGHT) / (BOARD_HEIGHT)))
#define MAX_LOGICAL_Y (BOARD_HEIGHT - 1)

static inline uint16_t scale_4_LCD_H(uint16_t num) {
    return ((MAX_LOGICAL_Y - num) * SQUARE_SIZE);
}

static inline uint16_t scale_4_LCD_W(uint16_t num) {
    return (num * SQUARE_SIZE);
}

#define CHARACTER_HEIGHT 8

#define START_BUTTON_Y         (LCD_PIXEL_HEIGHT / 2)
#define START_BUTTON_HEIGHT    50
#define START_BUTTON_LENGTH    100

#define START_BUTTON_TOP       (START_BUTTON_Y - (START_BUTTON_HEIGHT / 2))
#define START_BUTTON_BOTTOM    (START_BUTTON_Y + (START_BUTTON_HEIGHT / 2))
#define START_BUTTON_LEFT      ((LCD_PIXEL_WIDTH / 2) - (START_BUTTON_LENGTH / 2))
#define START_BUTTON_RIGHT     ((LCD_PIXEL_WIDTH / 2) + (START_BUTTON_LENGTH / 2))


void LCD_DrawCenteredButton(uint16_t y, uint16_t height, uint16_t length, const char *text, uint16_t color) {
    LCD_Draw_Square_Fill(LCD_PIXEL_WIDTH / 2, y, height, length, color);
    LCD_DisplayStringCentered(y - CHARACTER_HEIGHT, text);
}


// Define colors for each piece (as per your block definitions)
#define COLOR_I   LCD_COLOR_CYAN
#define COLOR_O   LCD_COLOR_YELLOW
#define COLOR_T   LCD_COLOR_MAGENTA
#define COLOR_S   LCD_COLOR_GREEN
#define COLOR_Z   LCD_COLOR_RED
#define COLOR_L   LCD_COLOR_ORANGE
#define COLOR_J   LCD_COLOR_BLUE

void Splash_Screen() {
    GameBoard Splash;

    for (int i = 0; i < BOARD_WIDTH; i++) {
            for (int j = 0; j < BOARD_HEIGHT; j++) {
                Splash.static_blocks[i][j].color = LCD_COLOR_BLACK;
            }
        }
    // ----- I Piece (Vertical Line) -----
    // Positioned towards the left side
    Splash.static_blocks[4][1].color = COLOR_I;
    Splash.static_blocks[5][1].color = COLOR_I;
    Splash.static_blocks[6][1].color = COLOR_I;
    Splash.static_blocks[7][1].color = COLOR_I;

    // ----- O Piece (Square) -----
    // Positioned in the center-left
    Splash.static_blocks[2][5].color = COLOR_O;
    Splash.static_blocks[2][6].color = COLOR_O;
    Splash.static_blocks[3][5].color = COLOR_O;
    Splash.static_blocks[3][6].color = COLOR_O;

    // ----- T Piece -----
    // Positioned towards the center-right
    Splash.static_blocks[6][11].color = COLOR_T;
    Splash.static_blocks[7][11].color = COLOR_T;
    Splash.static_blocks[8][11].color = COLOR_T;
    Splash.static_blocks[7][12].color = COLOR_T;

    // ----- S Piece -----
    // Positioned towards the top-right
    Splash.static_blocks[7][3].color = COLOR_S;
    Splash.static_blocks[8][3].color = COLOR_S;
    Splash.static_blocks[6][4].color = COLOR_S;
    Splash.static_blocks[7][4].color = COLOR_S;

    // ----- Z Piece -----
    // Positioned below the S piece
    Splash.static_blocks[6][6].color = COLOR_Z;
    Splash.static_blocks[7][6].color = COLOR_Z;
    Splash.static_blocks[7][7].color = COLOR_Z;
    Splash.static_blocks[8][7].color = COLOR_Z;

    // ----- L Piece -----
    // Positioned towards the bottom-left
    Splash.static_blocks[2][10].color = COLOR_L;
    Splash.static_blocks[2][11].color = COLOR_L;
    Splash.static_blocks[3][11].color = COLOR_L;
    Splash.static_blocks[4][11].color = COLOR_L;

    // ----- J Piece -----
    // Positioned towards the bottom-right
    Splash.static_blocks[6][15].color = COLOR_J;
    Splash.static_blocks[7][15].color = COLOR_J;
    Splash.static_blocks[7][16].color = COLOR_J;
    Splash.static_blocks[8][16].color = COLOR_J;

    // ----- Drawing on LCD -----
    LCD_Clear(0, LCD_COLOR_BLACK);

    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            if (Splash.static_blocks[i][j].color != LCD_COLOR_BLACK) {
                LCD_Draw_Square_Fill(
                    scale_4_LCD_W(i) + (SQUARE_SIZE / 2),
                    scale_4_LCD_H(j) + (SQUARE_SIZE / 2),
                    SQUARE_SIZE,
                    SQUARE_SIZE,
                    Splash.static_blocks[i][j].color
                );
            }
        }
    }

    // ----- Drawing the Start Button -----
    LCD_DrawCenteredButton(
        START_BUTTON_Y,
        START_BUTTON_HEIGHT,
        START_BUTTON_LENGTH,
        "Start",
        LCD_COLOR_GREEN
    );
}

#define Y_SINGLE        10
#define Y_DOUBLE        30
#define Y_TRIPLE        50
#define Y_TETRIS        70
#define Y_TIME          90
#define Y_TIME_NUMBER   110

void End_Screen() {
    Timer_Elapsed_Stop();
    Timer_Interrupt_Stop();
    uint32_t seconds, ms;
    Timer_Elapsed_GetTime(&seconds, &ms);

    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_Clear(0, LCD_COLOR_BLACK);

    char buffer[32]; // Buffer to hold each formatted string

    // Display Singles
    snprintf(buffer, sizeof(buffer), "Singles: %d", singles);
    LCD_DisplayStringCentered(Y_SINGLE, buffer);

    // Display Doubles
    snprintf(buffer, sizeof(buffer), "Doubles: %d", doubles);
    LCD_DisplayStringCentered(Y_DOUBLE, buffer);

    // Display Triples
    snprintf(buffer, sizeof(buffer), "Triples: %d", triples);
    LCD_DisplayStringCentered(Y_TRIPLE, buffer);

    // Display Tetris
    snprintf(buffer, sizeof(buffer), "Tetris: %d", quads);
    LCD_DisplayStringCentered(Y_TETRIS, buffer);

    // Display Time Elapsed
    LCD_DisplayStringCentered(Y_TIME, "Time Elapsed:");
    snprintf(buffer, sizeof(buffer), "%lu.%lu", seconds, ms);
    LCD_DisplayStringCentered(Y_TIME_NUMBER, buffer);
}

void Start_Game() {
    LCD_Clear(0, LCD_COLOR_BLACK);
    game_init();
    Timer_Elapsed_Start();
    Timer_Interrupt_Start();
    render_game();
}

void LCD_Visual_Demo(void) {
    visualDemo();
}

#if COMPILE_TOUCH_FUNCTIONS == 1

void LCD_Touch_Polling_Demo(void) {
    LCD_Clear(0, LCD_COLOR_GREEN);
    while (1) {
        /* If touch pressed */
        if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
            /* Touch valid */
            printf("\nX: %03d\nY: %03d\n", StaticTouchData.x, StaticTouchData.y);
            LCD_Clear(0, LCD_COLOR_RED);
        } else {
            /* Touch not pressed */
            printf("Not Pressed\n\n");
            LCD_Clear(0, LCD_COLOR_GREEN);
        }
    }
}


// TouchScreen Interrupt
#if TOUCH_INTERRUPT_ENABLED == 1

void LCDTouchScreenInterruptGPIOInit(void) {
    GPIO_InitTypeDef LCDConfig = {0};
    LCDConfig.Pin = GPIO_PIN_15;
    LCDConfig.Mode = GPIO_MODE_IT_RISING_FALLING;
    LCDConfig.Pull = GPIO_NOPULL;
    LCDConfig.Speed = GPIO_SPEED_FREQ_HIGH;

    // Clock enable
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // GPIO Init      
    HAL_GPIO_Init(GPIOA, &LCDConfig);

    // Interrupt Configuration
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    LCDTouchIRQ.Line = EXTI_LINE_15;

}

#define TOUCH_DETECTED_IRQ_STATUS_BIT   (1 << 0)  // Touchscreen detected bitmask

static uint8_t statusFlag;

static inline bool is_within_range(int value, int lower_bound, int upper_bound) {
    // Ensure the lower_bound is less than or equal to upper_bound
    if (lower_bound > upper_bound) {
        int temp = lower_bound;
        lower_bound = upper_bound;
        upper_bound = temp;
    }
    return (value >= lower_bound && value <= upper_bound);
}

void EXTI15_10_IRQHandler() {
    HAL_NVIC_DisableIRQ(EXTI15_10_IRQn); // May consider making this a universial interrupt guard
    bool isTouchDetected = false;

    static uint32_t count;
    count = 0;
    uint8_t attempts = 0;
    while (count == 0) {
        if (attempts++ == UINT8_MAX) {
            HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
            HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
            return;
        }
        count = STMPE811_Read(STMPE811_FIFO_SIZE);
    }

    // Disable touch interrupt bit on the STMPE811
    uint8_t currentIRQEnables = ReadRegisterFromTouchModule(STMPE811_INT_EN);
    WriteDataToTouchModule(STMPE811_INT_EN, 0x00);

    // Clear the interrupt bit in the STMPE811
    statusFlag = ReadRegisterFromTouchModule(STMPE811_INT_STA);
    uint8_t clearIRQData = (statusFlag | TOUCH_DETECTED_IRQ_STATUS_BIT); // Write one to clear bit
    WriteDataToTouchModule(STMPE811_INT_STA, clearIRQData);

    uint8_t ctrlReg = ReadRegisterFromTouchModule(STMPE811_TSC_CTRL);
    if (ctrlReg & 0x80) {
        isTouchDetected = true;
    }

    // Determine if it is pressed or unpressed
    if (isTouchDetected) // Touch has been detected
    {
        DetermineTouchPosition(&StaticTouchData);
        if (state == GAME_STATE_MAIN_MENU) {
            //check if bounded on button
            bool within_x = is_within_range(StaticTouchData.x, START_BUTTON_RIGHT, START_BUTTON_LEFT);
            bool within_y = is_within_range(StaticTouchData.y, START_BUTTON_BOTTOM, START_BUTTON_TOP);
            if (within_x && within_y) {
                state = GAME_STATE_PLAYING;
                Start_Game();
            }
        } else if (state == GAME_STATE_PLAYING) {
        	if(StaticTouchData.y < 100){
        		while(can_move_piece(0,-1)){
        			move_piece(0,-1);
        		}
        	}
        	else if (StaticTouchData.x > LCD_PIXEL_WIDTH / 2) {
                move_piece(1, 0);
            } else {
                move_piece(-1, 0);
            }

        } else if (state == GAME_STATE_END) {
            //No logic
        }

    }

    STMPE811_Write(STMPE811_FIFO_STA, 0x01);
    STMPE811_Write(STMPE811_FIFO_STA, 0x00);

    // Re-enable IRQs
    WriteDataToTouchModule(STMPE811_INT_EN, currentIRQEnables);
    HAL_EXTI_ClearPending(&LCDTouchIRQ, EXTI_TRIGGER_RISING_FALLING);

    HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    //Potential ERRATA? Clearing IRQ bit again due to an IRQ being triggered DURING the handling of this IRQ..
    WriteDataToTouchModule(STMPE811_INT_STA, clearIRQData);

}

#endif // TOUCH_INTERRUPT_ENABLED
#endif // COMPILE_TOUCH_FUNCTIONS

