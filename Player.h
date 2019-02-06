#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <iostream>

enum class SquareType { Wall, Exit, Empty, Human, Enemy1, Enemy2, Treasure1, Treasure2, Treasure3 };

struct Position {
	int row;
	int col;

	bool operator==(const Position &other) {
		return row == other.row && col == other.col;
	}
	bool operator!=(const Position &other) {
		return row != other.row || col != other.col;
	}
	std::ostream& operator<<(std::ostream& os) {
		os << "[" << row << "," << col << "]";
		return os;
	}
};


class Player {
public:
	// Constructor
	Player(const std::string name, const bool is_human);
	
	// Inline Member Functions
	std::string get_name() const {return name_; }
	int get_points() const {return points_; }
	Position get_position() const {return pos_; }
	bool is_human() const {return is_human_; }
	SquareType get_squaretype() const {return char_image_; }

	void ChangePoints(const int x);  

	void SetPosition(Position pos);

	// ToRelativePosition is a function we used to translate positions
	// into directions relative to the player (up, down, etc)
	std::string ToRelativePosition(Position other);

	// Convert this player to a string representation of their name and points
	std::string Stringify();

	void set_squaretype(SquareType char_image);

private:
	std::string name_;
	int points_;
	Position pos_;
	bool is_human_;
	SquareType char_image_;

}; // class Player

#endif  // _PLAYER_H_
