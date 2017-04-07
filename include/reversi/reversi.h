#pragma once

#include <stdio.h>
#include <string.h>

#include <graphics/gfxmacros.h>

#include <intuition/intuition.h>
#include <proto/graphics.h>
#include <proto/intuition.h>

#include "colors.h"

#include "reversi/defines.h"
#include "reversi/strings.h"

#include "serial.h"

//Flood fill
extern short bm_w, bm_h;
extern UBYTE *floodBuffer;
extern struct TmpRas tr;

typedef enum game_state_t {
	GAME_TURN_WHITE,
	GAME_TURN_BLACK,
	GAME_OVER
} GameState;

#include "reversi/board.h"

void REV_DrawBoard(struct Window *window);
void REV_DrawPieces(struct Window *window);
void REV_DrawScores(struct Window *window);

void REV_PlaceBackground(struct RastPort *rp, UBYTE pen, int x, int y);
void REV_PlacePiece(struct RastPort *rp, UBYTE color, int x, int y);

Tile REV_PixelToTile(UWORD x, UWORD y);
void REV_HandleTileClick(struct Window *window, UWORD mouseX, UWORD mouseY);
void REV_PrintGameStatus(struct Window *window);

UBYTE REV_CheckMoveLegality(Tile tile);
//void REV_FlipPieces(struct RastPort *rp, Tile origin, UBYTE directions, BoardTile goodColor, BoardTile badColor);
void REV_FlipPieces(struct RastPort *rp, ReversiBoard *pBoard, Tile origin, UBYTE directions, BoardTile goodColor);

void REV_AdvanceGameState(struct Window *window);

extern GameState gameState;