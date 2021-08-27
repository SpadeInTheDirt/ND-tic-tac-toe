#pragma once

#include "Header.h"

class Board {
	
	int dimensions;
	int width; // how wide 1 dimension is (ex. 3x3x3 board is 3 wide in 1 dimensions)
	int size; // how many spaces in total the board has

	std::vector<char> spaces;
	
public:
	Board(const int& new_dimensions, const int& new_size);

	//getters
	int get_dimensions();
	int get_width();
	int get_size();
	int get_space(const std::vector<int>& coords);
	int get_space(const int& index);
	
	std::string to_string();

	std::vector<std::string> to_string_helper(const std::vector<int>& coord, const int& current_dimension);

	std::string coord_to_string(const std::vector<int>& coords);

	// common translations and such

	int coord_to_index(const std::vector <int>& coords);
	
	std::vector <int> coord_to_index(const int& index);

	void step_square(std::vector<int>& coords, const std::vector <int>& offset);

	// board verification

	bool check_square(const int& index);
	bool check_square(const std::vector<int>& coords);

	bool check_square_helper(const std::vector<int>& coord, const std::vector<int>& closest, const std::vector<int>& flip);
	bool check_square_helper(const std::vector<int>& parent, const std::vector<int>& child, const std::vector<int>& coord, const std::vector<int>& closest, const std::vector<int>& flip);
	
	bool check_diagonal(const int& index, const std::vector<int>& offset);
	bool check_diagonal(const std::vector<int>& coords, const std::vector<int>& offset);

	
	// board modification


	void expand_board_dimensions(const std::vector <int>& add_dimensions);

	void expand_board_size(const std::vector <int>& add_positions);

	void contract_board_dimensions(const std::vector <int>& remove_dimensions);

	void contract_board_size(const std::vector <int>& remove_positions);

	void set_square_at(const char& chr, const std::vector<int>& coord);

	void set_square_at(const char& chr, const int& index);

};