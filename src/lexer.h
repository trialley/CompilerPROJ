/*词法分析器程序*/

#pragma once

#include <defines.h>

class lexer {
public:
	lexer(const std::string&);
	void openSrc(const std::string&);
	bool readLine();
	bool isLetter(const char c);
	bool isDigit(const char c);
	void getChar();
	void getBC();
	void retract();

	int reserve(const std::string&);

	symEntry GETSYM();	//获得一个单词符号，是主要接口

	int row = 0;  //在文件的第几行
private:
	FILE* fp;				 //源文件的指针
	char buffer[BUFFERLEN];	 //读入缓冲区
	int index_pointer;		 //指针
	char temp_ch;			 //读入字符
};
