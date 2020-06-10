#pragma once

#include <defines.h>
#include <graph.h>
#include <lexer.h>

#include <string>

class parser {
public:
	parser(const std::string&);
	neb::CJsonObject BLOCK();

	neb::CJsonObject analyzeConst();
	neb::CJsonObject analyzeVariable();
	neb::CJsonObject analyzeProcedure();
	neb::CJsonObject analyzeSentence();
	neb::CJsonObject analyzeCondition();
	neb::CJsonObject analyzeExpression();
	neb::CJsonObject analyzeTerm();
	neb::CJsonObject analyzeElement();

	bool insertSymbol(SymbolType kind, const std::string& id);
	int searchSymbol(const std::string& id);

	void pushCode(InsType fun, int lev, int offset);

	void printTable();
	void printCode();

	void generateFile(const std::string&);

	int error(int e, int eline);

private:
	std::vector<CODE> codeTable;  //代码表
	graph _g;					  //图形化模块
	lexer _lexer;				  //词法分析模块

	std::vector<SYMBOL> symbolTable;

	int lev = -1;  //层次
	int dx;		   //每层局部量的相对地址，每说明完一个变量后dx+1

	int mainEntry;	 //记录主程序入口
	int offset = 1;	 // 记录一开始空出的前两条指令的偏移

	int cx = 0;	 //代码段的大小
	int cx0;	 //保存当前生成代码的地址

	symEntry _tempSymEntry;	  //临时变量
	std::string errors[100];  //存储的错误信息
	int etop = 0;			  //错误信息栈顶
};
