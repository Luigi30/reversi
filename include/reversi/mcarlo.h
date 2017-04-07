#pragma once

#include <cstdlib>
#include <vector>
#include "board.h"

#define MAX_DEPTH 3

typedef struct nodeMonteCarlo_t {
	nodeMonteCarlo_t *parent;
	std::vector<nodeMonteCarlo_t*> children;
	ReversiBoard board;
	int wins;	//number of wins in child simulations
	int visits;	//number of simulations run from this node

	nodeMonteCarlo_t(ReversiBoard _board, nodeMonteCarlo_t *_parent) {
		board = _board;
		parent = _parent;
		wins = 0;
		visits = 0;
	}

} NodeMonteCarlo;

typedef enum gameResult_t {
	RESULT_WHITE_WINS,
	RESULT_BLACK_WINS,
	RESULT_DRAW
} GameResult;

BOOL AI_AnyValidMoves(BoardTile goodColor, BoardTile badColor);
void AI_MonteCarlo(ReversiBoard board, BoardTile goodColor, BoardTile badColor);
GameResult AI_MonteCarloSimulation(NodeMonteCarlo node, BoardTile goodColor, BoardTile badColor);