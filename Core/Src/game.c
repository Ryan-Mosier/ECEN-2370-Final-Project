#include "game.h"

GameBoard gameBoard = {0};
uint16_t singles = 0, doubles = 0, triples = 0, quads = 0;

static block I_all[4][4] = {{{0,  -1, LCD_COLOR_CYAN}, {0, 0, LCD_COLOR_CYAN}, {0, 1, LCD_COLOR_CYAN}, {0, 2, LCD_COLOR_CYAN}},
        // ROTATION_0
                            {{-1, 1,  LCD_COLOR_CYAN}, {0, 1, LCD_COLOR_CYAN}, {1, 1, LCD_COLOR_CYAN}, {2, 1, LCD_COLOR_CYAN}},
        // ROTATION_90
                            {{1,  -1, LCD_COLOR_CYAN}, {1, 0, LCD_COLOR_CYAN}, {1, 1, LCD_COLOR_CYAN}, {1, 2, LCD_COLOR_CYAN}},
        // ROTATION_180
                            {{-1, 0,  LCD_COLOR_CYAN}, {0, 0, LCD_COLOR_CYAN}, {1, 0, LCD_COLOR_CYAN}, {2, 0, LCD_COLOR_CYAN}},
        // ROTATION_270

};

static block O_all[4] = {
        {0, 0, LCD_COLOR_YELLOW},
        {0, 1, LCD_COLOR_YELLOW},
        {1, 0, LCD_COLOR_YELLOW},
        {1, 1, LCD_COLOR_YELLOW},
};

static block T_0[4] = {
        {0,  0, LCD_COLOR_MAGENTA},
        {0,  1, LCD_COLOR_MAGENTA},
        {1,  0, LCD_COLOR_MAGENTA},
        {-1, 0, LCD_COLOR_MAGENTA}
};

static block L_0[4] = {
        {0,  0, LCD_COLOR_ORANGE},
        {-1, 0, LCD_COLOR_ORANGE},
        {1,  0, LCD_COLOR_ORANGE},
        {1,  1, LCD_COLOR_ORANGE}
};

static block J_0[4] = {
        {0,  0, LCD_COLOR_BLUE},
        {-1, 0, LCD_COLOR_BLUE},
        {-1, 1, LCD_COLOR_BLUE},
        {1,  0, LCD_COLOR_BLUE}
};

static block S_0[4] = {
        {0,  0, LCD_COLOR_GREEN},
        {-1, 0, LCD_COLOR_GREEN},
        {0,  1, LCD_COLOR_GREEN},
        {1,  1, LCD_COLOR_GREEN}
};

static block Z_0[4] = {
        {0,  0, LCD_COLOR_RED},
        {1,  0, LCD_COLOR_RED},
        {0,  1, LCD_COLOR_RED},
        {-1, 1, LCD_COLOR_RED}
};
//typedef enum {
//    I_PIECE = 0, //custom
//    O_PIECE = 1, //custom
//    T_PIECE = 2, //use math
//    S_PIECE = 3, //use math
//    Z_PIECE = 4, //use math
//    J_PIECE = 5, //use math
//    L_PIECE = 6, //use math
//} PieceType;

static PieceQueue pieceQueue;

void PieceQueue_Init() {
    pieceQueue.front = 0;
    pieceQueue.rear = 0;
    pieceQueue.size = 0;
    PieceQueue_Fill();
}

bool PieceQueue_IsEmpty() {
    return (pieceQueue.size == 0);
}

bool PieceQueue_IsFull() {
    return (pieceQueue.size == MAX_PIECE_QUEUE_SIZE);
}

bool PieceQueue_Enqueue(const PieceType *p) {
    if (PieceQueue_IsFull()) {
        return false; // Queue is full
    }
    pieceQueue.buffer[pieceQueue.rear] = *p;
    pieceQueue.rear = (pieceQueue.rear + 1) % MAX_PIECE_QUEUE_SIZE;
    pieceQueue.size++;
    return true;
}

// Function to dequeue a piece from the queue
bool PieceQueue_Dequeue(PieceType *p) {
    if (PieceQueue_IsEmpty()) {
        return false; // Queue is empty
    }
    *p = pieceQueue.buffer[pieceQueue.front];
    pieceQueue.front = (pieceQueue.front + 1) % MAX_PIECE_QUEUE_SIZE;
    pieceQueue.size--;
    return true;
}

PieceType PieceQueue_GetHead() {
    return pieceQueue.buffer[pieceQueue.front];
}

void PieceQueue_GetNext3(PieceType p[3]) {
    for (int i = 0; i < 3; i++) {
        p[i] = pieceQueue.buffer[(pieceQueue.front + i) % MAX_PIECE_QUEUE_SIZE];
    }
}

void PieceQueue_Fill() {
    while ((pieceQueue.rear + 1) % MAX_PIECE_QUEUE_SIZE != pieceQueue.front) {
        uint32_t number = RNG_GetRandomNumber();
        number %= 7;
        PieceQueue_Enqueue((PieceType *) (&number));
    }
}

void game_init() {
    RNG_Init();
    Timer_Elapsed_Init();
    Timer_Interrupt_Init();
    PieceQueue_Init();
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            gameBoard.static_blocks[j][i].color = LCD_COLOR_BLACK;
        }
    }
    new_dynamic_piece();
}

void copy_blocks_data(block *data) {
    for (int i = 0; i < 4; i++) {
        gameBoard.dynamic_piece.blocks[i] = data[i];
    }
}

void new_dynamic_piece() {
    PieceQueue_Fill();
    gameBoard.dynamic_piece.rotation = ROTATION_0;
    PieceQueue_Dequeue(&gameBoard.dynamic_piece.type);
    switch (gameBoard.dynamic_piece.type) {
        case 0:
            copy_blocks_data(I_all[0]);
            gameBoard.dynamic_piece.x = 4;
            gameBoard.dynamic_piece.y = 17;
            break;
        case O_PIECE:
            copy_blocks_data(O_all);
            gameBoard.dynamic_piece.x = 4;
            gameBoard.dynamic_piece.y = 18;
            break;
        case T_PIECE:
            copy_blocks_data(T_0);
            gameBoard.dynamic_piece.x = 4;
            gameBoard.dynamic_piece.y = 18;
            break;
        case S_PIECE:
            copy_blocks_data(S_0);
            gameBoard.dynamic_piece.x = 4;
            gameBoard.dynamic_piece.y = 18;
            break;
        case Z_PIECE:
            copy_blocks_data(Z_0);
            gameBoard.dynamic_piece.x = 4;
            gameBoard.dynamic_piece.y = 18;
            break;
        case J_PIECE:
            copy_blocks_data(J_0);
            gameBoard.dynamic_piece.x = 4;
            gameBoard.dynamic_piece.y = 18;
            break;
        case L_PIECE:
            copy_blocks_data(L_0);
            gameBoard.dynamic_piece.x = 4;
            gameBoard.dynamic_piece.y = 18;
            break;
    }
    if(!can_move_piece(0,0)){
        state = GAME_STATE_END;
    }
}

// Move the dynamic piece by (dx, dy).
bool can_move_piece(int dx, int dy) {
    for (int i = 0; i < 4; i++) {
        int x, y;
        x = gameBoard.dynamic_piece.x + gameBoard.dynamic_piece.blocks[i].x + dx;
        y = gameBoard.dynamic_piece.y + gameBoard.dynamic_piece.blocks[i].y + dy;
        //check out of bounds
        if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
        	if((y == -1) &&(dy == -1)){
                merge_dynamic_to_static();
        	}
            return false;
        }
        //check static
        if (gameBoard.static_blocks[x][y].color != LCD_COLOR_BLACK) {
            if (dy == -1) {
                merge_dynamic_to_static();
            }
            return false;
        }
    }
    return true;
}

void move_piece(int dx, int dy) {
    if (!can_move_piece(dx, dy)) {
        return;
    }
    render_piece(LCD_COLOR_BLACK);
    gameBoard.dynamic_piece.x += dx;
    gameBoard.dynamic_piece.y += dy;
    render_piece(LCD_COLOR_CHECK);
}

void math_rotate(piece *piece, Rotation rotation) {
    while (rotation != 0) {
        for (int i = 0; i < 4; i++) {
            int newx, newy;
            newx = piece->blocks[i].y;
            newy = (-1) * piece->blocks[i].x;
            piece->blocks[i].x = newx;
            piece->blocks[i].y = newy;
        }
        rotation--;
    }
}
// Rotate the dynamic piece.
bool can_rotate_piece(Rotation rotation) {
    //copy data to a temp struct
    piece temp = {0};
    temp.x = gameBoard.dynamic_piece.x;
    temp.y = gameBoard.dynamic_piece.y;
    temp.type = gameBoard.dynamic_piece.type;
    temp.rotation = gameBoard.dynamic_piece.rotation;
    for (int i = 0; i < 4; i++) {
        temp.blocks[i] = gameBoard.dynamic_piece.blocks[i];
    }
    //rotate the temp piece
    switch (temp.type) {
        case I_PIECE:
            copy_blocks_data(I_all[temp.rotation]);
            break;
        case O_PIECE:
            break;
        case T_PIECE:
        case S_PIECE:
        case Z_PIECE:
        case J_PIECE:
        case L_PIECE:
            math_rotate(&temp, rotation);
            break;
    }

    for (int i = 0; i < 4; i++) {
        int x, y;
        x = gameBoard.dynamic_piece.x + gameBoard.dynamic_piece.blocks[i].x;
        y = gameBoard.dynamic_piece.y + gameBoard.dynamic_piece.blocks[i].y;
        if (gameBoard.static_blocks[x][y].color != LCD_COLOR_BLACK) {
            return false;
        }
    }
    return true;

}

void rotate_piece(Rotation rotation) {
    if (!can_rotate_piece(rotation)) {
        return;
    }
    render_piece(LCD_COLOR_BLACK);
    gameBoard.dynamic_piece.rotation = (gameBoard.dynamic_piece.rotation + rotation) % 4;
    switch (gameBoard.dynamic_piece.type) {
        case I_PIECE:
            copy_blocks_data(I_all[gameBoard.dynamic_piece.rotation]);
            break;
        case O_PIECE:
            break;
        case T_PIECE:
        case S_PIECE:
        case Z_PIECE:
        case J_PIECE:
        case L_PIECE:
            math_rotate(&gameBoard.dynamic_piece, rotation);
            break;
    }
    render_piece(LCD_COLOR_CHECK);
}

bool check_row(int row) {
    for (int i = 0; i < BOARD_WIDTH; i++) {
        if (gameBoard.static_blocks[i][row].color == LCD_COLOR_BLACK) {
            return false;
        }
    }
    return true;
}

// Merge the dynamic piece into static blocks.
// Must also check if row is filled
void merge_dynamic_to_static() {
    Timer_Interrupt_Stop();
    int cleared_rows[4] = {-1, -1, -1, -1};
    int j = 0;
    for (int i = 0; i < 4; i++) {
        int x, y;
        x = gameBoard.dynamic_piece.x + gameBoard.dynamic_piece.blocks[i].x;
        y = gameBoard.dynamic_piece.y + gameBoard.dynamic_piece.blocks[i].y;
        gameBoard.static_blocks[x][y].color = gameBoard.dynamic_piece.blocks[i].color;
        gameBoard.dynamic_piece.blocks[i].color = LCD_COLOR_BLACK;
        if (check_row(y)) {
            cleared_rows[j++] = y;
        }
    }
    handle_filled_rows(cleared_rows);
    Timer_Interrupt_Reset();
    new_dynamic_piece();
    Timer_Interrupt_Start();
}

void clear_row(int row) {
#define blocks gameBoard.static_blocks
    for (int i = row; i < BOARD_HEIGHT - 1; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            //bring the rows above row down by 1
            blocks[j][i] = blocks[j][i + 1];
        }
    }

#undef blocks
}

// Handle what happens when a row is filled.
void handle_filled_rows(int cleared_rows[4]) {
    for (int i = 0; i < 4; i++) {
        if (cleared_rows[i] == -1) continue;
        //check for adjacent rows
        int adjacent_rows_list[4 - i];
        int adjacent_rows = 1;
        adjacent_rows_list[0] = cleared_rows[i];
        for (int j = i; j < 4; j++) {
            if (cleared_rows[j] == -1) continue;
            int diff = cleared_rows[j] - cleared_rows[i];
            if (diff == -1 || diff == 1) {
                adjacent_rows_list[adjacent_rows++] = cleared_rows[j];
                cleared_rows[j] = -1;
            }
        }
        if (adjacent_rows == 1) {
            singles++;
        } else if (adjacent_rows == 2) {
            doubles++;
        } else if (adjacent_rows == 3) {
            triples++;
        } else if (adjacent_rows == 4) {
            quads++;
        }
        for (int j = 0; j < adjacent_rows; j++) {
            clear_row(adjacent_rows_list[j]);
        }
    }
}

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

// Render the whole game board.
// only called when a row is cleared
void render_game() {
    LCD_Clear(0, LCD_COLOR_BLACK);
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            LCD_Draw_Square_Fill(scale_4_LCD_W(i) + (SQUARE_SIZE / 2), scale_4_LCD_H(j) + (SQUARE_SIZE / 2),
                                 SQUARE_SIZE, SQUARE_SIZE, gameBoard.static_blocks[i][j].color);
        }
    }
    render_piece(LCD_COLOR_CHECK);
}

// Render the dynamic piece
// Ran when moved or rotated
void render_piece(uint16_t color) {
    for (int i = 0; i < 4; i++) {
        int x, y;
        x = gameBoard.dynamic_piece.x + gameBoard.dynamic_piece.blocks[i].x;
        y = gameBoard.dynamic_piece.y + gameBoard.dynamic_piece.blocks[i].y;
        if (color == LCD_COLOR_CHECK) {
            if (gameBoard.dynamic_piece.blocks[i].color == LCD_COLOR_BLACK) continue;
            LCD_Draw_Square_Fill(scale_4_LCD_W(x) + (SQUARE_SIZE / 2), scale_4_LCD_H(y) + (SQUARE_SIZE / 2),
                                 SQUARE_SIZE, SQUARE_SIZE, gameBoard.dynamic_piece.blocks[i].color);
        } else {
            LCD_Draw_Square_Fill(scale_4_LCD_W(x) + (SQUARE_SIZE / 2), scale_4_LCD_H(y) + (SQUARE_SIZE / 2),
                                 SQUARE_SIZE, SQUARE_SIZE, color);
        }
    }
}
