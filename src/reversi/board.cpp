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

BOOL ReversiBoard::canPlacePiece(BoardTile goodColor, BoardTile badColor, Tile position){
	char buf[64];
	sprintf(buf, "board.canPlacePiece() position is (%d,%d)\r\n", position.x, position.y);

	serialPort.SendString(buf);

	if(checkE(goodColor, badColor, position) || checkW(goodColor, badColor, position) || checkS(goodColor, badColor, position) || checkN(goodColor, badColor, position)){
		return TRUE;
	}

	if(checkSE(goodColor, badColor, position) || checkSW(goodColor, badColor, position) || checkNW(goodColor, badColor, position) || checkNE(goodColor, badColor, position))
		return TRUE;

	return FALSE;
}

/*******************************************
* There's a better way to do this...
*/
BOOL ReversiBoard::checkE(BoardTile goodColor, BoardTile badColor, Tile position){
	serialPort.SendString("* checkE\r\n");
	char buf[64];

	Tile currentTile { position.x, position.y };
	BoardTile currentPiece = getSquare(position.x, position.y);
	BOOL foundGoodTile = FALSE;
	BOOL foundBadTile = FALSE;
	BOOL foundEmptyTile = FALSE;

	sprintf(buf, "currentTile is (%d, %d)\r\n", currentTile.x, currentTile.y);
	serialPort.SendString(buf);

	//Loop until we run off the board, find a good color tile, or find an empty tile.
	while(currentTile.isPositionValid() && (foundGoodTile == FALSE && foundEmptyTile == FALSE)) {
		currentTile = currentTile.E();
		currentPiece = getSquare(currentTile.x, currentTile.y);

		sprintf(buf, "Now looking at currentTile (%d,%d)\r\n", currentTile.x, currentTile.y);
		serialPort.SendString(buf);

		if(currentPiece == badColor) foundBadTile = TRUE;					//Note this and keep traversing.
		else if(currentPiece == goodColor) foundGoodTile = TRUE; 			//This will be the last traversal.
		else if(currentPiece == BOARD_TILE_EMPTY) foundEmptyTile = TRUE; 	//This will be the last traversal.
	}
	if(foundBadTile == TRUE && foundGoodTile == TRUE){
		serialPort.SendString("* checkE: TRUE!\r\n");
		return TRUE; //If we found a bad tile and a good tile, we can place here.
	} 
	else return FALSE;
}

BOOL ReversiBoard::checkW(BoardTile goodColor, BoardTile badColor, Tile position){
	serialPort.SendString("* checkW\r\n");
	char buf[64];

	Tile currentTile { position.x, position.y };
	BoardTile currentPiece = getSquare(position.x, position.y);
	BOOL foundGoodTile = FALSE;
	BOOL foundBadTile = FALSE;
	BOOL foundEmptyTile = FALSE;

	sprintf(buf, "currentTile is (%d, %d)\r\n", currentTile.x, currentTile.y);
	serialPort.SendString(buf);

	//Loop until we run off the board, find a good color tile, or find an empty tile.
	while(currentTile.isPositionValid() && (foundGoodTile == FALSE && foundEmptyTile == FALSE)) {
		currentTile = currentTile.W();
		currentPiece = getSquare(currentTile.x, currentTile.y);

		sprintf(buf, "Now looking at currentTile (%d,%d)\r\n", currentTile.x, currentTile.y);
		serialPort.SendString(buf);

		if(currentPiece == badColor) foundBadTile = TRUE;					//Note this and keep traversing.
		else if(currentPiece == goodColor) foundGoodTile = TRUE; 			//This will be the last traversal.
		else if(currentPiece == BOARD_TILE_EMPTY) foundEmptyTile = TRUE; 	//This will be the last traversal.
	}
	if(foundBadTile == TRUE && foundGoodTile == TRUE){
		serialPort.SendString("* checkW: TRUE!\r\n");
		return TRUE; //If we found a bad tile and a good tile, we can place here.
	} 
	else return FALSE;
}

BOOL ReversiBoard::checkN(BoardTile goodColor, BoardTile badColor, Tile position){
	serialPort.SendString("* checkN\r\n");
	char buf[64];

	Tile currentTile { position.x, position.y };
	BoardTile currentPiece = getSquare(position.x, position.y);
	BOOL foundGoodTile = FALSE;
	BOOL foundBadTile = FALSE;
	BOOL foundEmptyTile = FALSE;

	sprintf(buf, "currentTile is (%d, %d)\r\n", currentTile.x, currentTile.y);
	serialPort.SendString(buf);

	//Loop until we run off the board, find a good color tile, or find an empty tile.
	while(currentTile.isPositionValid() && (foundGoodTile == FALSE && foundEmptyTile == FALSE)) {
		currentTile = currentTile.N();
		currentPiece = getSquare(currentTile.x, currentTile.y);

		sprintf(buf, "Now looking at currentTile (%d,%d)\r\n", currentTile.x, currentTile.y);
		serialPort.SendString(buf);

		if(currentPiece == badColor) foundBadTile = TRUE;					//Note this and keep traversing.
		else if(currentPiece == goodColor) foundGoodTile = TRUE; 			//This will be the last traversal.
		else if(currentPiece == BOARD_TILE_EMPTY) foundEmptyTile = TRUE; 	//This will be the last traversal.
	}
	if(foundBadTile == TRUE && foundGoodTile == TRUE){
		serialPort.SendString("* checkN: TRUE!\r\n");
		return TRUE; //If we found a bad tile and a good tile, we can place here.
	} 
	else return FALSE;
}

BOOL ReversiBoard::checkS(BoardTile goodColor, BoardTile badColor, Tile position){
	serialPort.SendString("* checkS\r\n");
	char buf[64];

	Tile currentTile { position.x, position.y };
	BoardTile currentPiece = getSquare(position.x, position.y);
	BOOL foundGoodTile = FALSE;
	BOOL foundBadTile = FALSE;
	BOOL foundEmptyTile = FALSE;

	sprintf(buf, "currentTile is (%d, %d)\r\n", currentTile.x, currentTile.y);
	serialPort.SendString(buf);

	//Loop until we run off the board, find a good color tile, or find an empty tile.
	while(currentTile.isPositionValid() && (foundGoodTile == FALSE && foundEmptyTile == FALSE)) {
		currentTile = currentTile.S();
		currentPiece = getSquare(currentTile.x, currentTile.y);

		sprintf(buf, "Now looking at currentTile (%d,%d)\r\n", currentTile.x, currentTile.y);
		serialPort.SendString(buf);

		if(currentPiece == badColor) foundBadTile = TRUE;					//Note this and keep traversing.
		else if(currentPiece == goodColor) foundGoodTile = TRUE; 			//This will be the last traversal.
		else if(currentPiece == BOARD_TILE_EMPTY) foundEmptyTile = TRUE; 	//This will be the last traversal.
	}
	if(foundBadTile == TRUE && foundGoodTile == TRUE){
		serialPort.SendString("* checkS: TRUE!\r\n");
		return TRUE; //If we found a bad tile and a good tile, we can place here.
	} 
	else return FALSE;
}

BOOL ReversiBoard::checkSW(BoardTile goodColor, BoardTile badColor, Tile position){
	serialPort.SendString("* checkSW\r\n");
	char buf[64];

	Tile currentTile { position.x, position.y };
	BoardTile currentPiece = getSquare(position.x, position.y);
	BOOL foundGoodTile = FALSE;
	BOOL foundBadTile = FALSE;
	BOOL foundEmptyTile = FALSE;

	sprintf(buf, "currentTile is (%d, %d)\r\n", currentTile.x, currentTile.y);
	serialPort.SendString(buf);

	//Loop until we run off the board, find a good color tile, or find an empty tile.
	while(currentTile.isPositionValid() && (foundGoodTile == FALSE && foundEmptyTile == FALSE)) {
		currentTile = currentTile.SW();
		currentPiece = getSquare(currentTile.x, currentTile.y);

		sprintf(buf, "Now looking at currentTile (%d,%d)\r\n", currentTile.x, currentTile.y);
		serialPort.SendString(buf);

		if(currentPiece == badColor) foundBadTile = TRUE;					//Note this and keep traversing.
		else if(currentPiece == goodColor) foundGoodTile = TRUE; 			//This will be the last traversal.
		else if(currentPiece == BOARD_TILE_EMPTY) foundEmptyTile = TRUE; 	//This will be the last traversal.
	}
	if(foundBadTile == TRUE && foundGoodTile == TRUE){
		serialPort.SendString("* checkSW: TRUE!\r\n");
		return TRUE; //If we found a bad tile and a good tile, we can place here.
	} 
	else return FALSE;
}

BOOL ReversiBoard::checkSE(BoardTile goodColor, BoardTile badColor, Tile position){
	serialPort.SendString("* checkSE\r\n");
	char buf[64];

	Tile currentTile { position.x, position.y };
	BoardTile currentPiece = getSquare(position.x, position.y);
	BOOL foundGoodTile = FALSE;
	BOOL foundBadTile = FALSE;
	BOOL foundEmptyTile = FALSE;

	sprintf(buf, "currentTile is (%d, %d)\r\n", currentTile.x, currentTile.y);
	serialPort.SendString(buf);

	//Loop until we run off the board, find a good color tile, or find an empty tile.
	while(currentTile.isPositionValid() && (foundGoodTile == FALSE && foundEmptyTile == FALSE)) {
		currentTile = currentTile.SE();
		currentPiece = getSquare(currentTile.x, currentTile.y);

		sprintf(buf, "Now looking at currentTile (%d,%d)\r\n", currentTile.x, currentTile.y);
		serialPort.SendString(buf);

		if(currentPiece == badColor) foundBadTile = TRUE;					//Note this and keep traversing.
		else if(currentPiece == goodColor) foundGoodTile = TRUE; 			//This will be the last traversal.
		else if(currentPiece == BOARD_TILE_EMPTY) foundEmptyTile = TRUE; 	//This will be the last traversal.
	}
	if(foundBadTile == TRUE && foundGoodTile == TRUE){
		serialPort.SendString("* checkSE: TRUE!\r\n");
		return TRUE; //If we found a bad tile and a good tile, we can place here.
	} 
	else return FALSE;
}

BOOL ReversiBoard::checkNW(BoardTile goodColor, BoardTile badColor, Tile position){
	serialPort.SendString("* checkNW\r\n");
	char buf[64];

	Tile currentTile { position.x, position.y };
	BoardTile currentPiece = getSquare(position.x, position.y);
	BOOL foundGoodTile = FALSE;
	BOOL foundBadTile = FALSE;
	BOOL foundEmptyTile = FALSE;

	sprintf(buf, "currentTile is (%d, %d)\r\n", currentTile.x, currentTile.y);
	serialPort.SendString(buf);

	//Loop until we run off the board, find a good color tile, or find an empty tile.
	while(currentTile.isPositionValid() && (foundGoodTile == FALSE && foundEmptyTile == FALSE)) {
		currentTile = currentTile.NW();
		currentPiece = getSquare(currentTile.x, currentTile.y);

		sprintf(buf, "Now looking at currentTile (%d,%d)\r\n", currentTile.x, currentTile.y);
		serialPort.SendString(buf);

		if(currentPiece == badColor) foundBadTile = TRUE;					//Note this and keep traversing.
		else if(currentPiece == goodColor) foundGoodTile = TRUE; 			//This will be the last traversal.
		else if(currentPiece == BOARD_TILE_EMPTY) foundEmptyTile = TRUE; 	//This will be the last traversal.
	}
	if(foundBadTile == TRUE && foundGoodTile == TRUE){
		serialPort.SendString("* checkNW: TRUE!\r\n");
		return TRUE; //If we found a bad tile and a good tile, we can place here.
	} 
	else return FALSE;
}

BOOL ReversiBoard::checkNE(BoardTile goodColor, BoardTile badColor, Tile position){
	serialPort.SendString("* checkNE\r\n");
	char buf[64];

	Tile currentTile { position.x, position.y };
	BoardTile currentPiece = getSquare(position.x, position.y);
	BOOL foundGoodTile = FALSE;
	BOOL foundBadTile = FALSE;
	BOOL foundEmptyTile = FALSE;

	sprintf(buf, "currentTile is (%d, %d)\r\n", currentTile.x, currentTile.y);
	serialPort.SendString(buf);

	//Loop until we run off the board, find a good color tile, or find an empty tile.
	while(currentTile.isPositionValid() && (foundGoodTile == FALSE && foundEmptyTile == FALSE)) {
		currentTile = currentTile.NE();
		currentPiece = getSquare(currentTile.x, currentTile.y);

		sprintf(buf, "Now looking at currentTile (%d,%d)\r\n", currentTile.x, currentTile.y);
		serialPort.SendString(buf);

		if(currentPiece == badColor) foundBadTile = TRUE;					//Note this and keep traversing.
		else if(currentPiece == goodColor) foundGoodTile = TRUE; 			//This will be the last traversal.
		else if(currentPiece == BOARD_TILE_EMPTY) foundEmptyTile = TRUE; 	//This will be the last traversal.
	}
	if(foundBadTile == TRUE && foundGoodTile == TRUE){
		serialPort.SendString("* checkNE: TRUE!\r\n");
		return TRUE; //If we found a bad tile and a good tile, we can place here.
	} 
	else return FALSE;
}