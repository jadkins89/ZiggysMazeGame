// Justin Adkins
// Player Implementation

#include "Player.h"

Player::Player(const std::string name, const bool is_human) : name_(name), is_human_(is_human) {
	char_image_ = SquareType::Human;
	points_ = 0;
}

Player::Player(const std::string name, const bool is_human, SquareType char_image) : name_(name), is_human_(is_human), char_image_(char_image) {
	points_ = 0;
}

void Player::ChangePoints(const int x) {
	points_ += x;
}

void Player::SetPosition(Position pos) {
	pos_ = pos;
}

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

std::string Player::Stringify() {
	return name_ + " " + std::to_string(points_);
}

void Player::set_squaretype(SquareType char_image) {
	char_image_ = char_image;
}