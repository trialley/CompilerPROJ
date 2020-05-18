#include "lexer.h"

#include <exception>
#include <stdexcept>

lexer::lexer(const std::string& filename) {
	initMap();

	index_pointer = 0;
	openSrc(filename);
}

void lexer::initMap() {	 //
	symMap[CONST] = "CONSTANT";
	symMap[VAR] = "VARIABLE";
	symMap[PROD] = "PROCEDURE";

	opMap[LIT] = "LIT";
	opMap[LOD] = "LOD";
	opMap[STO] = "STO";
	opMap[CAL] = "CAL";
	opMap[INT] = "INT";
	opMap[JMP] = "JMP";
	opMap[JPC] = "JPC";
	opMap[OPR] = "OPR";
}

void lexer::openSrc(const std::string& src) {
	fp = fopen(src.c_str(), "rb");
	if (fp == NULL) {
		std::cout << "open file error!" << std::endl;
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

int lexer::Reserve(const char* strToken) {	//在关键字表中查询，返回保留字的编码

	for (int i = 0; i < RESERVE_LEN; i++) {
		if (rsv_[i] == std::string(strToken)) {
			return i + 1;
		}
	}

	return 0;  //没找到保留字
}

symEntry lexer::getSym() {
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
			strcat(strToken, s);  //修改了strToken

			try {
				GetChar();
			} catch (std::exception e) {
				_tempSymEntry.sym = INVALID;
				return _tempSymEntry;
			}
		}

		Retract();

		code = Reserve(strToken);  //返回关键字的编码

		if (!code) {  //不在关键字表中，是标识符
			_tempSymEntry.sym = _IDENT;
			strcpy(_tempSymEntry.name, strToken);
			LOG << "symbol:" << _tempSymEntry.name << std::endl;
			return _tempSymEntry;

		} else {
			_tempSymEntry.sym = code;
			LOG << "key word:" << rsv_[code - 1] << std::endl;
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
		strcpy(_tempSymEntry.name, strToken);

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
