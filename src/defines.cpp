#include "defines.h"

// TODO:  在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用

char* rsv_[] = {"begin", "call", "const", "procedure", "if", "then", "do", "end", "while", "odd", "read", "write", "var"};	//关键字;

map<SymbolKind, const char*> symMap;
map<FunctionCode, const char*> opMap;