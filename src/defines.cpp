#include "defines.h"

#include <string>
#include <vector>
std::vector<std::string> rsv_{"begin", "call", "const", "procedure", "if", "then", "do", "end", "while", "odd", "read", "write", "var"};

std::map<SymbolKind, const char*> symMap;
std::map<FunctionCode, const char*> opMap;
