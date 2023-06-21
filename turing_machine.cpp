#include "turing_machine.h"

#define COUT_N(times, input){ \
	for(int i_ = 0; i_ < times; i_++){ \
		std::cout << input; \
	} \
}

#define COUT_CENTERED(N, input, len){ \
    const int padding = ((N - len) / 2); \
	COUT_N(padding, ' '); \
    std::cout << input; \
	COUT_N((len % 2 ? padding : padding+1), ' '); \
}

std::string TuringMachine::state_id_to_name(const int &id){
	for(const auto& i : state_id){
		if(i.second == id){
			return i.first;
		}
	}
	return "";
}

char TuringMachine::char_id_to_symbol(const int &id){
	for(const auto& i : alph_id){
		if(i.second == id){
			return i.first;
		}
	}
	return '?';
}

char dir_int_to_char(const int &d){
	switch(d){
		case -1:
			return '<';
		case 0:
			return '-';
		case 1:
			return '>';
	}
	return '?';
}

void TuringMachine::displayTape() {
	switch(tape_display_mode){
		case TapeDisplayMode::SINGLE_LINE:
			std::cout << "\r" << tape.substr(0, ptr);	// display tape contents before the pointer head
			std::cout << "[" << tape[ptr] << "]";		// display tape content under the pointer head
			std::cout << tape.substr(ptr + 1);			// display tape contents after the pointer head
			std::cout << std::flush;
			usleep(200000);
			break;
		case TapeDisplayMode::DRAW:
			// machine body
			constexpr int ASSUMED_MAX_STATE_NAME_WIDTH = 13;
			constexpr int MARGIN = 2;
			constexpr char MACHINE_NAME[] = "TM";
			constexpr int MACHINE_NAME_PADDING = (ASSUMED_MAX_STATE_NAME_WIDTH - (sizeof(MACHINE_NAME)-1) + 1) / 2;
			constexpr int PIN_PADDING = (ASSUMED_MAX_STATE_NAME_WIDTH+(MARGIN*2-(2*1)))/2;
			constexpr int PIN_POSITION = (ASSUMED_MAX_STATE_NAME_WIDTH+(MARGIN*2-(2*1)))/2 + MARGIN;
			const int ARROW_PADDING = 2+(4*ptr);
				// top
			std::cout << "┏"; 
			COUT_N(ASSUMED_MAX_STATE_NAME_WIDTH+(MARGIN*2-(2*1)), "━");
			std::cout << "┓" << std::endl; 
				// body
			std::cout << "┃ ";
			COUT_CENTERED(ASSUMED_MAX_STATE_NAME_WIDTH, MACHINE_NAME, (sizeof(MACHINE_NAME)-1));
			std::cout << " ┃" << std::endl;
				// state
			std::cout << "┃ ";
			std::string state_name = state_id_to_name(present_state_id);
			COUT_CENTERED(ASSUMED_MAX_STATE_NAME_WIDTH, state_name, state_name.size());
			std::cout << " ┃" << std::endl;
				// bottom
			std::cout << "┗"; 
			COUT_N(PIN_PADDING, "━");
			std::cout << "┰"; 
			COUT_N(PIN_PADDING, "━");
			std::cout << "┛" << std::endl; 
			// arrow
			COUT_N(PIN_POSITION - 1, ' ');
			std::cout << "│" << std::endl;
			if(ARROW_PADDING < PIN_POSITION){
				COUT_N(ARROW_PADDING, ' ');
				std::cout << "┌";
				COUT_N(PIN_POSITION-ARROW_PADDING-2, "─");
				std::cout << "┘" << std::endl;
			}else if(ARROW_PADDING > PIN_POSITION){
				COUT_N(PIN_POSITION-1, ' ');
				std::cout << "└";
				COUT_N(ARROW_PADDING-PIN_POSITION, "─");
				std::cout << "┐" << std::endl;
			}else{
				COUT_N(ARROW_PADDING, ' ');
				std::cout << "│" << std::endl;
			}
			COUT_N(ARROW_PADDING, ' ');
			std::cout << "│" << std::endl;
			COUT_N(ARROW_PADDING, ' ');
			std::cout << "V" << std::endl;
			// tape
			std::cout << '+';
			COUT_N(tape.size(), "---+");
			std::cout << std::endl;
			std::cout << '|';
			COUT_N(tape.size(), ' ' << tape[i_] << " |");
			std::cout << std::endl;
			std::cout << '+';
			COUT_N(tape.size(), "---+");
			std::cout << std::endl;
			// rules
			for(const auto &i : state_id){
				state_name = i.first;
				for(int h = 0; h < state_table[i.second].size(); h++){
					if(state_id_to_name(state_table[i.second][h]) == ""){ continue; }
					std::cout << "δ("
										<< state_name << ", "
										<< char_id_to_symbol(h)
									<< ") := ("
										<< state_id_to_name(state_table[i.second][h]) << ", "
										<< write_table[i.second][h] << ", "
										<< dir_int_to_char(dir_table[i.second][h])
									<< ")";
					if(present_state_id == i.second && tape[ptr] == char_id_to_symbol(h)){
						std::cout << "\t<";
					}
					std::cout << std::endl << std::endl;
				}
			}
			// separator
			COUT_N(5, std::endl);
			std::cout << std::flush;
			break;
	}
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
	int current_state_id, next_state_id, read_char_id, dir_id;

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
		current_state_id = state_id.find(present_state)->second;
		next_state_id = state_id.find(next_state)->second;
		read_char_id = alph_id.find(read_char)->second;
		if(direction == 'l')
			dir_id = -1;
		else if(direction == 'r')
			dir_id = +1;
		else
			dir_id = 0;

		// populate transition tables with unique id's
		state_table[current_state_id][read_char_id] = next_state_id;
		write_table[current_state_id][read_char_id] = write_char;
		dir_table[current_state_id][read_char_id] = dir_id;
	}
	ifs.close();
}

void TuringMachine::turingSimulator(){
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
