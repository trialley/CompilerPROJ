// #include <filesystem>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "parser.h"
// vm t(target);
void currentdir() {
	char szBuf[128];
	char szPath[128];

	memset(szBuf, 0x00, sizeof(szBuf));
	memset(szPath, 0x00, sizeof(szPath));

	getcwd(szBuf, sizeof(szBuf) - 1);
	printf("buf:%s\n", szBuf);

	int ret = readlink("/proc/self/exe", szPath, sizeof(szPath) - 1);
	printf("ret:%d\n", ret);
	printf("path:%s\n", szPath);
	int* lost = new int;
	return;
}
void tests() {
	currentdir();
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