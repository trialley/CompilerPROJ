#include <lexer.h>

#include <exception>
#include <stdexcept>

lexer::lexer(const std::string& filename) {
	index_pointer = 0;
	openSrc(filename);
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

void lexer::getChar() {
	temp_ch = buffer[index_pointer++];

	if (temp_ch == '\n') {	//换行

		row++;

		if (readLine()) {
			index_pointer = 0;
			temp_ch = buffer[index_pointer++];	//取新行的第一个字符
		} else {
			throw std::exception(std::logic_error("文件读到结尾！"));
		}
	}
}

void lexer::getBC() {  //跳过空格
	while ((temp_ch == ' ') || (temp_ch == '\t') || (temp_ch == '\r')) {
		try {
			getChar();
		}

		catch (std::exception e) {
			throw e;
		}
	}
}

void lexer::retract() {	 //
	if (index_pointer > 0)
		index_pointer--;
	temp_ch = ' ';
}

bool lexer::isLetter(const char c) {
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return true;

	else
		return false;
}

bool lexer::isDigit(const char c) {
	if (c >= '0' && c <= '9')
		return true;

	else
		return false;
}

int lexer::reserve(const std::string& strToken) {  //在关键字表中查询，返回保留字的编码

	for (int i = 0; i < RESERVE_LEN; i++) {
		if (KeyWords[i] == strToken) {
			return i + 1;
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
		getChar();
	} catch (std::exception e) {
		_tempSymEntry.sym = INVALID;
		return _tempSymEntry;
	}

	try {
		getBC();
	}

	catch (std::exception e) {
		_tempSymEntry.sym = INVALID;
		return _tempSymEntry;
	}

	if (isLetter(temp_ch)) {  //start with letter,which means it is a identifier

		while ((isLetter(temp_ch) || isDigit(temp_ch))) {
			char s[2];
			s[0] = temp_ch, s[1] = '\0';
			strcat(strToken, s);  //修改了strToken

			try {
				getChar();
			} catch (std::exception e) {
				_tempSymEntry.sym = INVALID;
				return _tempSymEntry;
			}
		}

		retract();

		code = reserve(strToken);  //返回关键字的编码

		if (!code) {  //不在关键字表中，是标识符
			_tempSymEntry.sym = _IDENT;
			_tempSymEntry.name = strToken;
			LOG << "symbol:" << _tempSymEntry.name << std::endl;
			return _tempSymEntry;

		} else {
			_tempSymEntry.sym = code;
			LOG << "key word:" << KeyWords[code - 1] << std::endl;
			return _tempSymEntry;
		}

	} else if (isDigit(temp_ch)) {
		while (isDigit(temp_ch)) {
			char s[2];
			s[0] = temp_ch, s[1] = '\0';
			strcat(strToken, s);

			try {
				getChar();
			} catch (std::exception e) {
				_tempSymEntry.sym = INVALID;
				return _tempSymEntry;
			}
		}

		retract();

		_tempSymEntry.sym = _NUMBER;
		_tempSymEntry.name = strToken;

		LOG << "number:" << _tempSymEntry.name << std::endl;
		return _tempSymEntry;
	}

	LOG << "opeartor:";
	if (temp_ch == '>') {
		std::cout << "opeartor:";
		try {
			getChar();
		} catch (std::exception e) {
			_tempSymEntry.sym = INVALID;
			return _tempSymEntry;
		}
		if (temp_ch == '=') {
			_tempSymEntry.sym = _BIGGER_EQ;

			std::cout << ">=" << std::endl;
			return _tempSymEntry;

		} else {
			retract();
			_tempSymEntry.sym = _BIGGER;

			std::cout << ">" << std::endl;
			return _tempSymEntry;
		}

	} else if (temp_ch == '<') {
		std::cout << "opeartor:";
		try {
			getChar();
		} catch (std::exception e) {
			_tempSymEntry.sym = INVALID;
			return _tempSymEntry;
		}

		if (temp_ch == '=') {
			_tempSymEntry.sym = _LOWER_EQ;

			std::cout << "<=" << std::endl;
			return _tempSymEntry;

		} else {
			retract();
			_tempSymEntry.sym = _LOWER;

			std::cout << "<" << std::endl;
			return _tempSymEntry;
		}

	} else if (temp_ch == '=') {
		std::cout << "opeartor:";
		_tempSymEntry.sym = _EQ;
		std::cout << "=" << std::endl;
		return _tempSymEntry;
	} else if (temp_ch == '#') {
		std::cout << "opeartor:";
		_tempSymEntry.sym = _UEQ;
		std::cout << "#" << std::endl;
		return _tempSymEntry;
	} else if (temp_ch == ':') {
		std::cout << "opeartor:";
		try {
			getChar();
		}

		catch (std::exception e) {
			_tempSymEntry.sym = INVALID;
			return _tempSymEntry;
		}

		if (temp_ch == '=') {
			_tempSymEntry.sym = _ASSIGN;
			std::cout << ":=" << std::endl;
			return _tempSymEntry;
		}

		else {
			retract();
			/*:后面不是=*/
		}
	} else if (temp_ch == '+') {
		_tempSymEntry.sym = _PLUS;

		std::cout << "+" << std::endl;
		return _tempSymEntry;
	} else if (temp_ch == '-') {
		_tempSymEntry.sym = _MINUS;
		std::cout << "-" << std::endl;
		return _tempSymEntry;
	} else if (temp_ch == '*') {
		try {
			getChar();
		} catch (std::exception e) {
			_tempSymEntry.sym = INVALID;
			return _tempSymEntry;
		}

		if (temp_ch == '*') {
			_tempSymEntry.sym = _POWER;

			std::cout << "**" << std::endl;
			return _tempSymEntry;
		} else {
			retract();
			_tempSymEntry.sym = _STAR;

			std::cout << "*" << std::endl;
			return _tempSymEntry;
		}

	} else if (temp_ch == '/') {
		_tempSymEntry.sym = _DIV;
		std::cout << "/" << std::endl;
		return _tempSymEntry;
	} else if (temp_ch == ',') {
		_tempSymEntry.sym = _COMMA;

		std::cout << "," << std::endl;
		return _tempSymEntry;
	} else if (temp_ch == ';') {
		_tempSymEntry.sym = _SEMICOLON;

		std::cout << ";" << std::endl;
		return _tempSymEntry;
	} else if (temp_ch == '(') {
		_tempSymEntry.sym = _LPAIR;

		std::cout << "(" << std::endl;
		return _tempSymEntry;
	} else if (temp_ch == ')') {
		_tempSymEntry.sym = _RPAIR;

		std::cout << ")" << std::endl;
		return _tempSymEntry;
	} else if (temp_ch == '{') {
		_tempSymEntry.sym = _LBRACE;

		std::cout << "{" << std::endl;
		return _tempSymEntry;
	} else if (temp_ch == '}') {
		_tempSymEntry.sym = _RBRACE;

		std::cout << "}" << std::endl;
		return _tempSymEntry;
	} else {
		printf("Illegal input!\n");

		_tempSymEntry.sym = INVALID;
		return _tempSymEntry;
	}
}
