#include "Maze.h"

/**
	Maze Constructor: inializes a board and a dummy player to check the board to make sure it is "winnable". Deletes dummy player after successful completion of search and initializes turn_count_ to zero.
*/
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

/**
	NewGame places the human passed in as a parameter on the board and initializes the enemy players and places them on the board.

	@param human: a pointer to the human player object
	@param enemies: the number of enemies that need to be initialized 
*/
void Maze::NewGame(Player *human, const int enemies) {
	// Place human on board
	Position start = {0,0};
	human->SetPosition(start);
	players_.push_back(human);
	int col = board_->get_cols() - 1;

	// Initialize enemies and place on board
	for (int i = 0; i < enemies; i++) {
		std::string name = "Enemy_" + std::to_string(i + 1);
		Player *new_baddie = new Player(name, false);
		
		Position e_start = {0, col - i};
		new_baddie->SetPosition(e_start);
		board_->SetSquareValue(e_start, new_baddie->get_squaretype());

		players_.push_back(new_baddie);
	}
}

/**
	TakeTurn checks to see if the current turn is a human or an enemy's. 

	If a human, it prompts the user for input and makes sure it is valid. If valid it saves the requested position move in a temp variable and calls the MovePlayer to request the move. If true, we continue on. Else, we prompt the user again because the move is illegal.

	If an enemy, TakeTurn uses a random strategy that moves the enemy to a position that it can occupy "randomly". If it can avoid attempting to move into another enemy, it does so.

	Lastly, the turn_count_ is incremented unless on the last player, then it is reset to zero.

	@param p: A pointer to the player who is taking a turn
*/
void Maze::TakeTurn(Player *p) {
	Position temp = p->get_position();
	if (p->is_human()) {
		bool move_suc = false;
		do {
			std::cout << "Please enter your choice: ";
			std::string input;
			std::cin >> input;

			// Check to see if user entered correct input
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
		// If positions are available that are not running into another enemy, use those positions
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

/**
	GetNextPlayer is an accessor function that returns the player whose "turn" it is.
*/
Player* Maze::GetNextPlayer() {
	return players_[turn_count_];

}

/**
	IsGameOver is a method that checks to see if the game conditions have caused a "Game Over" scenario. It checks if the human is on the exit, and if the human has been overtaken by enemies.

	@return: True if game is over, false otherwise
*/
bool Maze::IsGameOver() const {
	// We've done it! Human is on the exit position
	if (board_->GetExitOccupant() == SquareType::Human) {
		return true;
	}
	// Save human to check against enemy position
	Player *human = players_[0];

	// Checks all enemies positions against the human position
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

/**
	GenerateReport steps through the player vector and builds a string containing their names and scores. A helper method of the Player class called Stringify is used to build each players portion.

	@return string: This final string contains all the players and scores, each on a new line.
*/
std::string Maze::GenerateReport() const {
	std::string output = "";
	for (auto &p : players_) {
		output += p->Stringify();
	}
	return output;
}

/**
	checkBoard is a recursive function whose goal is to check to see if the board is solvable. It works by having a player starting at the top left, try all possible positions from there until they hit the exit or have no more moves. Visited positions are marked to not double back.

	@param p: A player pointer, which is used to move around the board.
	@return: True if board is winnable, false otherwise.
*/
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

/**
	Overloaded << operator so we can output the game based on its current state.

	@return: a reference to an ostream that contains the output we've built based on game state.
*/
std::ostream& operator<<(std::ostream& os, const Maze &m) {
	os << *m.board_;

	if (!m.IsGameOver()) {
		os << m.players_[m.turn_count_]->get_name() << " can go: ";

		std::vector<Position> moves = m.board_->GetMoves(m.players_[m.turn_count_]);
		// Print all possible moves
		for (auto &cur_move : moves) {
			os << m.players_[m.turn_count_]->ToRelativePosition(cur_move) << " ";
		}
		os << std::endl;
		if (!m.players_[m.turn_count_]->is_human()) {
			std::cout << "Hit enter to continue...";
    		std::cin.get();
		}
	}
	// Game Over scenarios, calls GenerateReport to output scores
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