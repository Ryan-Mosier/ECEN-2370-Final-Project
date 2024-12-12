#ifndef INC_RENDERER_H_
#define INC_RENDERER_H_

#include <stdint.h>
#include <stdbool.h>
#include "RNG.h"
#include "timers.h"
#include "LCD_Driver.h"
#include "ApplicationCode.h"

extern uint16_t singles, doubles, triples, quads;
typedef struct {
    int8_t x;
    int8_t y;
    uint16_t color;
} block;

typedef enum {
    I_PIECE = 0,
    O_PIECE = 1,
    T_PIECE = 2,
    S_PIECE = 3,
    Z_PIECE = 4,
    J_PIECE = 5,
    L_PIECE = 6,
} PieceType;

typedef enum {
    ROTATION_0,
    ROTATION_90,
    ROTATION_180,
    ROTATION_270
} Rotation;

typedef struct {
    uint8_t x, y;
    PieceType type;
    Rotation rotation;
    block blocks[4];
} piece;

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
typedef struct {
    block static_blocks[BOARD_WIDTH][BOARD_HEIGHT]; // 20 rows by 10 columns game board
    piece dynamic_piece; // currently controlled piece
} GameBoard;

#define MAX_PIECE_QUEUE_SIZE 5
typedef struct {
    PieceType buffer[MAX_PIECE_QUEUE_SIZE];
    int front;
    int rear;
    int size;
} PieceQueue;

// Function prototypes for PieceQueue
void PieceQueue_Init();

bool PieceQueue_IsEmpty();

bool PieceQueue_IsFull();

bool PieceQueue_Enqueue(const PieceType *p);

bool PieceQueue_Dequeue(PieceType *p);

PieceType PieceQueue_GetHead();

void PieceQueue_GetNext3(PieceType p[3]);

void PieceQueue_Fill();


void game_init();

// Create a new dynamic piece.
void new_dynamic_piece();

// Move the dynamic piece by (dx, dy).
bool can_move_piece(int dx, int dy);

void move_piece(int dx, int dy);

// Rotate the dynamic piece.
bool can_rotate_piece(Rotation rotation);

void rotate_piece(Rotation rotation);

// Merge the dynamic piece into static blocks.
// Must also chec if row is filled
void merge_dynamic_to_static();

// Handle what happens when a row is filled.
void handle_filled_rows(int cleared_rows[4]);

// Render the whole game board.
// only called when a row is cleared
void render_game();

void render_piece(uint16_t color);


#endif
