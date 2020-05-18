// #include <filesystem>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "parser.h"
#include "vm.h"

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
			std::cout << std::endl;
			std::cout << "file" << path << "table:" << std::endl;
			a.printTable();
		} catch (std::exception& ex) {
			std::cout << "file: " << path << "has some problems." << std::endl;
			std::cout << ex.what();
		}
	}
}
void run(int argc, const char* argv[]) {
	std::string path = "./TestCodeV2/PL0_code3.in";
	parser a(path);

	std::cout << std::endl;
	std::cout << "符号表中内容如下:" << std::endl;
	a.printTable();

	std::cout << std::endl;
	std::cout << "代码表中内容如下:" << std::endl;

	a.printCode();

	std::cout << "符号表(.sym)和目标代码(.pl0)都已输出在文件中." << std::endl;

	vm t(path + ".pl0cache");
}
int main(int argc, const char* argv[]) {
	if (argc > 0) {
		std::cout << "Your request will be processed:" << std::endl;
		run(argc, argv);
	} else {
		std::cout << "No arguments input, runn default tests:" << std::endl;
		tests();
	}

	return 0;
}