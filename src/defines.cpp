#include <defines.h>

#include <string>
#include <vector>

std::vector<std::string> KeyWords{"begin", "call", "const", "procedure", "if", "then", "do", "end", "while", "odd", "read", "write", "var"};

std::map<SymbolType, std::string> SymToString{
	{CONST, "CONSTANT"},
	{VAR, "VARIABLE"},
	{PROD, "PROCEDURE"}};
std::map<InsType, std::string> TisToString{
	{LIT, "LIT"},
	{LOD, "LOD"},
	{STO, "STO"},
	{CAL, "CAL"},
	{INT, "INT"},
	{JMP, "JMP"},
	{JPC, "JPC"},
	{OPR, "OPR"},
};

void err(std::string in) {
	throw std::exception(std::logic_error(in));
}