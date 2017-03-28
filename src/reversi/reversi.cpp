#include "reversi/reversi.h"

ReversiBoard board = ReversiBoard();
GameState gameState = GAME_TURN_WHITE;

short bm_w, bm_h;
UBYTE *floodBuffer;
struct TmpRas tr;

void REV_PlaceBackground(struct RastPort *rp, UBYTE pen, int x, int y){
	int xPixel = BOARD_STARTING_X+(x*TILE_SIZE_X);
	int yPixel = BOARD_STARTING_Y+(y*TILE_SIZE_Y);
	
	SetAPen(rp, pen);
	RectFill(rp, xPixel, yPixel, xPixel+TILE_SIZE_X, yPixel+TILE_SIZE_Y);
	
	//Draw a black box around the inside of the tile.
	SetAPen(rp, PEN_BLACK);
	Move(rp, xPixel, yPixel);							//top left...
	Draw(rp, xPixel+TILE_SIZE_X, yPixel);				//top right...
	Draw(rp, xPixel+TILE_SIZE_X, yPixel+TILE_SIZE_Y);	//bottom right...
	Draw(rp, xPixel, yPixel+TILE_SIZE_Y);				//bottom left...
	Draw(rp, xPixel, yPixel);							//top left.
}

void REV_DrawBoard(struct Window *window){
	auto rp = window->RPort;	
	
	for(int y=0;y<8;y++){
		REV_PlaceBackground(rp, PEN_BLUE, 0, y);
		REV_PlaceBackground(rp, PEN_BLUE, 1, y);
		REV_PlaceBackground(rp, PEN_BLUE, 2, y);
		REV_PlaceBackground(rp, PEN_BLUE, 3, y);
		REV_PlaceBackground(rp, PEN_BLUE, 4, y);
		REV_PlaceBackground(rp, PEN_BLUE, 5, y);
		REV_PlaceBackground(rp, PEN_BLUE, 6, y);
		REV_PlaceBackground(rp, PEN_BLUE, 7, y);
	}

}

void REV_DrawPieces(struct Window *window){
	for(int y = 0; y < BOARD_TILE_HEIGHT; y++){
		for(int x = 0; x < BOARD_TILE_WIDTH; x++){
			switch(board.getSquare(x, y)){
				case BOARD_TILE_BLACK:
					REV_PlacePiece(window->RPort, PEN_BLACK, x, y);
					break;
				case BOARD_TILE_WHITE:
					REV_PlacePiece(window->RPort, PEN_WHITE, x, y);
					break;
				default:
					REV_PlaceBackground(window->RPort, PEN_BLUE, x, y);
					break;
			}
		}
		
	}
	
}

void REV_PlacePiece(struct RastPort *rp, UBYTE color, int x, int y){

	serialPort.SendString("REV_PlacePiece()\r\n");

	int xCenter = (BOARD_STARTING_X+(x*TILE_SIZE_X)) + (TILE_SIZE_X / 2);
	int yCenter = (BOARD_STARTING_Y+(y*TILE_SIZE_Y)) + (TILE_SIZE_Y / 2);

	SetAPen(rp, color);
	RectFill(rp, xCenter-5, yCenter-5, xCenter+5, yCenter+5); 

	serialPort.SendString("REV_PlacePiece() complete\r\n");
}

Tile REV_PixelToTile(UWORD x, UWORD y){	
	x = (x - BOARD_STARTING_X) / TILE_SIZE_X;
	y = (y - BOARD_STARTING_Y) / TILE_SIZE_Y;
	
	if(x < 0 || y < 0) return Tile { -1, -1 };
	if(x > 7 || y > 7) return Tile { -1, -1 };
	
	return Tile { x, y };
	
}

void REV_DrawScores(struct Window *window, ReversiBoard board){
	auto rp = window->RPort;
	
	int whiteScore = board.getScore(BOARD_TILE_WHITE);
	int blackScore = board.getScore(BOARD_TILE_BLACK);
	char strWhiteScore[8];
	char strBlackScore[8];
	sprintf(strWhiteScore, "%5d", whiteScore);
	sprintf(strBlackScore, "%d", blackScore);
	
	char whiteName[] = "White";
	char blackName[] = "Black";
	
	SetAPen(rp, PEN_WHITE);
	
	Move(rp, NAME_WHITE_X, NAME_WHITE_Y);
	Text(rp, whiteName, strlen(whiteName));
	Move(rp, SCORE_WHITE_X, SCORE_WHITE_Y);
	Text(rp, strWhiteScore, strlen(strWhiteScore));
	
	Move(rp, NAME_BLACK_X, NAME_BLACK_Y);
	Text(rp, blackName, strlen(blackName));
	Move(rp, SCORE_BLACK_X, SCORE_BLACK_Y);
	Text(rp, strBlackScore, strlen(strBlackScore));
}

void REV_HandleTileClick(struct Window *window, UWORD mouseX, UWORD mouseY){
	Tile tile = REV_PixelToTile(mouseX, mouseY);
	
	char clickStr[32];
	sprintf(clickStr, "Tile (%2d,%2d)", tile.x, tile.y);
	PrintString(window->RPort, clickStr, PEN_WHITE, 0, 32);
	
	BOOL isLegalMove = REV_CheckMoveLegality(board, tile);

	if(isLegalMove){
		REV_PrintGameStatus(window);
	} else {
		PrintString(window->RPort, STR_ILLEGAL_MOVE, PEN_WHITE, 0, 192);
	}
}

void REV_PrintGameStatus(struct Window *window){
	char buf[32];

	switch(gameState){
		case GAME_TURN_WHITE:
			sprintf(buf, STR_WHITES_TURN);
			break;
		case GAME_TURN_BLACK:
			sprintf(buf, STR_WHITES_TURN);
			break;
		case GAME_OVER:
			sprintf(buf, STR_GAME_OVER);
			break;
	};

	PrintString(window->RPort, buf, PEN_WHITE, 0, 192);
}

BOOL REV_CheckMoveLegality(ReversiBoard board, Tile move){
	//	* 	Look for a neighboring piece of opposite color
	//	* 	Traverse the board in that direction
	//	*	If there's a piece of our color before we reach the end of the board, the move is valid

	//Look for a neighboring piece of the opposite color
	serialPort.SendString("REV_CheckMoveLegality\r\n");

	BoardTile goodColor, badColor;
	switch(gameState){
		case GAME_TURN_WHITE:
			goodColor = BOARD_TILE_WHITE;
			badColor  = BOARD_TILE_BLACK;
			break;
		case GAME_TURN_BLACK:
			goodColor = BOARD_TILE_BLACK;
			badColor  = BOARD_TILE_WHITE;
			break;
		case GAME_OVER:
			return false;
			break;
		default:
			return false;
			break;
	}

	if(board.getSquare(move.x, move.y) != BOARD_TILE_EMPTY){
		return false;
	}

	return board.canPlacePiece(goodColor, badColor, move);
}