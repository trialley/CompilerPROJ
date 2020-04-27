/*语法分析程序*/
#ifndef SYNTAXANAL_H
#define SYNTAXANAL_H

#include <string>

#include "defines.h"
#include "lexer.h"
class SyntaxAnal {
   public:
	SyntaxAnal(const std::string&);

	void BLOCK();		  //语法分析
	void analyzeConst();  //常量说明部分
	void analyzeVar();	  //变量说明部分
	void analyzePro();	  //过程说明部分
	void analyzeSent();	  //语句

	void analyzeCond();	 //处理条件语句
	void analyzeExpr();	 //处理表达式

	void analyzeTerm();	 //处理项
	void analyzeElem();	 //分析因子

	bool insertSymbol(SymbolKind kind, const char* id);	 //向符号表插入符号
	int searchSymbol(const char* id);					 //在符号表中查找符号，没找到返回-1

	void GEN(FunctionCode fun, int lev, int offset);  //产生中间代码

	void printTable();
	void printCode();

	void generateFile(const char* filename);  //生成目标文件

	int error(int e, int eline);  //错误处理

	vector<CODE> codeTable;	 //代码表

   private:
	lexer* wa;	//词法分析器
	tuple3 t;	//词法分析器取到的单词符号

	vector<SYMBOL> symbolTable;	 //符号表

	int lev = -1;  //层次
	int dx;		   //每层局部量的相对地址，每说明完一个变量后dx+1

	int cx = 0;	 //代码段的大小
	int cx0;	 //保存当前生成代码的地址

	int mainEntry;	 //记录主程序入口
	int offset = 1;	 // 记录一开始空出的前两条指令的偏移

	string errors[100];	 //存储的错误信息
	int etop = 0;		 //错误信息栈顶
};

#endif