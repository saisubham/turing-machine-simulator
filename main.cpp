#include "turing_machine.h"

int main(int argc, char const *argv[]){
	if(argc == 1){
		std::cerr << "Too few arguments\n";
		return 1;
	}
	for (int i = 1; i < argc; ++i){
		std::cout << argv[i] << ":\n";
		TuringMachine t(argv[i]);
		if(t.parseFile()){ continue; }
		t.makeTransitionTables();
		t.turingSimulator();
		std::cout << "\n\n";
	}
	return 0;
}
