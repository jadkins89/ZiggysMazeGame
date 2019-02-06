#ifndef _MAZE_H_
#define _MAZE_H_

#include "Player.h"
#include "Board.h"

class Maze {
public:
	// Constructor
	Maze();

	// Public Methods
	void NewGame(Player *human, const int enemies);
	void TakeTurn(Player *p);
	Player* GetNextPlayer();
	bool IsGameOver() const;
	std::string GenerateReport() const;
	
	// Overload
	friend std::ostream& operator<<(std::ostream& os, const Maze &m);

private:
	// Private Method
	bool checkBoard(Player *p);

	// Private Variables
	Board *board_; 
	std::vector<Player *> players_;
	int turn_count_;
};  // class Maze

#endif  // _MAZE_H_
