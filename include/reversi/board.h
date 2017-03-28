#pragma once

#include <stdio.h>

#include <exec/types.h>

#include "reversi/defines.h"
#include "serial.h"

typedef enum board_tile_t {
	BOARD_TILE_EMPTY,
	BOARD_TILE_WHITE,
	BOARD_TILE_BLACK,
	BOARD_TILE_INVALID
} BoardTile;

class ReversiBoard {
	BoardTile square[BOARD_TILE_COUNT];
	
	public:
		BoardTile getSquare(int x, int y) {
			char buf[64];
			sprintf(buf, "getSquare(%d, %d)", x, y);
			serialPort.SendString(buf);
			if(x >= BOARD_TILE_WIDTH || x < 0 || y >= BOARD_TILE_HEIGHT || y < 0){
				return BOARD_TILE_INVALID;
			} else {
				return square[y*BOARD_TILE_WIDTH + x]; 
			}
		};
		void setSquare(int x, int y, BoardTile piece) { square[y*BOARD_TILE_WIDTH + x] = piece; };
		
		void initForNewGame();
		int getScore(BoardTile color);

		BOOL canPlacePiece(BoardTile goodColor, BoardTile badColor, Tile position);

		BOOL checkE(BoardTile goodColor, BoardTile badColor, Tile position);
		BOOL checkW(BoardTile goodColor, BoardTile badColor, Tile position);
		BOOL checkN(BoardTile goodColor, BoardTile badColor, Tile position);
		BOOL checkS(BoardTile goodColor, BoardTile badColor, Tile position);

		//Weird: if you comment this and the functions out in board.cpp, it works...
		BOOL checkSE(BoardTile goodColor, BoardTile badColor, Tile position);
		BOOL checkSW(BoardTile goodColor, BoardTile badColor, Tile position);
		BOOL checkNE(BoardTile goodColor, BoardTile badColor, Tile position);
		BOOL checkNW(BoardTile goodColor, BoardTile badColor, Tile position);
};

extern ReversiBoard board;