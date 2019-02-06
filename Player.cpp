// Justin Adkins
// Player Implementation

#include "Player.h"

/**
	Player Constructor: Inializes Player objects private variables. If enemy, it randomly chooses one of the two SquareTypes associated with the enemies.
*/
Player::Player(const std::string name, const bool is_human) : name_(name), is_human_(is_human) {
	// Array of different enemy types
	SquareType enemy[2] = {SquareType::Enemy1, SquareType::Enemy2};
	if (is_human) {
		char_image_ = SquareType::Human;
	}
	else {
		// Choose random character image
		char_image_ = enemy[rand() % 2];
	}
	points_ = 0;
}

/**
	ChangePoints is an accessor method to change the value of a players points
*/
void Player::ChangePoints(const int x) {
	points_ += x;
}

/**
	SetPosition is an accessor method to set the pos_ variable
*/
void Player::SetPosition(Position pos) {
	pos_ = pos;
}

/**
	ToRelativePosition looks to see if the position passed in is up, down, left, or right of the players current position.

	@param other: this is the position we are locating around the players current position.
	@return: A string with either the direction of the position, or invalid if an immediate position wasn't given.
*/
std::string Player::ToRelativePosition(Position other) {
	if (other.row == pos_.row) {
		if (other.col == pos_.col - 1) {
			return "LEFT";
		}
		else if (other.col == pos_.col + 1) {
			return "RIGHT";
		}
	}
	else if (other.col == pos_.col) {
		if (other.row == pos_.row + 1) {
			return "DOWN";
		}
		else if (other.row == pos_.row - 1) {
			return "UP";
		}
	}
	return "INVALID"; // Probably shouldn't get here
}

/**
	Stringify is a small helper method to print a players name and points
*/
std::string Player::Stringify() {
	return name_ + " has " + std::to_string(points_) + ".\n";
}

/**
	set_squaretype is an accessor method to set the char_image_ variable
*/
void Player::set_squaretype(SquareType char_image) {
	char_image_ = char_image;
}