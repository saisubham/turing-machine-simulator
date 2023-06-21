#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

typedef std::vector<std::vector<int>> vvi;
typedef std::vector<std::vector<char>> vvc;

class TuringMachine {
	std::string file_name,
				tape;		// input tape on which turing machine works
	int num_states,	// number of states
		num_alphs, 	// number of alphabets
		ptr,		// location of pointer head of turing machine
		present_state_id = 0,
		read_char_id = 0,
		dir_id = 0;
	std::map<std::string, int> state_id;		// maps each state to a unique key
	std::map<char, int> alph_id;				// maps each alphabet to a unique key
	std::vector<int> accept_state;
	std::vector<std::vector<int>> state_table;	// transition table storing next state
												// for a given (state, input char) pair
	std::vector<std::vector<int>> dir_table;	// transition table storing next direction
												// for a given (state, input char) pair
	std::vector<std::vector<char>> write_table;	// transition table storing next write char
												// for a given (state, input char) pair
	enum class TapeDisplayMode {
		SINGLE_LINE,
		DRAW
	} tape_display_mode = TapeDisplayMode::SINGLE_LINE;

	std::string state_id_to_name(const int &id);
	char char_id_to_symbol(const int &id);
public:
	TuringMachine(std::string _file_name):
		file_name(_file_name),
		num_states(0),
		num_alphs(0),
		ptr(0){};

	void displayTape();
	int parseFile();
	//void initializeTables();
	void makeTransitionTables();
	void turingSimulator();
};
