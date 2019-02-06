// Justin Adkins
// Board Implementation

#include "Board.h"

// Definitions for Probability and Points
#define WALL_PROB 20
#define TRES_PROB 10
#define TRES_PTS1 100
#define TRES_PTS2 75
#define TRES_PTS3 50
#define EXIT_PTS 200

/**
	SquareTypeStringify takes a SquareType and returns an emoji for the specified SquareType.

	@param sq: SquareType of specific element of board
	@return: string containing the emoji
*/
std::string SquareTypeStringify(SquareType sq) {
	switch(sq) {
		case(SquareType::Wall):
			return "🌋";
		case(SquareType::Exit):
			return "🚀";
		case(SquareType::Empty):
			return "⬜️";
		case(SquareType::Human):
			return "👨🏿‍🎤";
		case(SquareType::Enemy1):
			return "👽";
		case(SquareType::Enemy2):
			return "🧟‍♂️ ";
		case(SquareType::Treasure1):
			return "🎸";
		case(SquareType::Treasure2):
			return "🤘";
		case(SquareType::Treasure3):
			return "🍺";
	}
}

/**
	Board Constructor: Builds the board. Uses randomization to create unique boards. Two for loops are used to iterate through the 2D board space and place empty spaces, walls, and treasures. Lastly, the human and exit are placed. We also initialize the visted_ array to false for Board verification.
*/
Board::Board() : rows_(ROW), cols_(COL) {
	struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
	srand((time_t)ts.tv_nsec); // using nanosecond time to seed
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < cols_; j++) {
			// setting equal to true if number is less than given probability
			bool wall = (rand() % 100) <= WALL_PROB;
			bool treasure = (rand() % 100) <= TRES_PROB;
			// Array of treasure types to implement different treasures
			SquareType tres[3] = {SquareType::Treasure1, SquareType::Treasure2, SquareType::Treasure3};
			if (wall) {
				arr_[i][j] = SquareType::Wall;
			}
			else if (treasure) {
				arr_[i][j] = tres[i % 3];
			}
			else {
				arr_[i][j] = SquareType::Empty;
			}
		}
	}
	// Set Human and Exit positions
	arr_[0][0] = SquareType::Human;
	arr_[rows_ - 1][cols_ - 1] = SquareType::Exit;
	won_ = false;

	// Initialize all visted_ array to false
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			visited_[i][j] = false;
		}
	}
}

/**
	set_visited is an accessor method for checking if a specific space has been visited. This will be used by Maze when calling the checkBoard method.
*/
void Board::set_visited(int row, int col, bool val) {
	visited_[row][col] = val;
}

/**
	get_square_value is an accessor method for getting the SquareType of a specific position.
*/
SquareType Board::get_square_value(Position pos) const {
	return arr_[pos.row][pos.col];
}

/**
	SetSquareValue is an accessor method for setting the SquareType of a specific position.
*/
void Board::SetSquareValue(Position pos, SquareType value) {
	arr_[pos.row][pos.col] = value;
}

/**
	GetMoves finds the moves that are available to a specific player.

	@param p: The player whose moves we are finding.
	@return: A vector of positions that are viable for the player.
*/
std::vector<Position> Board::GetMoves(Player *p) { 
	Position player_pos = p->get_position();

	// Initialize all possible positions
	Position up, down, left, right;
	up = down = left = right = player_pos;
	up.row--;
	down.row++;
	left.col--;
	right.col++;

	std::vector<Position> list, temp;
	temp = {up, down, left, right};

	// Find valid moves 
	for (auto &cur_pos : temp) {
		// Checks to see if outside of board boundaries
		if ( !(cur_pos.col < 0) && 
			 !(cur_pos.row < 0) && 
			 !(cur_pos.row > (rows_ - 1)) &&
			 !(cur_pos.col > (cols_ - 1)) &&
			 // Adds to list of moves if not a Wall
			 (get_square_value(cur_pos) != SquareType::Wall)) {
				// Makes sure enemy isn't given move to occupy exit
				if (p->is_human() || 
					get_square_value(cur_pos) != SquareType::Exit) {
					list.push_back(cur_pos);
				}
		}
	}
	return list;
}

/**
	MovePlayer attempts to change a players position and do various things based on the position they are moving into. If move is illegal it returns false, otherwise true.

	@param p: A pointer to the player that is in motion
	@param pos: The requested position they would like to move to
	@return: True if successful move, false otherwise
*/
bool Board::MovePlayer(Player *p, Position pos) {
	std::vector<Position> moves = GetMoves(p);
	for (auto &m : moves) {
		// Found attempted move in list of moves
		if (m == pos) {
			// Considers different movement collisions
			switch (arr_[pos.row][pos.col]) {
				// Forfeit turn to enemy that moves into another enemy
				case SquareType::Enemy1:
				case SquareType::Enemy2:
					if (!p->is_human()) {
						return true;
					}
					// If human moves into enemy
					else {
						SetSquareValue(p->get_position(), SquareType::Empty);
						p->SetPosition(pos);
						return true;
					}
				// Handles different treasure point allotments
				case SquareType::Treasure1:
					p->ChangePoints(TRES_PTS1);
					break;
				case SquareType::Treasure2:
					p->ChangePoints(TRES_PTS2);
					break;
				case SquareType::Treasure3:
					p->ChangePoints(TRES_PTS3);
					break;
				case SquareType::Exit:
					if (!p->is_human()) {
						return false;
					}
					// Give points and sets won to true if human finishes the game
					else {
						won_ = true;
						p->ChangePoints(EXIT_PTS);		
					}
				default:
					break;
			}
			SetSquareValue(p->get_position(), SquareType::Empty);
			SetSquareValue(pos, p->get_squaretype());

			p->SetPosition(pos);
			return true;
		}
	}
	return false;
}

/**
	GetExitOccupant is an accessor method for returning the SquareType that is currently set to the exit. This is useful for checking if the game has been won.
*/
SquareType Board::GetExitOccupant() {
	return arr_[rows_-1][cols_-1];
}

/**
	Overloaded << operator to output the image of the board. It loops through the SquareType 2D array and outputs them row by row.
*/
std::ostream& operator<<(std::ostream& os, const Board &b) {
	int rows = b.rows_;
	int cols = b.cols_;

	// Visualize the board by printing out each element in the 2D array to the terminal
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			os << SquareTypeStringify(b.arr_[i][j]);
		}
		os << std::endl;
	}
	return os;
}