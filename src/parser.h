#pragma once

#include <string>

#include "defines.h"
#include "lexer.h"

class parser {
public:
	parser(const std::string&);

	void BLOCK();
	void analyzeConst();
	void analyzeVar();
	void analyzePro();
	void analyzeSent();

	void analyzeCond();
	void analyzeExpr();

	void analyzeTerm();
	void analyzeElem();

	bool insertSymbol(SymbolType kind, const std::string& id);
	int searchSymbol(const std::string& id);

	void pushCode(InsType fun, int lev, int offset);

	void printTable();
	void printCode();

	void generateFile(const std::string&);

	int PushError(int e, int eline);

	std::vector<CODE> codeTable;

private:
	lexer* wa;
	symEntry _tempSymEntry;

	std::vector<SYMBOL> symbolTable;

	int lev = -1;  //层次
	int dx;		   //每层局部量的相对地址，每说明完一个变量后dx+1

	int cx = 0;	 //代码段的大小
	int cx0;	 //保存当前生成代码的地址

	int mainEntry;	 //记录主程序入口
	int offset = 1;	 // 记录一开始空出的前两条指令的偏移

	std::string errors[100];  //存储的错误信息
	int etop = 0;			  //错误信息栈顶
};
