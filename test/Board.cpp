#include "Board.h"

Board::Board(const int& new_dimensions, const int& new_size) {
	
	dimensions = new_dimensions;
	width = new_size;

	size = int_pow(width, dimensions);

	for (int i = 0; i < size; i++) {
		spaces.push_back(' ');
	}

}

// board general translations and functions
int Board::get_dimensions() {
	return dimensions;
}
int Board::get_width() {
	return width;
}
int Board::get_size() {
	return size;
}
int Board::get_space(const std::vector<int>& coords) {
	return spaces[coord_to_index(coords)];
}
int Board::get_space(const int& index) {
	return spaces[index];
}

std::string Board::to_string() {
	std::string ret = "";
	std::vector<std::string> board_strings = to_string_helper({}, dimensions);

	for (const std::string& i : board_strings) {
		ret += i + "\n";
	}

	return ret;
}

std::vector<std::string> Board::to_string_helper(const std::vector<int>& coords, const int& current_dimension) { // goes backwards from top to bottom (higher dimension calls to_string_helper with one dimension lower)
	//coord is the last however many coordinates of the board, with the coordinates to the "left" or missing in front of the given being those that have already been assembled
	//also they are reversed to make going down dimensions easier

	if (current_dimension <= 1) {
		// hard assemble a single line
		//add a 0 to the front of the coords
		std::vector<int> forwards_coords = { 0 };

		for (const int& i : coords) {
			forwards_coords.push_back(i);
		}

		int starter = coord_to_index(forwards_coords);
		

		std::string ret = "";

		for (int i = starter; i < starter + width; i++) {
			ret += spaces[i];
		}
		//std::cout << "any poggers?";
		return {ret};
	}
	if (current_dimension%2 == 0) { // vertical append
		
		// add an element to the front of the coords
		std::vector<int> new_coords{ 0 };

		for (const int& i : coords) {
			new_coords.push_back(i);
		}

		std::vector<std::string> ret_board_strings(to_string_helper(new_coords, current_dimension - 1)); // initialize ret_board_strings as the first of the next boards to assemble together
		std::vector<std::string> collected_board_strings;

		int board_strings_amount = ret_board_strings.size();

		// assemble the delimiter. the delimiter is as long as the given strings.
		std::string delimiter = "";
		
		for (int i = 0; i < ret_board_strings[0].size(); i++) {
			delimiter += "-";
		}

		for (int i = 1; i < width; i++) {
			
			new_coords[0] = i; // adjust the coords

			// collect the right-adjacent block and slap them on the end of the board strings to return
			collected_board_strings = to_string_helper(new_coords, current_dimension - 1);

			// add the delimiter. the delimiter will grow for every 2 dimensions, starting at 0
			for (int i = 0; i < (current_dimension-1)/ 2; i++) {
				ret_board_strings.push_back(delimiter);
			}

			for (int j = 0; j < board_strings_amount; j++) {
				// add the delim first to each board string
				ret_board_strings.push_back(collected_board_strings[j]);
			}
		}

		return ret_board_strings;

	} else { // horizontal append
		
		// assemble the delimiter. the delimiter will grow for every 2 dimensions, starting at 0
		std::string delimiter = "";
		for (int i = 0; i < current_dimension / 2; i++) {
			delimiter += "|";
		}

		// add an element to the front of the new coords
		std::vector<int> new_coords{ 0 };

		for (const int& i : coords) {
			new_coords.push_back(i);
		}
		
		std::vector<std::string> ret_board_strings(to_string_helper(new_coords, current_dimension - 1)); // initialize ret_board_strings as the first of the next boards to assemble together
		std::vector<std::string> collected_board_strings;

		int board_strings_amount = ret_board_strings.size();

		for (int i = 1; i < width; i++) {
			
			new_coords[0] = i; // adjust the coords

			// collect the right-adjacent block and slap them on the ends of the board strings to return
			collected_board_strings = to_string_helper(new_coords, current_dimension - 1);

			for (int j = 0; j < board_strings_amount; j++) {
				// add the delim first to each board string
				ret_board_strings[j] += delimiter + collected_board_strings[j];
			}
		}

		return ret_board_strings;
	}
}

std::string Board::coord_to_string(const std::vector<int>& coords) {

	if (coords.size() != 0) {
		std::string ret = "{ ";

		ret += std::to_string(coords[0]);

		for (int i = 1; i < coords.size(); i++) {
			ret += ", " + std::to_string(coords[i]);
		}

		ret += " }";
		return ret;
		
	}

	return "{ }";
}

int Board::coord_to_index(const std::vector <int>& coords) {
	if (coords.size() != dimensions) return -1; //if there isnt the correct amount of coordinates

	int ret = 0;

	for (int i = 0; i < dimensions; i++) {
		ret += coords[i] * int_pow(width, i);
	}

	return ret;


}

std::vector <int> Board::coord_to_index(const int& index) {
	std::vector<int> ret(dimensions);

	for (int i = 0; i < dimensions; i++) {
		ret[i] = (index/(int_pow(width, i)))%width;
	}

	return ret;
}

void Board::step_square(std::vector<int>& coords, const std::vector<int>& offset) {
	int temp_val;

	for (int i = 0; i < dimensions; i++) {
		temp_val = coords[i] + offset[i];

		if (temp_val < 0) {
			coords[i] = (width + temp_val % width) % width;
		}
		else {
			coords[i] = temp_val % width;
		}
	}
}

//board verification functions

bool Board::check_square(const int& index) {
	return check_square(coord_to_index(index)); // convert to coords and use other function
}

bool Board::check_square(const std::vector<int>& coords) {
	//compile a list of all the coords' closest distance to an edge along their respective axis
	//also compile which coords are closer to the further edge and which are closer to 0 side
	//
	//ex. in a 4 wide, 2d board, the coords (1, 2) (coords starting from 0) gets translated to (1, 1)
	//with the second coord marked as closer to the opposite edge

	std::vector <int> closest(dimensions);
	std::vector <int> flip_data(dimensions);

	int halfway = (width-1) / 2; //compensate for index ex: width 4, 0 and 1 are not flipped, width 3, 0 and 1 are not flipped

	for (int i = 0; i < dimensions; i++) {
		if (coords[i] <= halfway) {
			closest[i] = coords[i];
			flip_data[i] = 1;
		
		} else {
			closest[i] = width - coords[i];
			flip_data[i] = -1;
		}
	}

	int perms = int_pow(2, dimensions);
	for (int i = 1; i < perms; i++) {
		
	}

	//std::cout << coord_to_string(coords) << coord_to_string(closest) << coord_to_string(flip_data) << "\n";

	return check_square_helper(coords, closest, flip_data);
}

bool Board::check_square_helper(const std::vector<int>& coord, const std::vector<int>& closest, const std::vector<int>& flip) {
	std::vector <int> closest_indexes(0);

	for (int i = 0; i < dimensions; i++) {
		closest_indexes.push_back(i);
	}
	
	return check_square_helper({}, closest_indexes, coord, closest, flip);
}

bool Board::check_square_helper(const std::vector<int>& parent, const std::vector<int>& child, const std::vector<int>& coord, const std::vector<int>& closest, const std::vector<int>& flip) {
	if (child.size() == 0) {
		if (parent.size() != 0) {
			
			// check if all the coords selected from closest are the same
			bool parents_same = true;
			int first_coord = closest[parent[0]];

			for (int i = 1; i < parent.size(); i++) {
				if (closest[parent[i]] != first_coord) {
					parents_same = false;
					i = (int) parent.size(); // break
				}
			}


			if (parents_same) {
				// if all the coords selected from closest are the same, create an offset coord, then run check_diagonal using the coord and offset coords
				std::vector<int> offset(dimensions);
				for (const int& i : parent) {
					// for each index in the permutation, set offset at that index to the value of flip at the same index
					offset[i] = flip[i];
				}

				return check_diagonal(coord, offset);

			} else {
				// if this premutation is not valid - not all of the selected closest offsets for each coord are the same, return false
				return false;
			}


		} else {
			// if both parent and child are empty, return false
			return false;
		}

	}
	else {

		std::vector <int> new_parent(parent); //copy
		std::vector <int> new_child(child);

		new_child = slice(new_child, 1, (int)new_child.size());
		bool first_result = check_square_helper(new_parent, new_child, coord, closest, flip);
		
		if (first_result) {
			return first_result; // if first_result is true, dont bother doing the second branch
		}

		new_parent.push_back(child[0]);
		return check_square_helper(new_parent, new_child, coord, closest, flip);


	}
}

bool Board::check_diagonal(const int& index, const std::vector<int>& offset) {
	return check_diagonal(coord_to_index(index), offset);
}

bool Board::check_diagonal(const std::vector<int>& coords, const std::vector<int>& offset) {
	char first_char = spaces[coord_to_index(coords)];

	if (first_char == ' ' || first_char == 0) {
		return false;
	}

	//set current_square to one step after coords
	std::vector <int> current_square(coords);
	step_square(current_square, offset);

	for (int i = 0; i < width - 1; i++) {
		//std::cout << coord_to_string(current_square) << "\n";
		if (spaces[coord_to_index(current_square)] != first_char) {
			return false;
		}

		step_square(current_square, offset);
	}

	return true;
}

// board modification functions

void Board::expand_board_dimensions(const std::vector <int>& add_dimensions) { // add a dimension to the end
	
	int new_size = int_pow(width, dimensions);

	for (int i = 0; i < new_size - size; i++) {
		
	}

	dimensions = dimensions + (int) add_dimensions.size();
}

void Board::expand_board_size(const std::vector <int>& add_positions) {
	
}

void Board::contract_board_dimensions(const std::vector <int>& remove_dimensions) {

}

void Board::contract_board_size(const std::vector <int>& remove_positions) {

}

void Board::set_square_at(const char& chr, const std::vector<int>& coord) {
	set_square_at(chr, coord_to_index(coord));
}

void Board::set_square_at(const char& chr, const int& index) {
	spaces[index] = chr;
}