#include <iostream>

#include "Iterpreter.h"

// string source;
// // ;
// cout << "����������Ҫ�����Դ�ļ�·��:" << endl;
// cin >> source;

// SyntaxAnal a(source);

// cout << endl;
// cout << "���ű�����������:" << endl;
// a.printTable();

// cout << endl;
// cout << "���������������:" << endl;

// a.printCode();

// cout << "���ű�(.sym)��Ŀ�����(.pl0)����������ļ���." << endl;

// char target[99];
// cout << "����������Ҫִ�е�Ŀ������ļ�·����" << endl;
// cin >> target;

// Iterpreter t(target);

void tests() {
	std::vector<std::string> inFilePaths;
	for (auto path : inFilePaths) {
		SyntaxAnal a(source);
	}
}
void run(int argc, const char* argv[]) {
	//TODO
}
int main(int argc, const char* argv[]) {
	if (argc >= 0) {
		std::cout << "Your request will be processed:" << endl;
		run(argc, argv);
	} else {
		std::cout << "No arguments input, runn default tests:" << endl;
		tests();
	}

	return 0;
}