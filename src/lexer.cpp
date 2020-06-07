#include <lexer.h>

#include <exception>
#include <stdexcept>

lexer::lexer(const std::string& filename) {
	initMap();

	index_pointer = 0;
	openSrc(filename);
}

void lexer::initMap() {	 //
	SymToString[CONST] = "CONSTANT";
	SymToString[VAR] = "VARIABLE";
	SymToString[PROD] = "PROCEDURE";

	TisToString[LIT] = std::string("LIT 将常数置于栈顶");
	TisToString[LOD] = std::string("LOD 将变量值置于栈顶");
	TisToString[STO] = std::string("STO 将栈顶的值赋与某变量");
	TisToString[CAL] = std::string("CAL 用于过程调用的指令");
	TisToString[INT] = std::string("INT 在数据栈中分配存贮空间");
	TisToString[JMP] = std::string("JMP 跳转");
	TisToString[JPC] = std::string("JPC 栈顶0则跳转");
	TisToString[OPR] = std::string("OPR 一组算术或逻辑运算指令");
}

void lexer::openSrc(const std::string& src) {
	fp = fopen(src.c_str(), "rb");
	if (fp == NULL) {
		std::cout << "open file PushError!" << std::endl;
		return;
	}
	row = 1;
}

bool lexer::readLine() {
	if ((fgets(buffer, BUFFERLEN, fp)) != NULL)
		return 1;

	else
		return 0;
}

void lexer::GetChar() {
	ch = buffer[index_pointer++];

	if (ch == '\n') {  //换行

		row++;

		if (readLine()) {
			index_pointer = 0;
			ch = buffer[index_pointer++];  //取新行的第一个字符
		} else {
			throw std::exception(std::logic_error("文件读到结尾！"));
		}
	}
}

void lexer::GetBC() {  //跳过空格
	while ((ch == ' ') || (ch == '\t') || (ch == '\r')) {
		try {
			GetChar();
		}

		catch (std::exception e) {
			throw e;
		}
	}
}

void lexer::Retract() {	 //
	if (index_pointer > 0)
		index_pointer--;
	ch = ' ';
}

bool lexer::IsLetter(const char c) {
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return true;

	else
		return false;
}

bool lexer::IsDigit(const char c) {
	if (c >= '0' && c <= '9')
		return true;

	else
		return false;
}

int lexer::Reserve(const std::string& strToken) {  //在关键字表中查询，返回保留字的编码

	for (int i = 0; i < RESERVE_LEN; i++) {
		if (KeyWords[i] == strToken) {
			return i + 1;//代表关键词索引
		}
	}

	return 0;  //没找到保留字
}

symEntry lexer::GETSYM() {
	int code, value;
	symEntry _tempSymEntry;
	char strToken[MAX_ID_LEN];
	strToken[0] = '\0';

	try {
		GetChar();
	} catch (std::exception e) {
		_tempSymEntry.sym = INVALID;
		return _tempSymEntry;
	}

	try {
		GetBC();
	}

	catch (std::exception e) {
		_tempSymEntry.sym = INVALID;
		return _tempSymEntry;
	}

	if (IsLetter(ch)) {	 //start with letter,which means it is a identifier

		while ((IsLetter(ch) || IsDigit(ch))) {
			char s[2];
			s[0] = ch, s[1] = '\0';
			strcat(strToken, s);  //添加到strToken

			try {
				GetChar();
			} catch (std::exception e) {
				_tempSymEntry.sym = INVALID;
				return _tempSymEntry;
			}
		}

		Retract();//回退一个索引

		code = Reserve(strToken);  //返回关键字的编码

		if (!code) {  //不在关键字表中，是标识符
			_tempSymEntry.sym = _IDENT;
			_tempSymEntry.name = strToken;
			LOG << "symbol:" << _tempSymEntry.name << std::endl;
			return _tempSymEntry;

		} else {//在关键词表中，
			_tempSymEntry.sym = code;//赋予当前符号结构的code
			LOG << "key word:" << KeyWords[code - 1] << std::endl;
			return _tempSymEntry;
		}

	} else if (IsDigit(ch)) {
		while (IsDigit(ch)) {
			char s[2];
			s[0] = ch, s[1] = '\0';
			strcat(strToken, s);

			try {
				GetChar();
			} catch (std::exception e) {
				_tempSymEntry.sym = INVALID;
				return _tempSymEntry;
			}
		}

		Retract();

		_tempSymEntry.sym = _NUMBER;
		_tempSymEntry.name = strToken;

		LOG << "number:" << _tempSymEntry.name << std::endl;
		return _tempSymEntry;
	}

	LOG << "opeartor:";
	if (ch == '>') {
		std::cout << "opeartor:";
		try {
			GetChar();
		} catch (std::exception e) {
			_tempSymEntry.sym = INVALID;
			return _tempSymEntry;
		}
		if (ch == '=') {
			_tempSymEntry.sym = _BIGGER_EQ;

			std::cout << ">=" << std::endl;
			return _tempSymEntry;

		} else {
			Retract();
			_tempSymEntry.sym = _BIGGER;

			std::cout << ">" << std::endl;
			return _tempSymEntry;
		}

	} else if (ch == '<') {
		std::cout << "opeartor:";
		try {
			GetChar();
		} catch (std::exception e) {
			_tempSymEntry.sym = INVALID;
			return _tempSymEntry;
		}

		if (ch == '=') {
			_tempSymEntry.sym = _LOWER_EQ;

			std::cout << "<=" << std::endl;
			return _tempSymEntry;

		} else {
			Retract();
			_tempSymEntry.sym = _LOWER;

			std::cout << "<" << std::endl;
			return _tempSymEntry;
		}

	} else if (ch == '=') {
		std::cout << "opeartor:";
		_tempSymEntry.sym = _EQ;
		std::cout << "=" << std::endl;
		return _tempSymEntry;
	} else if (ch == '#') {
		std::cout << "opeartor:";
		_tempSymEntry.sym = _UEQ;
		std::cout << "#" << std::endl;
		return _tempSymEntry;
	} else if (ch == ':') {
		std::cout << "opeartor:";
		try {
			GetChar();
		}

		catch (std::exception e) {
			_tempSymEntry.sym = INVALID;
			return _tempSymEntry;
		}

		if (ch == '=') {
			_tempSymEntry.sym = _ASSIGN;
			std::cout << ":=" << std::endl;
			return _tempSymEntry;
		}

		else {
			Retract();
			/*:后面不是=*/
		}
	} else if (ch == '+') {
		_tempSymEntry.sym = _PLUS;

		std::cout << "+" << std::endl;
		return _tempSymEntry;
	} else if (ch == '-') {
		_tempSymEntry.sym = _MINUS;
		std::cout << "-" << std::endl;
		return _tempSymEntry;
	} else if (ch == '*') {
		try {
			GetChar();
		} catch (std::exception e) {
			_tempSymEntry.sym = INVALID;
			return _tempSymEntry;
		}

		if (ch == '*') {
			_tempSymEntry.sym = _POWER;

			std::cout << "**" << std::endl;
			return _tempSymEntry;
		} else {
			Retract();
			_tempSymEntry.sym = _STAR;

			std::cout << "*" << std::endl;
			return _tempSymEntry;
		}

	} else if (ch == '/') {
		_tempSymEntry.sym = _DIV;
		std::cout << "/" << std::endl;
		return _tempSymEntry;
	} else if (ch == ',') {
		_tempSymEntry.sym = _COMMA;

		std::cout << "," << std::endl;
		return _tempSymEntry;
	} else if (ch == ';') {
		_tempSymEntry.sym = _SEMICOLON;

		std::cout << ";" << std::endl;
		return _tempSymEntry;
	} else if (ch == '(') {
		_tempSymEntry.sym = _LPAIR;

		std::cout << "(" << std::endl;
		return _tempSymEntry;
	} else if (ch == ')') {
		_tempSymEntry.sym = _RPAIR;

		std::cout << ")" << std::endl;
		return _tempSymEntry;
	} else if (ch == '{') {
		_tempSymEntry.sym = _LBRACE;

		std::cout << "{" << std::endl;
		return _tempSymEntry;
	} else if (ch == '}') {
		_tempSymEntry.sym = _RBRACE;

		std::cout << "}" << std::endl;
		return _tempSymEntry;
	} else {
		printf("Illegal input!\n");

		_tempSymEntry.sym = INVALID;
		return _tempSymEntry;
	}
}
