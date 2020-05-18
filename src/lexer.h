/*词法分析器程序*/

#pragma once

#include "defines.h"

class lexer {
public:
	lexer(const std::string&);

	void initMap();	 //初始化每个符号对应的字符串名称，便于打印调试

	void openSrc(const std::string&);  //打开源文件
	bool readLine();				   //读取一行

	void GetChar();	 //获得一个符号
	void GetBC();	 //跳过空格
	void Retract();

	bool IsLetter(const char c);
	bool IsDigit(const char c);

	int Reserve(const std::string&);

	symEntry getSym();	//读取源文件并返回单词符号

	int row = 0;  //在文件的第几行

private:
	FILE* fp;  //源文件的指针

	char buffer[BUFFERLEN];	 //读入缓冲区
	int index_pointer;		 //搜索指示器
	char ch;				 //读入字符
};
