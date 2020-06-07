#include <defines.h>

#include <string>
#include <vector>

//关键词向量
std::vector<std::string> KeyWords{"begin", "call", "const", "procedure", "if", "then", "do", "end", "while", "odd", "read", "write", "var"};

std::map<SymbolType, std::string> SymToString;//从符号到字符串
std::map<InsType, std::string> TisToString;//从指令到字符串
