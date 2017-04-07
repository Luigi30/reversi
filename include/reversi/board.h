#pragma once

#include <stdio.h>

#include <exec/types.h>

#include "reversi/defines.h"
#include "serial.h"

#define VALID_MOVE_NONE 0b00000000
#define VALID_MOVE_E 	0b00000001
#define VALID_MOVE_SE	0b00000010
#define VALID_MOVE_S	0b00000100
#define VALID_MOVE_SW	0b00001000
#define VALID_MOVE_W	0b00010000
#define VALID_MOVE_NW	0b00100000
#define VALID_MOVE_N	0b01000000
#define VALID_MOVE_NE	0b10000000

typedef enum board_direction_t {
	BOARD_DIR_E 	= 0,
	BOARD_DIR_SE 	= 1,
	BOARD_DIR_S		= 2,
	BOARD_DIR_SW 	= 3,
	BOARD_DIR_W 	= 4,
	BOARD_DIR_NW 	= 5,
	BOARD_DIR_N 	= 6,
	BOARD_DIR_NE 	= 7
} BoardDirection;

typedef enum board_tile_t {
	BOARD_TILE_EMPTY,
	BOARD_TILE_WHITE,
	BOARD_TILE_BLACK,
	BOARD_TILE_INVALID
} BoardTile;

class ReversiBoard {
	BoardTile square[BOARD_TILE_COUNT];
	
	public:
		ReversiBoard() {
			for (int i = 0; i < BOARD_TILE_COUNT; i++) {
				square[i] = BOARD_TILE_EMPTY;
			}
		}

		ReversiBoard(const ReversiBoard &obj) {
			for (int i = 0; i < BOARD_TILE_COUNT; i++) {
				square[i] = obj.square[i];
			}
		}

		ReversiBoard& operator=(const ReversiBoard& obj) {
			for (int i = 0; i < BOARD_TILE_COUNT; i++) {
				square[i] = obj.square[i];
			}
			return *this;
		}

		BoardTile getSquare(int x, int y) {
			char buf[64];
			sprintf(buf, "getSquare(%d, %d)", x, y);
			//serialPort.SendString(buf);
			if(x >= BOARD_TILE_WIDTH || x < 0 || y >= BOARD_TILE_HEIGHT || y < 0){
				return BOARD_TILE_INVALID;
			} else {
				return square[y*BOARD_TILE_WIDTH + x]; 
			}
		};
		void setSquare(int x, int y, BoardTile piece) { square[y*BOARD_TILE_WIDTH + x] = piece; };
		
		void initForNewGame();
		int getScore(BoardTile color);

		UBYTE canPlacePiece(BoardTile goodColor, BoardTile badColor, Tile position);
		UBYTE checkDirection(BoardTile goodColor, BoardTile badColor, Tile position);
		void print();
};

extern ReversiBoard board;