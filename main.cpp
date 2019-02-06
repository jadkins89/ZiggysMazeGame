/** 
	Justin Adkins
 	Main.cpp for Ziggy Maze Game
 	This game is currently arranged to be a one-player maze game.
 	Collect the treasure, get to the exit, and don't get overtaken by the bad guys!
 */

#include "Maze.h"

int main(void) {
	Maze game;
	Player* bowie = new Player("David", true);
	game.NewGame(bowie, 2);
	while(!game.IsGameOver()) {
		std::cout << game;
		game.TakeTurn(game.GetNextPlayer());
	}
	// Show final board
	std::cout << game;


	return 0;
}