#include "reversi/mcarlo.h"
#include "reversi/reversi.h"

#include <vector>

BOOL AI_AnyValidMoves(BoardTile goodColor, BoardTile badColor)
{
	for (int i = 0; i < BOARD_TILE_COUNT; i++) {
		Tile tile = Tile{ i % 8, i / 8 };

		if (board.checkDirection(goodColor, badColor, tile))
			return true;
	}

	return false;
}

std::vector<Tile> AI_FindAllValidMoves(ReversiBoard simBoard, BoardTile goodColor, BoardTile badColor) {
	std::vector<Tile> movesList; //all valid moves for this board

	for (int i = 0; i < BOARD_TILE_COUNT; i++) {
		Tile tile = Tile{ i % 8, i / 8 };

		if (board.checkDirection(goodColor, badColor, tile))
			movesList.push_back(tile);
	}

	return movesList;
}

void AI_MonteCarlo(ReversiBoard board, BoardTile goodColor, BoardTile badColor) {
	//Create a parent node and manage the Monte Carlo AI.
	NodeMonteCarlo root = NodeMonteCarlo(board, nullptr);

	AI_MonteCarloSimulation(root, goodColor, badColor);
}

GameResult AI_MonteCarloSimulation(NodeMonteCarlo node, BoardTile goodColor, BoardTile badColor) {
	//Perform a simulation.

	std::vector<Tile> movesList; //all valid moves for this board

	if (AI_AnyValidMoves(goodColor, badColor)) {
		movesList = AI_FindAllValidMoves(node.board, goodColor, badColor);

		//Perform a random move.
		int random = rand() % movesList.size();
		auto directionFlags = board.checkDirection(BOARD_TILE_WHITE, BOARD_TILE_BLACK, Tile{ 2, 4 });
		REV_FlipPieces(nullptr, &(node.board), movesList[random], directionFlags, goodColor);

		node.board.print();

		//AI_MonteCarloSimulation(movesList[random], badColor, goodColor); //flip goodcolor and badcolor for the next move
		return RESULT_DRAW;
	}

	return RESULT_DRAW;
}