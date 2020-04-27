#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "vm.h"
// string source;
// // ;
// cout << "请输入你想要编译的源文件路径:" << endl;
// cin >> source;

// parser a(source);

// cout << endl;
// cout << "符号表中内容如下:" << endl;
// a.printTable();

// cout << endl;
// cout << "代码表中内容如下:" << endl;

// a.printCode();

// cout << "符号表(.sym)和目标代码(.pl0)都已输出在文件中." << endl;

// char target[99];
// cout << "请输入你想要执行的目标代码文件路径：" << endl;
// cin >> target;

// vm t(target);

void tests() {
	/*
	std::filesystem::path str("../TestCodeV2");
	if (!std::filesystem::exists(str)) {
		throw std::exception("TestCodeV2 does not exist.");
	}
	std::filesystem::directory_entry entry(str);						 //文件入口
	if (entry.status().type() == std::filesystem::file_type::directory)	 //这里用了C++11的强枚举类型
		cout << "该路径是一个目录" << endl;

	std::filesystem::directory_iterator list(str);	//文件入口容器
	for (auto& it : list) {							//对于文件夹中每一个成员
	}
	cout << it.path().filename() << endl;  //通过文件入口（it）获取path对象，再得到path对象的文件名，将之输出
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
			std::cout << "文件" << path << "的符号表:" << endl;
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