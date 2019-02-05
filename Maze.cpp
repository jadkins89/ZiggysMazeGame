#include "Maze.h"

Maze::Maze() {
	Board* new_board = new Board;
	board_ = new_board;

	// Dummy player used to test board
	Player* dummy = new Player("dummy", true);
	dummy->SetPosition({0,0});

	// Runs until a board has been found that has a path to victory
	while (!checkBoard(dummy)) {
		delete board_;
		Board* new_board = new Board;
		board_ = new_board;
		dummy->SetPosition({0,0}); 
	}
	delete dummy;

	turn_count_ = 0;
}

void Maze::NewGame(Player *human, const int enemies) {
	// Place human on board
	Position start = {0,0};
	human->SetPosition(start);
	players_.push_back(human);
	int col = board_->get_cols() - 1;

	// Array of different enemy types
	SquareType enemy[2] = {SquareType::Enemy1, SquareType::Enemy2};

	// Initialize enemies and place on board
	for (int i = 0; i < enemies; i++) {
		std::string name = "Enemy_" + std::to_string(i + 1);
		Player *new_baddie = new Player(name, false, enemy[i % 2]);
		
		Position e_start = {0, col - i};
		new_baddie->SetPosition(e_start);
		board_->SetSquareValue(e_start, new_baddie->get_squaretype());

		players_.push_back(new_baddie);
	}
}

void Maze::TakeTurn(Player *p) {
	Position temp = p->get_position();
	if (p->is_human()) {
		bool move_suc = false;
		do {
			std::string input;
			std::cin >> input;

			switch(toupper(input[0])) {
				case 'U':
					temp.row--;
					break;
				case 'D':
					temp.row++;
					break;
				case 'L':
					temp.col--;
					break;
				case 'R':
					temp.col++;
					break;
			}
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			move_suc = board_->MovePlayer(p, temp);
		} while (!move_suc);
	}
	// Random Strategy
	else {
		std::vector<Position> moves = board_->GetMoves(p);
		int index = rand() % moves.size();
		while ((board_->get_square_value(moves[index]) == SquareType::Enemy1 ||
			board_->get_square_value(moves[index]) == SquareType::Enemy2) &&
			moves.size() > 1) {
				moves.erase(moves.begin() + index);
				index = rand() % moves.size();
		}
		board_->MovePlayer(p, moves[index]);
	}
	// Increment turn unless on last player, then reset
	if (turn_count_ == players_.size() - 1) {
		turn_count_ = 0;
	}
	else {
		turn_count_++;	
	}
}

Player* Maze::GetNextPlayer() {
	return players_[turn_count_];

}

bool Maze::IsGameOver() const {
	if (board_->GetExitOccupant() == SquareType::Human) {
		return true;
	}
	Player *human = players_[0];

	// Code no longer necessary since boards are checked for path

	// std::vector<Position> moves = board_->GetMoves(human);
	// if (!moves.size()) {
	// 	std::cout << "NO POSSIBLE MOVES - GAME OVER" << std::endl;
	// 	return true;
	// }

	for (auto &e : players_) {
		// Only looking for bad guys
		if (e->is_human()) {
			continue;
		}
		// Check if the bad guys overtook the humans
		if  (human->get_position() == e->get_position()) {
			return true; 

		}
	}
	return false;
}

std::string Maze::GenerateReport() const {
	std::string output = "";
	for (auto &p : players_) {
		output += p->get_name() + " has " + std::to_string(p->get_points()) + " points.\n";
	}
	return output;
}

bool Maze::checkBoard(Player *p) {
	std::vector<Position> moves = board_->GetMoves(p);
	Player temp = *p;
	Position current = temp.get_position();

	if (board_->get_square_value(temp.get_position()) == SquareType::Exit) {
		return true;
	}

	if (board_->get_visited(current.row, current.col)) {
		return false;
	}

	board_->set_visited(current.row, current.col, true);
	for (int j = 0; j < moves.size(); j++) {
		temp.SetPosition(moves[j]);
		// Recursive call to each possible move
		if (checkBoard(&temp)) {
			return true;
		}
	}
	return false;
}


std::ostream& operator<<(std::ostream& os, const Maze &m) {
	os << *m.board_;

	if (!m.IsGameOver()) {
		os << m.players_[m.turn_count_]->get_name() << " can go: ";

		std::vector<Position> moves = m.board_->GetMoves(m.players_[m.turn_count_]);
		for (auto &cur_move : moves) {
			os << m.players_[m.turn_count_]->ToRelativePosition(cur_move) << " ";
		}
		os << std::endl;
		if (m.players_[m.turn_count_]->is_human()) {
			os << "Please enter your choice: ";
		}
		else {
			std::cout << "Hit enter to continue...";
    		std::cin.get();
		}
	}
	else {
		if (m.board_->get_won()) {
			os << "🤩 YOU'VE WON! BRAVO ZIGGY 🤩" << std::endl;
		}
		else {
			os << "☠️ BETTER LUCK NEXT TIME MR. STARDUST ☠️" << std::endl;
		}
		os << m.GenerateReport();
	}

	return os;
}