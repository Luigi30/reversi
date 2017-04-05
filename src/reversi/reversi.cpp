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
					break;
			}
		}
		
	}
}

void REV_PlacePiece(struct RastPort *rp, UBYTE color, int x, int y){

	switch(color){
		case PEN_WHITE:
		board.setSquare(x, y, BOARD_TILE_WHITE);
		break;
		case PEN_BLACK:
		board.setSquare(x, y, BOARD_TILE_BLACK);
		break;
	}

	int xCenter = (BOARD_STARTING_X+(x*TILE_SIZE_X)) + (TILE_SIZE_X / 2);
	int yCenter = (BOARD_STARTING_Y+(y*TILE_SIZE_Y)) + (TILE_SIZE_Y / 2);

	serialPort.SendString("REV_PlacePiece()\r\n");
	SetAPen(rp, color);

	AreaCircle(rp, xCenter, yCenter, 5);
	AreaEnd(rp);

	serialPort.SendString("REV_PlacePiece() complete\r\n");
}

Tile REV_PixelToTile(UWORD x, UWORD y){	
	x = (x - BOARD_STARTING_X) / TILE_SIZE_X;
	y = (y - BOARD_STARTING_Y) / TILE_SIZE_Y;
	
	if(x < 0 || y < 0) return Tile { -1, -1 };
	if(x > 7 || y > 7) return Tile { -1, -1 };
	
	return Tile { x, y };
	
}

void REV_DrawScores(struct Window *window){
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
	
	UBYTE isLegalMove = REV_CheckMoveLegality(tile);

	BoardTile goodColor, badColor;

	if(isLegalMove){
		UBYTE pen;
		switch(gameState) {
			case GAME_TURN_WHITE:
				pen = PEN_WHITE;
				goodColor = BOARD_TILE_WHITE;
				badColor = BOARD_TILE_BLACK;
				break;
			case GAME_TURN_BLACK:
				pen = PEN_BLACK;
				goodColor = BOARD_TILE_BLACK;
				badColor = BOARD_TILE_WHITE;
				break;
			default:
				pen = PEN_BLUE;
				goodColor = BOARD_TILE_EMPTY;
				badColor = BOARD_TILE_EMPTY;
				break;
		}

		REV_FlipPieces(window->RPort, tile, isLegalMove, goodColor, badColor);
		REV_PlacePiece(window->RPort, pen, tile.x, tile.y);
		REV_DrawBoard(window);
		REV_DrawPieces(window);
		REV_DrawScores(window);
		REV_AdvanceGameState(window);
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
			sprintf(buf, STR_BLACKS_TURN);
			break;
		case GAME_OVER:
			sprintf(buf, STR_GAME_OVER);
			break;
	};

	PrintString(window->RPort, buf, PEN_WHITE, 0, 192);
}

UBYTE REV_CheckMoveLegality(Tile move){
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
		char buf[64];
		sprintf(buf, "* Piece already present in (%d,%d). Illegal. \r\n", move.x, move.y);
		serialPort.SendString(buf);
		return false;
	}

	return board.canPlacePiece(goodColor, badColor, move);
}

void REV_FlipPieces(struct RastPort *rp, Tile origin, UBYTE directions, BoardTile goodColor, BoardTile badColor){
	char buf[64];
	sprintf(buf, "REV_FlipPieces(): direction flags are 0x%02X\r\n", directions);
	serialPort.SendString(buf);
	//return 0;

	//Flip pieces in the directions specified in UBYTE directions until we run into a good color piece.
	BoardDirection direction = static_cast<BoardDirection>(0);
	Tile currentTile = origin;

	//find the first direction flag in directions
	while(direction <= 7) {

		if((directions & (1 << direction)) == 0) {
			direction = static_cast<BoardDirection>(direction + 1);
			continue;
		} else {
			sprintf(buf, "REV_FlipPieces(): processing direction %02X\r\n", (1 << direction));
			serialPort.SendString(buf);
		}

		switch(direction){
			case BOARD_DIR_E:
				currentTile = currentTile.E();
				break;
			case BOARD_DIR_SE:
				currentTile = currentTile.SE();
				break;
			case BOARD_DIR_S:
				currentTile = currentTile.S();
				break;
			case BOARD_DIR_SW:
				currentTile = currentTile.SW();
				break;
			case BOARD_DIR_W:
				currentTile = currentTile.W();
				break;
			case BOARD_DIR_NW:
				currentTile = currentTile.NW();
				break;
			case BOARD_DIR_N:
				currentTile = currentTile.N();
				break;
			case BOARD_DIR_NE:
				currentTile = currentTile.NE();
				break;
		}

		BoardTile currentPiece = board.getSquare(currentTile.x, currentTile.y);
		board.setSquare(currentTile.x, currentTile.y, goodColor);
		sprintf(buf, "Flipped tile (%d,%d)\r\n", currentTile.x, currentTile.y);
		serialPort.SendString(buf);
		if(currentPiece == goodColor){
			direction = static_cast<BoardDirection>(direction + 1);
			currentTile = origin;
		}
	}
}

void REV_AdvanceGameState(struct Window *window){
	if(gameState == GAME_TURN_WHITE) {
		gameState = GAME_TURN_BLACK;
		SetAPen(window->RPort, PEN_BLACK);
		Move(window->RPort, NAME_BLACK_X-5, NAME_BLACK_Y-8);
		Draw(window->RPort, NAME_BLACK_X+42, NAME_BLACK_Y-8);

		SetAPen(window->RPort, PEN_BGGRAY);
		Move(window->RPort, NAME_WHITE_X-5, NAME_WHITE_Y-8);
		Draw(window->RPort, NAME_WHITE_X+42, NAME_WHITE_Y-8);
	}
	else if(gameState == GAME_TURN_BLACK) {
		gameState = GAME_TURN_WHITE;
		SetAPen(window->RPort, PEN_BLACK);
		Move(window->RPort, NAME_WHITE_X-5, NAME_WHITE_Y-8);
		Draw(window->RPort, NAME_WHITE_X+42, NAME_WHITE_Y-8);

		SetAPen(window->RPort, PEN_BGGRAY);
		Move(window->RPort, NAME_BLACK_X-5, NAME_BLACK_Y-8);
		Draw(window->RPort, NAME_BLACK_X+42, NAME_BLACK_Y-8);
	}
}