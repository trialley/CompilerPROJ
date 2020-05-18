#include "parser.h"

#include <exception>
#include <iomanip>
#include <iostream>
parser::parser(const std::string& filename) {
	wa = new lexer(filename);  //创建新的词法分析器
	wa->readLine();			   //读取一行

	_tempSymEntry = wa->getSym();  //获得一个符号到temp
	if (_tempSymEntry.sym == INVALID) {
		LOG << "非法标识符";
		throw std::exception(std::logic_error("非法标识符"));
		return;
	}

	BLOCK(); /*语法分析程序*/

	std::vector<CODE>::iterator iter = codeTable.begin();

	codeTable.insert(iter, CODE(JMP, 0, mainEntry));  //在中间代码表首加入

	generateFile(filename);	 //生成二进制文件

	if (!etop) {  //没有错误
		std::cout << std::endl;
		std::cout << "compile success" << std::endl;
	} else {  //输出编译错误

		std::cout << std::endl;
		for (int i = 0; i < etop; i++) {
			std::cout << errors[i] << std::endl;
		}
		std::cout << "compile failed" << std::endl;
		exit(-1);
	}
}

void parser::BLOCK() {
	lev++;	//层次，代码调用
	dx = 3;

	if (_tempSymEntry.sym == _CONST) {	//是CONST声明
		_tempSymEntry = wa->getSym();	//再获取一个符号
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeConst();	 //分析一个const类型的变量符号

		while (_tempSymEntry.sym == _COMMA)	 //逗号，多个声明。
		{
			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			analyzeConst();
		}

		if (_tempSymEntry.sym == _SEMICOLON) {	//遇到分号
			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;
		} else {
			/*既不是逗号也不是分号，声明语句没有以;结束*/
			error(10, wa->row);
		}
	}

	if (_tempSymEntry.sym == _VAR) {  //是VAR声明

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeVar();

		while (_tempSymEntry.sym == _COMMA)	 //逗号
		{
			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			analyzeVar();
		}

		if (_tempSymEntry.sym == _SEMICOLON) {	//遇到了分号

			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;
		}

		else {
			/*声明语句没有以;结束*/
			error(13, wa->row);
		}
	}

	if (_tempSymEntry.sym == _PROCEDURE) {	//是过程说明

		// cout << "过程说明部分！" << std::endl;

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzePro();
	}
	//}
	/*说明部分结束*/
	if (lev == 0)  //主程序层.ppp
		mainEntry = cx + offset;

	GEN(INT, 0, dx);  // 生成空间分配指令，分配dx个空间（3个空间+变量的数目）

	analyzeSent();	//处理遇到的语句

	GEN(OPR, 0, 0);	 //退出这个过程
}

void parser::GEN(InsType fun, int lev, int offset) {
	codeTable.push_back(CODE(fun, lev, offset));
	cx++;
}

void parser::analyzeConst() {  //常量说明部分
	LOG << "进入const分析部分\n";
	if (_tempSymEntry.sym == _IDENT) {	//获取标识符

		std::string id = _tempSymEntry.name;

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		if (_tempSymEntry.sym == _ASSIGN)  //是赋值号
		{
			// 如果不是等号，而是赋值符号:=，抛出1号错误
			error(19, wa->row);
		} else if (_tempSymEntry.sym == _EQ) {
			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			if (_tempSymEntry.sym == _NUMBER) {	 //是数字

				if (!insertSymbol(SymbolType::CONST, id))
					error(109, wa->row);
				/*重定义*/;	 //插入到符号表失败

				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;
			}

			else {	//赋值号后不是数字

				error(18, wa->row);
			}
		}

		else {	//标识符后不是等号

			error(19, wa->row);
		}
	} else {  //const 后没有加标识符

		error(11, wa->row);
	}
}

void parser::analyzeVar() {	 //变量说明部分

	LOG << "分析变量声明部分" << std::endl;

	if (_tempSymEntry.sym == _IDENT) {
		// std::string id;
		// strcpy(id, _tempSymEntry.name);

		if (!insertSymbol(SymbolType::VAR, _tempSymEntry.name)) {
			LOG << "重定义" << std::endl;
			error(109, wa->row);
		}

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;	 //取下一个词
	} else {
		// 如果变量声明过程中遇到的第一个字符不是标识符
		error(12, wa->row);
	}
}

void parser::analyzePro() {	 //过程说明部分

	if (_tempSymEntry.sym == _IDENT) {
		if (!insertSymbol(SymbolType::PROD, _tempSymEntry.name))
			error(109, wa->row);
		/*重定义*/

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;
	} else {
		/*过程首部没有标识符*/
		error(20, wa->row);
	}

	if (_tempSymEntry.sym == _SEMICOLON) {
		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;
	} else {  //过程名没有以分号结束
		error(17, wa->row);
	}

	//保存当前dx和lev
	int tempdx = dx;
	int templev = lev;

	BLOCK();  //分程序，内部过程说明表的构造

	//恢复
	dx = tempdx;
	lev = templev;

	if (_tempSymEntry.sym == _SEMICOLON) {	//end;后的符号。这里有问题

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		while (_tempSymEntry.sym == _PROCEDURE) {  //如果还有并列的过程说明

			_tempSymEntry = wa->getSym();
			analyzePro();
		}
	} else {
		/*过程说明end后没有加分号*/
		error(22, wa->row);
	}
}

void parser::analyzeSent() {
	int i;
	int cx1;
	int cx2;

	switch (_tempSymEntry.sym) {
	case _IDENT: { /*赋值语句*/

		i = searchSymbol(_tempSymEntry.name);
		if (i < 0) { /*没有找到符号的声明*/
			error(108, wa->row);
		}

		else if (symbolTable.at(i).kind != SymbolType::VAR) {
			// 如果在符号表中找到了，但是该标识符不是变量名，
			/*不可修改的左值*/
			error(110, wa->row);
		}

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		if (_tempSymEntry.sym == _ASSIGN) {	 //是赋值号

			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;
		} else {
			/*变量名后没有加赋值号*/
			error(14, wa->row);
		}

		analyzeExpr();	//处理赋值号右部的表达式

		if (i >= 0) {
			GEN(STO, lev - symbolTable.at(i).lev, symbolTable.at(i).addr);
		}

		break;
	}

	case _CALL: {
		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		if (_tempSymEntry.sym != _IDENT) {	//call 后面不是标识符
			error(34, wa->row);
		} else {
			i = searchSymbol(_tempSymEntry.name);  //查找标识符对应的符号表

			if (i < 0) {
				error(108, wa->row);
			}

			else if (symbolTable.at(i).kind == SymbolType::PROD) {	//类型检查

				GEN(CAL, lev - symbolTable.at(i).lev, symbolTable.at(i).addr);
			}

			else {	//call 后加的不是proc

				/*调用的标示符不是过程名*/
				error(34, wa->row);
			}

			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;
		}

		break;
	}

	case _IF: {
		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeCond();	//处理if后面的条件
		cx1 = cx;		//jpc语句占用的位置
		cx++;

		if (_tempSymEntry.sym == _THEN) {
			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			analyzeSent();														   //then后的语句
			codeTable.insert(codeTable.begin() + cx1, CODE(JPC, 0, cx + offset));  //回填then语句之后的代码地址

		} else {
			/*条件语句缺少then*/
			error(36, wa->row);
		}

		//cx = codeTable.size();

		//codeTable.push_back(CODE(JPC, 0, 0));
		//codeTable.at(cx1).offset = codeTable.size();
		break;
	}

	case _BEGIN: {
		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeSent();

		while (_tempSymEntry.sym != INVALID) {	//这里写的有点乱

			if (_tempSymEntry.sym == _SEMICOLON) {	//t为";",说明后面还有语句

				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;

				analyzeSent();
			} else if (_tempSymEntry.sym == _END) {	 //t为end，表示复合语句结束

				_tempSymEntry = wa->getSym();
				//if (t.sym == INVALID) return;
				return;
			}

			else {
				//出现其他字符，则说明复合语句错误
				error(23, wa->row);
				return;
			}
			//analyzeSent();
		}

		/*复合语句出错*/
		break;
	}

	case _WHILE: {
		cx1 = cx;  // 记录当前代码分配位置，这是while循环的开始位置，也就是无条件跳转到的地方

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeCond();	//判断while后面的条件语句

		cx2 = cx;  //jpc语句插入的地方
		cx++;	   //占一个Code位置

		if (_tempSymEntry.sym == _DO) {
			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			analyzeSent();

			GEN(JMP, 0, cx1 + offset);	//跳转到cx1处，即再次进行判断是否进行循环
			codeTable.insert(codeTable.begin() + cx2, CODE(JPC, 0, cx + offset));
		} else {
			/*while后面没有接do*/
			error(39, wa->row);
		}

		break;
	}

	case _READ: {
		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		if (_tempSymEntry.sym == _LPAIR) {
			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			if (_tempSymEntry.sym == _IDENT) {
				i = searchSymbol(_tempSymEntry.name);
				if (i < 0) {
					/*未说明的标识符*/
					error(108, wa->row);
				}

				else {
					//codeTable.push_back(CODE(OPR, 0, OPR::READ));// 生成16号读指令，从键盘读取数字
					//codeTable.push_back(CODE(STO, lev - symbolTable.at(i).lev, symbolTable.at(i).addr));// 生成sto指令，存入指定变量
					GEN(OPR, 0, OPR::READ);
					GEN(STO, lev - symbolTable.at(i).lev, symbolTable.at(i).addr);
				}
			}

			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			while (_tempSymEntry.sym == _COMMA) {  //读多个键盘输入

				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;

				if (_tempSymEntry.sym == _IDENT) {
					i = searchSymbol(_tempSymEntry.name);
					if (i < 0) {
						/*没找到标识符*/
						error(108, wa->row);
					} else {
						//	codeTable.push_back(CODE(OPR, 0, OPR::READ));//读键盘
						//codeTable.push_back(CODE(STO, lev - symbolTable.at(i).lev, symbolTable.at(i).addr));
						GEN(OPR, 0, OPR::READ);
						GEN(STO, lev - symbolTable.at(i).lev, symbolTable.at(i).addr);
					}
				}

				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;
			}

			if (_tempSymEntry.sym != _RPAIR) {
				/*左右括号不匹配*/
				error(27, wa->row);
			}

		} else {
			/*不是左括号*/
			error(29, wa->row);
		}

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		break;
	}

	case _WRITE: {
		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		if (_tempSymEntry.sym == _LPAIR) {
			_tempSymEntry = wa->getSym();
			analyzeExpr();
			GEN(OPR, 0, OPR::WRITE);

			while (_tempSymEntry.sym == _COMMA) {
				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;

				analyzeExpr();

				GEN(OPR, 0, OPR::WRITE);
			}

			if (_tempSymEntry.sym != _RPAIR) {
				/*左右括号不匹配*/
				error(31, wa->row);
			} else {
				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;
			}
		}

		break;
	}

	default:  //不是begin开头
		//error(8, wa->row);
		break;
	}
}

void parser::analyzeCond() {
	int relop;
	if (_tempSymEntry.sym == _ODD) {  //一元运算符

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeExpr();	//分析表达式

		//codeTable.push_back(CODE(OPR, 0, OPR::ODD));//生成奇偶判断指令
		GEN(OPR, 0, OPR::ODD);
	}

	else {	//二元运算符

		analyzeExpr();				//分析左边表达式
		relop = _tempSymEntry.sym;	//保存二元运算符

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeExpr();	//分析右边表达式

		switch (relop) {
		case _EQ:
			//codeTable.push_back(CODE(OPR, 0, OPR::EQ));
			GEN(OPR, 0, OPR::EQ);
			break;

		case _UEQ:
			//codeTable.push_back(CODE(OPR, 0, OPR::UE));
			GEN(OPR, 0, OPR::UE);
			break;

		case _LOWER:
			//codeTable.push_back(CODE(OPR, 0, OPR::LT));
			GEN(OPR, 0, OPR::LT);
			break;

		case _BIGGER_EQ:
			//codeTable.push_back(CODE(OPR, 0,OPR::GE));
			GEN(OPR, 0, OPR::GE);
			break;

		case _BIGGER:
			//codeTable.push_back(CODE(OPR, 0, OPR::GT));
			GEN(OPR, 0, OPR::GT);
			break;

		case _LOWER_EQ:
			//codeTable.push_back(CODE(OPR, 0, OPR::LE));
			GEN(OPR, 0, OPR::LE);
			break;

		default:
			/*保存的不是逻辑运算符*/
			break;
		}
	}
}

void parser::analyzeExpr() {
	int addop;
	if (_tempSymEntry.sym == _PLUS || _tempSymEntry.sym == _MINUS) {  //表达式以正负开头

		addop = _tempSymEntry.sym;

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeTerm();	//进行项的处理

		if (addop == _MINUS) {
			//codeTable.push_back(CODE(OPR, 0, OPR::MINUS));//取反运算
			GEN(OPR, 0, OPR::MINUS);
		}
	} else {
		analyzeTerm();	//表达式项的开头，直接项的分析
	}

	while (_tempSymEntry.sym == _PLUS || _tempSymEntry.sym == _MINUS) {
		addop = _tempSymEntry.sym;	//算术运算符保存在addop中

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeTerm();

		if (addop == _PLUS) {  // 项分析完毕后，如果刚才保存的是加号，则生成加法指令

			//codeTable.push_back(CODE(OPR, 0, OPR::ADD));
			GEN(OPR, 0, OPR::ADD);	//加法指令
		} else {
			//codeTable.push_back(CODE(OPR, 0, OPR::SUB));//减法指令
			GEN(OPR, 0, OPR::SUB);
		}
	}
}

void parser::analyzeTerm() {
	int mulop;

	analyzeElem();

	while (_tempSymEntry.sym == _STAR || _tempSymEntry.sym == _DIV) {
		mulop = _tempSymEntry.sym;	/// 把运算符保存在mulop中(乘法或是除法)

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeElem();	// 运算符后应该是一个因子，进行因子分析

		if (mulop == _STAR) {
			//codeTable.push_back(CODE(OPR, 0, OPR::MUL));//4号乘法指令
			GEN(OPR, 0, OPR::MUL);
		} else {
			//codeTable.push_back(CODE(OPR, 0, OPR::DIV));//除法指令
			GEN(OPR, 0, OPR::DIV);
		}
	}
}

void parser::analyzeElem() {
	int i;
	while (_tempSymEntry.sym == _IDENT || _tempSymEntry.sym == _NUMBER || _tempSymEntry.sym == _LPAIR) {
		switch (_tempSymEntry.sym) {
		case _IDENT: {
			i = searchSymbol(_tempSymEntry.name);
			if (i < 0) {
				/*没有找到符号*/
				error(108, wa->row);
			}

			else if (symbolTable.at(i).kind == SymbolType::CONST) {
				// 如果该标识符为常量,则生成lit指令,把val放到栈顶
				//codeTable.push_back(CODE(LIT, 0, symbolTable.at(i).val));
				GEN(LIT, 0, symbolTable.at(i).val);

				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;
			}

			else if (symbolTable.at(i).kind == SymbolType::VAR) {
				//codeTable.push_back(CODE(LOD, lev - symbolTable.at(i).lev,
				//symbolTable.at(i).addr));
				GEN(LOD, lev - symbolTable.at(i).lev,
					symbolTable.at(i).addr);

				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;
			} else if (symbolTable.at(i).kind == SymbolType::PROD) {
				/*该标识符为过程名，出错*/
				error(43, wa->row);
				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;
			}

			break;
		}

		case _NUMBER:  //因子分析遇到数字
		{
			int num = atoi(_tempSymEntry.name.c_str()); /*要判断范围*/

			//codeTable.push_back(CODE(LIT, 0, num));// 生成lit指令，把这个数值字面常量放到栈顶
			GEN(LIT, 0, num);

			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			break;
		}

		case _LPAIR: {
			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			analyzeExpr();

			if (_tempSymEntry.sym == _RPAIR) {
				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;
			} else {
				/*左右括号不匹配*/
				error(41, wa->row);
			}

			break;
		}
		}
	}
}

bool parser::insertSymbol(SymbolType kind, const std::string& id) {
	if (searchSymbol(id) >= 0)	//已经找到符号的声明
		return false;

	switch (kind) {
	case SymbolType::CONST: {
		int num = atoi(_tempSymEntry.name.c_str());	 //越界检查
		symbolTable.push_back(SYMBOL(id, kind, num, 0, 0));
		break;
	}

	case SymbolType::VAR: {
		symbolTable.push_back(SYMBOL(id, kind, 0xFFFFFFFF, lev, dx++));	 //变量默认值是-1
		break;
	}

	case SymbolType::PROD: {
		symbolTable.push_back(SYMBOL(id, kind, 0, lev, cx + offset));
		break;
	}
	default:
		break;
	}

	return true;
}

int parser::searchSymbol(const std::string& id) {
	for (int i = 0; i < symbolTable.size(); i++) {
		SYMBOL s = symbolTable.at(i);
		if ((id == s.name) && lev >= s.lev)	 //s在当前层及以上且名字相同
			return i;
	}

	return -1;
}

int parser::error(int e, int eline) {
	std::ostringstream oss;

	switch (e) {
	case 0: {
		oss << e << ": " << eline << "行有非法字符";
		errors[etop++] = oss.str();
		break;
	}
	case 1: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "行有超过14个字母的单词";
		oss << e << ": " << eline << "行有超过10个字母的单词";
		errors[etop++] = oss.str();
		break;
	}
	case 2: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "行有数字开头的标识符";
		oss << e << ": " << eline << "行有数字开头的标识符";
		errors[etop++] = oss.str();
		break;
	}
	case 3: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "行有超过10个字母的标识符";
		oss << e << ": " << eline << "行有超过10个字母的标识符";
		errors[etop++] = oss.str();
		break;
	}
	case 4: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "行程序结尾没有.";
		oss << e << ": " << eline << "行程序结尾没有.";
		errors[etop++] = oss.str();
		break;
	}
	case 5: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "行程序首部标识符后面没有;";
		oss << e << ": " << eline << "行程序首部标识符后面没有;";
		errors[etop++] = oss.str();
		break;
	}
	case 6: {  //
		//errors[etop++] = itos(e) + ": " + itos(eline) + "行程序首部PRAGRAM后面没有标识符";
		oss << e << ": " << eline << "行程序首部PROCEDURE后面没有标识符";
		errors[etop++] = oss.str();
		break;
	}
	case 7: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "行没有程序首部";
		oss << e << ": " << eline << "行没有程序首部";
		errors[etop++] = oss.str();
		break;
	}
	case 8: {
		oss << e << ": " << eline << "行分程序后缺少语句部分";
		errors[etop++] = oss.str();
		break;
	}
	case 9: {
		oss << e << ": " << eline << "行常量说明的,字符后面有错误";
		errors[etop++] = oss.str();
		break;
	}
	case 10: {
		oss << e << ": " << eline << "行常量说明后面没有;结尾";
		errors[etop++] = oss.str();
		break;
	}
	case 11: {
		oss << e << ": " << eline << "行常量定义有错误";
		errors[etop++] = oss.str();
		break;
	}
	case 12: {
		oss << e << ": " << eline << "行变量说明,后面缺少标识符";
		errors[etop++] = oss.str();
		break;
	}
	case 13: {
		oss << e << ": " << eline << "行变量说明没有;结尾";
		errors[etop++] = oss.str();
		break;
	}
	case 14: {
		oss << e << ": " << eline << "行VAR后面没有定义的标识符";
		errors[etop++] = oss.str();
		break;
	}

	case 15: {
		oss << e << ": " << eline << "行分程序后面没有;结尾";
		errors[etop++] = oss.str();
		break;
	}
	case 16: {
		oss << e << ": " << eline << "行过程说明后面没有分程序";
		errors[etop++] = oss.str();
		break;
	}
	case 17: {
		oss << e << ": " << eline << "行过程首部后面没有;结尾";
		errors[etop++] = oss.str();
		break;
	}
	case 18: {
		oss << e << ": " << eline << "行常量定义没有指定数字";
		errors[etop++] = oss.str();
		break;
	}
	case 19: {
		oss << e << ": " << eline << "行常量定义没有=字符";
		errors[etop++] = oss.str();
		break;
	}
	case 20: {
		oss << e << ": " << eline << "行过程首部PROCEDURE后面并没有标识符";
		errors[etop++] = oss.str();
		break;
	}
	case 21: {
		oss << e << ": " << eline << "行语句后面没有;结尾";
		errors[etop++] = oss.str();
		break;
	}
	case 22: {
		oss << e << ": " << eline << "行没有END结尾，或者此行语句缺少;结尾";
		errors[etop++] = oss.str();
		break;
	}
	case 23: {
		oss << e << ": " << eline << "行复合语句的BEGIN后面不是语句";
		errors[etop++] = oss.str();
		break;
	}
	case 24: {
		oss << e << ": " << eline << "行赋值语句后面不是表达式";
		errors[etop++] = oss.str();
		break;
	}
	case 25: {
		oss << e << ": " << eline << "行赋值语句没有:=赋值号";
		errors[etop++] = oss.str();
		break;
	}
	case 26: {
		oss << e << ": " << eline << "行READ语句参数中有非法,字符";
		errors[etop++] = oss.str();
		break;
	}
	case 27: {
		oss << e << ": " << eline << "行READ语句缺少)字符";
		errors[etop++] = oss.str();
		break;
	}
	case 28: {
		oss << e << ": " << eline << "行READ语句缺少标识符";
		errors[etop++] = oss.str();
		break;
	}
	case 29: {
		oss << e << ": " << eline << "行READ语句缺少(字符";
		errors[etop++] = oss.str();
		break;
	}
	case 30: {
		oss << e << ": " << eline << "行WRITE语句参数中有非法,字符";
		errors[etop++] = oss.str();
		break;
	}
	case 31: {
		oss << e << ": " << eline << "行WRITE语句缺少)字符";
		errors[etop++] = oss.str();
		break;
	}
	case 32: {
		oss << e << ": " << eline << "行WRITE语句缺少标识符";
		errors[etop++] = oss.str();
		break;
	}
	case 33: {
		oss << e << ": " << eline << "行WRITE语句缺少(字符";
		errors[etop++] = oss.str();
		break;
	}
	case 34: {
		oss << e << ": " << eline << "行CALL语句没有过程名字";
		errors[etop++] = oss.str();
		break;
	}
	case 35: {
		oss << e << ": " << eline << "行IF语句的THEN后面不是合法语句";
		errors[etop++] = oss.str();
		break;
	}
	case 36: {
		oss << e << ": " << eline << "行IF语句的条件后面没有THEN";
		errors[etop++] = oss.str();
		break;
	}
	case 37: {
		oss << e << ": " << eline << "行IF语句后面不是合法条件";
		errors[etop++] = oss.str();
		break;
	}
	case 38: {
		oss << e << ": " << eline << "行WHILE语句的DO后面不是合法语句";
		errors[etop++] = oss.str();
		break;
	}
	case 39: {
		oss << e << ": " << eline << "行WHILE语句没有DO";
		errors[etop++] = oss.str();
		break;
	}
	case 40: {
		oss << e << ": " << eline << "行WHILE语句后面不是合法的条件";
		errors[etop++] = oss.str();
		break;
	}
	case 41: {
		oss << e << ": " << eline << "行表达式的右边缺少)字符";
		errors[etop++] = oss.str();
		break;
	}
	case 42: {
		oss << e << ": " << eline << "行有非法表达式";
		errors[etop++] = oss.str();
		break;
	}
	case 43: {
		oss << e << ": " << eline << "行有非法因子";
		errors[etop++] = oss.str();
		break;
	}
	case 44: {
		oss << e << ": " << eline << "行有非法项";
		errors[etop++] = oss.str();
		break;
	}
	case 45: {
		oss << e << ": " << eline << "行有非法项";
		errors[etop++] = oss.str();
		break;
	}
	case 46: {
		oss << e << ": " << eline << "行有非法项";
		errors[etop++] = oss.str();
		break;
	}
	case 47: {
		oss << e << ": " << eline << "行有非法项";
		errors[etop++] = oss.str();
		break;
	}
	case 48: {
		oss << e << ": " << eline << "行关系运算符有非法表达式";
		errors[etop++] = oss.str();
		break;
	}
	case 49: {
		oss << e << ": " << eline << "行ODD有非法表达式";
		errors[etop++] = oss.str();
		break;
	}
	case 101: {
		oss << e << ": " << eline << "行给非变量赋值或变量没有定义";
		errors[etop++] = oss.str();
		break;
	}
	case 102: {
		oss << e << ": " << eline << "行read语句读入非变量或变量没有定义";
		errors[etop++] = oss.str();
		break;
	}
	case 103: {
		oss << e << ": " << eline << "行read语句读入非变量或变量没有定义";
		errors[etop++] = oss.str();
		break;
	}
	case 106: {
		oss << e << ": " << eline << "行call语句后面不是过程名称或过程没有定义";
		errors[etop++] = oss.str();
		break;
	}
	case 107: {
		oss << e << ": " << eline << "行因子出错";
		errors[etop++] = oss.str();
		break;
	}

	case 108: {
		oss << e << ": " << eline << "行该层中未声明的标识符";
		errors[etop++] = oss.str();
		break;
	}

	case 109: {
		oss << e << ": " << eline << "行标识符重复定义";
		errors[etop++] = oss.str();
		break;
	}

	case 110: {
		oss << e << ": " << eline << "行不可修改的左值";
		errors[etop++] = oss.str();
		break;
	}
	}
	return e;
}

void parser::printTable() {
	std::cout << std::setiosflags(std::ios::left) << std::setw(12) << "name"
			  << std::setiosflags(std::ios::left) << std::setw(12) << "sym"
			  << std::setiosflags(std::ios::left) << std::setw(12) << "value"
			  << std::setiosflags(std::ios::left) << std::setw(12) << "level"
			  << std::setiosflags(std::ios::left) << std::setw(12) << "address" << std::endl;

	for (int i = 0; i < symbolTable.size(); i++) {
		SYMBOL symbol = symbolTable.at(i);

		std::cout << std::setiosflags(std::ios::left) << std::setw(12) << symbol.name;

		if (SymToString.find(symbol.kind) != SymToString.end())
			std::cout << std::setiosflags(std::ios::left) << std::setw(12) << SymToString[symbol.kind];
		/*
		name
sym
 "val:" 
 "lev:" 
 "addr:"
*/
		std::cout << std::setiosflags(std::ios::left) << std::setw(12) << symbol.val
				  << std::setiosflags(std::ios::left) << std::setw(12) << symbol.lev
				  << std::setiosflags(std::ios::left) << std::setw(12) << symbol.addr
				  << std::endl;
	}
}

void parser::printCode() {
	for (int i = 0; i < codeTable.size(); i++) {
		CODE inst = codeTable.at(i);

		std::cout << "No:" << i << "\t";
		std::cout << "op:" << TisToString[inst.fun] << "\t";
		std::cout << "lev:" << inst.lev << "\t";
		std::cout << "addr:" << inst.offset << std::endl;
	}
}

void parser::generateFile(const std::string& filename) {
	// char _Drive[9];	 //文件后缀
	// char _Dir[99];
	// char _Filename[99];

	// _splitpath(filename, _Drive, _Dir, _Filename, NULL);

	// char targetSym[99], targetCode[99];	 //符号表文件和目标代码文件

	// strcat(targetSym, ".sym");

	// strcat(targetCode, ".pl0");

	FILE *fp1, *fp2;
	if ((fp1 = fopen((filename + ".sym").c_str(), "wb")) == NULL) {
		std::cout << "创建.sym文件失败!" << std::endl;
		exit(-1);
	}
	for (int i = 0; i < symbolTable.size(); i++) {
		fwrite(&symbolTable[i], sizeof(SYMBOL), 1, fp1);
	}

	if ((fp2 = fopen((filename + ".pl0cache").c_str(), "wb")) == NULL) {
		std::cout << "创建.pl0文件失败!" << std::endl;
		exit(-1);
	}
	for (int i = 0; i < codeTable.size(); i++) {
		fwrite(&codeTable[i], sizeof(CODE), 1, fp2);
	}

	fclose(fp1);
	fclose(fp2);
}
