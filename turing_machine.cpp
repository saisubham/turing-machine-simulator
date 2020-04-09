#include "turing_machine.h"

void TuringMachine::displayTape() {
	std::cout << "\r" << tape.substr(0, ptr);	// display tape contents before the pointer head
	std::cout << "[" << tape[ptr] << "]";		// display tape content under the pointer head
	std::cout << tape.substr(ptr + 1);			// display tape contents after the pointer head
	std::cout << std::flush;
	usleep(200000);
}

int TuringMachine::parseFile() {

	// opening input file for reading
	std::ifstream ifs;
	ifs.open(file_name, std::ios::in);
	if(!ifs.is_open()) {
		std::cerr << "error: Can't open file\n";
		return 1;
	}

	std::string line, present_state, next_state;
	char read_char, write_char, direction;

	getline(ifs,tape);	// reading tape (input)
	while(getline(ifs, line)){
		std::istringstream iss(line);
		iss >> present_state;
		if(present_state == "accept"){
			while(iss >> present_state)
				accept_state.push_back(state_id.find(present_state)->second);
			continue;
		}
		iss >> read_char >> write_char >> direction >> next_state;
		if(state_id.insert(std::make_pair(present_state, num_states)).second)
			++num_states;
		if(state_id.insert(std::make_pair(next_state, num_states)).second)
			++num_states;
		if(alph_id.insert(std::make_pair(read_char, num_alphs)).second)
			++num_alphs;
		if(alph_id.insert(std::make_pair(write_char, num_alphs)).second)
			++num_alphs;
	}
	
	// initialize transition tables
	state_table = std::vector<std::vector<int>>(num_states, std::vector<int>(num_alphs, -1));
	write_table = std::vector<std::vector<char>>(num_states, std::vector<char>(num_alphs, '_'));
	dir_table = std::vector<std::vector<int>>(num_states, std::vector<int>(num_alphs, 0));

	ifs.close();
	return 0;
}

void TuringMachine::makeTransitionTables(){
	std::string line, present_state, next_state, tape;
	char read_char, write_char, direction;
	int present_state_id, next_state_id, read_char_id, dir_id;

	std::ifstream ifs;
	ifs.open(file_name, std::ios:: in);
	getline(ifs, line);
	while(getline(ifs, line)){
		std::istringstream iss(line);

		iss >> present_state;
		if(present_state == "accept")
			continue;

		iss >> read_char >> write_char >> direction >> next_state;

		// find respective unique id's
		present_state_id = state_id.find(present_state)->second;
		next_state_id = state_id.find(next_state)->second;
		read_char_id = alph_id.find(read_char)->second;
		if(direction == 'l')
			dir_id = -1;
		else if(direction == 'r')
			dir_id = +1;
		else
			dir_id = 0;

		// populate transition tables with unique id's
		state_table[present_state_id][read_char_id] = next_state_id;
		write_table[present_state_id][read_char_id] = write_char;
		dir_table[present_state_id][read_char_id] = dir_id;
	}
	ifs.close();
}

void TuringMachine::turingSimulator(){
	int present_state_id = 0, read_char_id, dir_id = 0;

	displayTape();
	while(true){
		read_char_id = alph_id.find(tape[ptr])->second;
		dir_id = dir_table[present_state_id][read_char_id];
		tape[ptr] = write_table[present_state_id][read_char_id];
		present_state_id = state_table[present_state_id][read_char_id];

		if(present_state_id == -1){
			if(accept_state.size() > 0)
				std::cout << "\n> rejected";
			else
				std::cout << "\n> halted";
			break;
		}
		if(find(accept_state.begin(), accept_state.end(), present_state_id)
			!= accept_state.end()){
			std::cout << "\n> accepted";
			break;
		}

		ptr += dir_id;
		if(ptr < 0){
			tape = "_" + tape;
			ptr = 0;
		}
		if(ptr >= tape.size())
			tape = tape + "_";
		displayTape();
	}
}