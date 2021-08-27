#include "Header.h"
#include "Board.h"

bool check_sequence(const std::vector < char > &sequence);
void print_board(const std::vector<std::vector<char>> &board);
void print_sequences(const std::vector<int>& parent, const std::vector<int>& child);
void print_sequences(const std::vector<int>& child);

inline
void print_line(const std::vector<char>& sequence);

const int DIMENSIONS = 3;
const int WIDTH = 4;
const int PLAYERS = 2;
const std::vector<char> PLAYER_TOKENS = {'X', 'O', 'A', 'H'};

int main() {
	
	//std::vector<std::vector<char>> board = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };

	//print_board(board);

	bool game_end = false;
	int winner = -1;
	int input;
	int current_player;

	std::vector <int> coord(DIMENSIONS);
	bool valid_coord_input = false;
	bool valid_input = false;

	Board board(DIMENSIONS, WIDTH);
	
	

	for (int i = 0; i < board.get_size(); i++) {
		std::cout << board.to_string();

		current_player = (i % PLAYERS);
		std::cout << "Player " << current_player + 1 << "'s turn. Your pieces are " << PLAYER_TOKENS[current_player] << ". Enter " << DIMENSIONS << " coordinates to place.\n";

		valid_coord_input = false;

		while (!valid_coord_input) {
			for (int j = 0; j < DIMENSIONS; j++) {
				valid_input = false;

				while (!valid_input) { // continually retry getting input
					try {
						std::cin >> input;
						if (input <= WIDTH && input > 0) {
							valid_input = true;
						}
						else {
							std::cout << "Coordinate " << j + 1 << " was not between 1 and " << WIDTH << "\n";
						}
					}
					catch (const std::exception& e) {
						std::cout << "Bad input for coordinate " << j + 1 << ", please retry." << "\n";
					}
				}

				coord[j] = input - 1; // input coords start at 1, code handles coords starting at 0
			}

			if (board.get_space(coord) != ' ') { // if the space wasnt empty, try again
				std::cout << "Square was occupied, enter a different coordinate.\n";
			}
			else {
				valid_coord_input = true;
			}
		}

		board.set_square_at(PLAYER_TOKENS[current_player], coord);
		game_end = board.check_square(coord);
		if (game_end) {
			winner = current_player;
		}
		std::cout << "\033[2J\033[1;1H"; // ? ? ? clears screen ? ? ? i dont know ? ? ?
	}

	std::cout << "GAME OVER - ";
	if (winner == -1) {
		std::cout << "TIE\n";
	} else {
		std::cout << "PLAYER " << winner + 1 << " WINS!\n";
	}

	//board.set_square_at('X', { 0, 0, 0, 0 });
	//board.set_square_at('X', { 1, 0, 0, 0 });
	//board.set_square_at('X', { 2, 0, 0, 0 });
	//board.set_square_at('O', { 3, 0, 0, 0 });

	//std::cout << board.to_string() << "\n";
	//std::cout << board.check_square({ 0, 0, 0, 0 }) << "\n";
	//std::cout << board.check_diagonal({ 0, 0, 0, 0 }, { 1, 0, 0, 0 }) << "\n";
	//std::cout << board.check_diagonal({ 0, 0, 0, 0 }, { 1, 1, 0, 0 }) << "\n";
	
	//std::cout << board.coord_to_string({0, 2, 3, 4});
	return 0;
}

void print_sequences(const std::vector<int>& child) {
	print_sequences({ }, child);
}

void print_sequences(const std::vector<int>& parent, const std::vector<int>& child) {
	if (child.size() == 0) {
		
		for (const int& i : parent) {
			std::cout << i;
		}
		std::cout << "\n";
		
	} else {

		std::vector <int> new_parent(parent); //copy
		std::vector <int> new_child(child);

		new_child = slice(new_child, 1, (int)new_child.size());
		print_sequences(new_parent, new_child);

		new_parent.push_back(child[0]);
		print_sequences(new_parent, new_child);


	}
}

bool check_sequence(const std::vector < char > &sequence) {
	if (sequence.size() == 0) { // check if the sequence is 0 chars long
		return true;
	}
	else {
		char first_char = sequence[0];
		for (int i = 1; i < sequence.size(); i++) {
			if (first_char != sequence[i]) {
				return false;
			}
		}

		return true;
	}
}

void print_board(const std::vector<std::vector<char>> &board) {
	if (board.size() > 0) { // if the board has any rows
		
		std::string line_delimiter = "";
		int line_size;

		//print the first line
		print_line(board[0]);

		for (int y = 1; y < board.size(); y++) {

			//construct delimiter for lines
			line_delimiter = "-";
			line_size = (int)((board[y].size() > board[y - 1].size()) ? board[y].size() : board[y-1].size());
			
			for (int i = 1; i < line_size; i++) {
				line_delimiter += "--";
			}

			std::cout << line_delimiter << "\n";

			print_line(board[y]);

		}
	}

	
}

inline
void print_line(const std::vector<char> &sequence) {
	if (sequence.size() != 0) {
		std::cout << sequence[0];
		
		for (int i = 1; i < sequence.size(); i++) {
			std::cout << "|" << sequence[i];
		}

		std::cout << "\n";
	}
}