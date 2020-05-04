#include "defines.h"

#include <string>
#include <vector>
std::vector<std::string> rsv_{"begin", "call", "const", "procedure", "if", "then", "do", "end", "while", "odd", "read", "write", "var"};

map<SymbolKind, const char*> symMap;
map<FunctionCode, const char*> opMap;
