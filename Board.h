#ifndef _BOARD_H_
#define _BOARD_H_

#include "Player.h"
#include <vector>

#define ROW 6
#define COL 6

class Board {
public:
	Board();
	
	// Inline Member Functions
	int get_rows() const {return rows_; }
	int get_cols() const {return cols_; }
	bool get_visited(int row, int col) {return visited_[row][col]; }

	// Setters & Getters
	void set_visited(int row, int col, bool val);
	SquareType get_square_value(Position pos) const; 
	bool get_won() const {return won_; }
	void SetSquareValue(Position pos, SquareType value);
	SquareType GetExitOccupant();

	// Public Methods
	std::vector<Position> GetMoves(Player *p);
	bool MovePlayer(Player *p, Position pos);

	// Overloads
	friend std::ostream& operator<<(std::ostream& os, const Board &b);

private:
	SquareType arr_[ROW][COL];
	int rows_;
	int cols_;
	bool won_;
	bool visited_[ROW][COL];
	
};  // class Board

#endif // _BOARD_H_