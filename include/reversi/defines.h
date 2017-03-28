#pragma once

/* How many tiles are in the board? */
#define BOARD_TILE_HEIGHT 8
#define BOARD_TILE_WIDTH 8
#define BOARD_TILE_COUNT BOARD_TILE_HEIGHT*BOARD_TILE_WIDTH

/* How many pixels is a tile? */
#define TILE_SIZE_X 16
#define TILE_SIZE_Y 16

/* The coordinates of the top left pixel of the board. */
#define BOARD_STARTING_X 95
#define BOARD_STARTING_Y 35

/* Score coordinates. */
#define NAME_WHITE_X 10
#define NAME_WHITE_Y 44
#define SCORE_WHITE_X 10
#define SCORE_WHITE_Y 60

#define NAME_BLACK_X 260
#define NAME_BLACK_Y 44
#define SCORE_BLACK_X 260
#define SCORE_BLACK_Y 60

//DrawCircle macro. RP = RastPort, X Y = coordinates, R = radius
#define DrawCircle(RP, X, Y, R) DrawEllipse(RP, X, Y, R, R)

//Combines SetAPen, Move, and Text functions into one macro.
//RP = RastPort, STRING = char* string, PEN = UBYTE color, X Y = coordinates
#define PrintString(RP, STRING, PEN, X, Y) SetAPen(RP, PEN); Move(RP, X, Y); Text(RP, STRING, strlen(STRING))

typedef struct tile_t {

	int x = -1;
	int y = -1;

	tile_t NW() { return validate(tile_t { x-1, y-1 }); }
	tile_t N() 	{ return validate(tile_t { x  , y-1 }); }
	tile_t NE() { return validate(tile_t { x+1, y-1 }); }
	tile_t W() 	{ return validate(tile_t { x-1, y   }); }
	tile_t E() 	{ return validate(tile_t { x+1, y   }); }
	tile_t SW() { return validate(tile_t { x-1, y+1 }); }
	tile_t S() 	{ return validate(tile_t { x  , y+1 }); }
	tile_t SE() { return validate(tile_t { x+1, y+1 }); }

	static tile_t validate(tile_t tile) 
	{ 
		if(tile.x > BOARD_TILE_WIDTH-1 || tile.x < 0 || tile.y > BOARD_TILE_HEIGHT-1 || tile.y < 0)
			return tile_t { -1, -1 };
		else
			return tile_t { tile.x, tile.y };
	}

	BOOL isPositionValid(){
		if(x > BOARD_TILE_WIDTH-1 || x < 0 || y > BOARD_TILE_HEIGHT-1 || y < 0)
			return FALSE;
		else
			return TRUE;
	}

} Tile;