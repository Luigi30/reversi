#include "reversi/board.h"

void ReversiBoard::initForNewGame() {
	for(int i=0;i<BOARD_TILE_COUNT;i++){
		square[i] = BOARD_TILE_EMPTY;
		
		setSquare(3, 3, BOARD_TILE_WHITE);
		setSquare(4, 3, BOARD_TILE_BLACK);
		setSquare(3, 4, BOARD_TILE_BLACK);
		setSquare(4, 4, BOARD_TILE_WHITE);
	}
}

int ReversiBoard::getScore(BoardTile color){
	int score = 0;
	
	for(int i=0;i<BOARD_TILE_COUNT;i++){
		if(square[i] == color)
			score++;
	}
	
	return score;
}

UBYTE ReversiBoard::canPlacePiece(BoardTile goodColor, BoardTile badColor, Tile position){
	char buf[64];
	sprintf(buf, "board.canPlacePiece() position is (%d,%d)\r\n", position.x, position.y);
	serialPort.SendString(buf);

	return checkDirection(goodColor, badColor, position);
}

/*******************************************
* There's a better way to do this...
*/
UBYTE ReversiBoard::checkDirection(BoardTile goodColor, BoardTile badColor, Tile position){
	UBYTE validDirections = 0x00;

	char buf[64];

	BoardDirection direction = BOARD_DIR_E;

	Tile currentTile { position.x, position.y };
	BoardTile currentPiece = getSquare(position.x, position.y);
	BOOL foundGoodTile = FALSE;
	BOOL foundBadTile = FALSE;
	BOOL foundEmptyTile = FALSE;

	BOOL advanceToNextDirection = FALSE;

	sprintf(buf, "currentTile is (%d, %d). direction is %d\r\n", currentTile.x, currentTile.y, direction);
	//serialPort.SendString(buf);

	//Loop until we run off the board, find a good color tile, or find an empty tile.
	while(direction <= 7) {

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

		currentPiece = getSquare(currentTile.x, currentTile.y);

		sprintf(buf, "Now looking at currentTile (%d,%d). Color is %d. Good %d Bad %d\r\n", currentTile.x, currentTile.y, currentPiece, goodColor, badColor);
		serialPort.SendString(buf);

		if(currentPiece == badColor) foundBadTile = TRUE;					//Note this and keep traversing.
		else if(currentPiece == goodColor) foundGoodTile = TRUE; 			//This will be the last traversal.
		else if(currentPiece == BOARD_TILE_EMPTY) foundEmptyTile = TRUE; 	//This will be the last traversal.

		//This is broken again.
		if(foundBadTile == TRUE && foundGoodTile == TRUE){
			validDirections = validDirections | (1 << direction); //set bitflag for this direction
			sprintf(buf, "added flag 0x%02X - validDirections is now 0x%02X\r\n", (1 << direction), validDirections);
			serialPort.SendString(buf);
			advanceToNextDirection = TRUE;
		}
		else if((!currentTile.isPositionValid() || foundEmptyTile == TRUE) || (foundBadTile == FALSE && foundGoodTile == TRUE)) {
			advanceToNextDirection = TRUE;
		}

		if(advanceToNextDirection){
			currentTile = { position.x, position.y };
			currentPiece = getSquare(position.x, position.y);
			direction = static_cast<BoardDirection>(direction + 1);

			foundBadTile = FALSE;
			foundGoodTile = FALSE;
			foundEmptyTile = FALSE;

			sprintf(buf, "Direction is now %d", direction);
			//serialPort.SendString(buf);

			advanceToNextDirection = FALSE;
		}
	}

	return validDirections;
}