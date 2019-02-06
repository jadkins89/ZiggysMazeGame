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
	
	// Setters & Getters
	std::string get_name() const {return name_; }
	int get_points() const {return points_; }
	Position get_position() const {return pos_; }
	bool is_human() const {return is_human_; }
	SquareType get_squaretype() const {return char_image_; }
	void SetPosition(Position pos);
	void set_squaretype(SquareType char_image);

	// Public Methods
	void ChangePoints(const int x);  
	std::string ToRelativePosition(Position other);
	std::string Stringify();
private:
	std::string name_;
	int points_;
	Position pos_;
	bool is_human_;
	SquareType char_image_;

}; // class Player

#endif  // _PLAYER_H_
