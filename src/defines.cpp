#include "defines.h"

// TODO:  �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������

char* rsv_[] = {"begin", "call", "const", "procedure", "if", "then", "do", "end", "while", "odd", "read", "write", "var"};	//�ؼ���;

map<SymbolKind, const char*> symMap;
map<FunctionCode, const char*> opMap;