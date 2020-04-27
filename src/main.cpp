#include <iostream>
#include <string>
#include <vector>

#include "parser.h"

void tests() {
	std::vector<std::string>
		inFilePaths{
			"./TestCodeV2/PL0_code.in",
			"./TestCodeV2/PL0_code0.in",
			"./TestCodeV2/PL0_code1.in",
			"./TestCodeV2/PL0_code2.in",
			"./TestCodeV2/PL0_code3.in",
		};
	for (auto path : inFilePaths) {
		try {
			parser a(path);
			std::cout << endl;
			std::cout << "file" << path << "table:" << endl;
			a.printTable();
		} catch (std::exception& ex) {
			std::cout << "file: " << path << "has some problems." << std::endl;
			std::cout << ex.what();
		}
	}
}
void run(int argc, const char* argv[]) {
	//TODO
}
int main(int argc, const char* argv[]) {
	if (argc > 1) {
		std::cout << "Your request will be processed:" << endl;
		run(argc, argv);
	} else {
		std::cout << "No arguments input, runn default tests:" << endl;
		tests();
	}

	return 0;
}