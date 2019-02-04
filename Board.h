#ifndef _BOARD_H_
#define _BOARD_H_

#include "Player.h"
#include <vector>

#define ROW 4
#define COL 4

class Board {
public:
	Board();
	
	// Inline Member Functions
	int get_rows() const {return rows_; }
	int get_cols() const {return cols_; }
	bool get_visited(int row, int col) {return visited_[row][col]; }

	// Other Setters & Getters
	void set_visited(int row, int col, bool val);
	SquareType get_square_value(Position pos) const; 
	bool get_won() const {return won_; }

	// set the value of a square to the given SquareType
	void SetSquareValue(Position pos, SquareType value);

	// get the possible Positions that a Player could move to
	// (not off the board or into a wall)
	std::vector<Position> GetMoves(Player *p);

	// Move a player to a new position on the board. Return
	// true if they moved successfully, false otherwise.
	bool MovePlayer(Player *p, Position pos);

	// Get the square type of the exit square
	SquareType GetExitOccupant();

	friend std::ostream& operator<<(std::ostream& os, const Board &b);

private:
	SquareType arr_[ROW][COL];
	
	int rows_; // might be convenient but not necessary
	int cols_;
	bool won_;
	bool visited_[ROW][COL];
	
};  // class Board

#endif // _BOARD_H_