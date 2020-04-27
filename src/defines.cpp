// stdafx.cpp : 只包括标准包含文件的源文件
// compile_ex2.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "defines.h"

// TODO:  在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用

char* rsv_[] = { "begin", "call", "const", "procedure", "if", "then", "do", "end", "while", "odd", "read", "write", "var" };//关键字;

map<SymbolKind, const char*> symMap;
map<FunctionCode, const char*> opMap;

