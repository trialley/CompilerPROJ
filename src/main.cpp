#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "vm.h"
// string source;
// // ;
// cout << "����������Ҫ�����Դ�ļ�·��:" << endl;
// cin >> source;

// parser a(source);

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

// vm t(target);

void tests() {
	/*
	std::filesystem::path str("../TestCodeV2");
	if (!std::filesystem::exists(str)) {
		throw std::exception("TestCodeV2 does not exist.");
	}
	std::filesystem::directory_entry entry(str);						 //�ļ����
	if (entry.status().type() == std::filesystem::file_type::directory)	 //��������C++11��ǿö������
		cout << "��·����һ��Ŀ¼" << endl;

	std::filesystem::directory_iterator list(str);	//�ļ��������
	for (auto& it : list) {							//�����ļ�����ÿһ����Ա
	}
	cout << it.path().filename() << endl;  //ͨ���ļ���ڣ�it����ȡpath�����ٵõ�path������ļ�������֮���
	*/
	std::vector<std::string> inFilePaths{
		"../TestCodeV2/PL0_code.in",
		"../TestCodeV2/PL0_code0.in",
		"../TestCodeV2/PL0_code1.in",
		"../TestCodeV2/PL0_code2.in",
		"../TestCodeV2/PL0_code3.in",
	};
	for (auto path : inFilePaths) {
		try {
			parser a(path);
			std::cout << endl;
			std::cout << "�ļ�" << path << "�ķ��ű�:" << endl;
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
	if (argc >= 0) {
		std::cout << "Your request will be processed:" << endl;
		run(argc, argv);
	} else {
		std::cout << "No arguments input, runn default tests:" << endl;
		tests();
	}

	return 0;
}