#ifndef _MAZE_H_
#define _MAZE_H_

#include <vector>
#include "Player.h"
#include "Board.h"

class Maze {
public:
	Maze(); // constructor

	// initialize a new game, given one human player and 
	// a number of enemies to generate
	void NewGame(Player *human, const int enemies);

	// have the given Player take their turn
	void TakeTurn(Player *p);

	// Get the next player in turn order
	Player* GetNextPlayer();

	// return true iff the human made it to the exit 
	// or the enemies ate all the humans
	bool IsGameOver() const;

	// string info about the game's conditions after it is over
	std::string GenerateReport() const;
	
	friend std::ostream& operator<<(std::ostream& os, const Maze &m);

private:
	bool checkBoard(Player *p);

	Board *board_; 
	std::vector<Player *> players_;
	int turn_count_;
};  // class Maze

#endif  // _MAZE_H_
