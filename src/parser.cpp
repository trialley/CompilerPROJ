#include "parser.h"

#include <exception>
#include <iomanip>
#include <iostream>
parser::parser(const std::string& filename) {
	wa = new lexer(filename);  //�����µĴʷ�������
	wa->readLine();			   //��ȡһ��

	_tempSymEntry = wa->getSym();  //���һ�����ŵ�temp
	if (_tempSymEntry.sym == INVALID) {
		LOG << "�Ƿ���ʶ��";
		throw std::exception(std::logic_error("�Ƿ���ʶ��"));
		return;
	}

	BLOCK(); /*�﷨��������*/

	std::vector<CODE>::iterator iter = codeTable.begin();

	codeTable.insert(iter, CODE(JMP, 0, mainEntry));  //���м������׼���

	generateFile(filename);	 //���ɶ������ļ�

	if (!etop) {  //û�д���
		std::cout << std::endl;
		std::cout << "compile success" << std::endl;
	} else {  //����������

		std::cout << std::endl;
		for (int i = 0; i < etop; i++) {
			std::cout << errors[i] << std::endl;
		}
		std::cout << "compile failed" << std::endl;
		exit(-1);
	}
}

void parser::BLOCK() {
	lev++;	//��Σ��������
	dx = 3;

	if (_tempSymEntry.sym == _CONST) {	//��CONST����
		_tempSymEntry = wa->getSym();	//�ٻ�ȡһ������
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeConst();	 //����һ��const���͵ı�������

		while (_tempSymEntry.sym == _COMMA)	 //���ţ����������
		{
			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			analyzeConst();
		}

		if (_tempSymEntry.sym == _SEMICOLON) {	//�����ֺ�
			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;
		} else {
			/*�Ȳ��Ƕ���Ҳ���Ƿֺţ��������û����;����*/
			error(10, wa->row);
		}
	}

	if (_tempSymEntry.sym == _VAR) {  //��VAR����

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeVar();

		while (_tempSymEntry.sym == _COMMA)	 //����
		{
			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			analyzeVar();
		}

		if (_tempSymEntry.sym == _SEMICOLON) {	//�����˷ֺ�

			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;
		}

		else {
			/*�������û����;����*/
			error(13, wa->row);
		}
	}

	if (_tempSymEntry.sym == _PROCEDURE) {	//�ǹ���˵��

		// cout << "����˵�����֣�" << std::endl;

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzePro();
	}
	//}
	/*˵�����ֽ���*/
	if (lev == 0)  //�������.ppp
		mainEntry = cx + offset;

	GEN(INT, 0, dx);  // ���ɿռ����ָ�����dx���ռ䣨3���ռ�+��������Ŀ��

	analyzeSent();	//�������������

	GEN(OPR, 0, 0);	 //�˳��������
}

void parser::GEN(FunctionCode fun, int lev, int offset) {
	codeTable.push_back(CODE(fun, lev, offset));
	cx++;
}

void parser::analyzeConst() {  //����˵������
	LOG << "����const��������\n";
	if (_tempSymEntry.sym == _IDENT) {	//��ȡ��ʶ��

		char* id = new char[MAX_ID_LEN];
		strcpy(id, _tempSymEntry.name);

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		if (_tempSymEntry.sym == _ASSIGN)  //�Ǹ�ֵ��
		{
			// ������ǵȺţ����Ǹ�ֵ����:=���׳�1�Ŵ���
			error(19, wa->row);
		} else if (_tempSymEntry.sym == _EQ) {
			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			if (_tempSymEntry.sym == _NUMBER) {	 //������

				if (!insertSymbol(SymbolKind::CONST, id))
					error(109, wa->row);
				/*�ض���*/;	 //���뵽���ű�ʧ��

				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;
			}

			else {	//��ֵ�ź�������

				error(18, wa->row);
			}
		}

		else {	//��ʶ�����ǵȺ�

			error(19, wa->row);
		}
	} else {  //const ��û�мӱ�ʶ��

		error(11, wa->row);
	}
}

void parser::analyzeVar() {	 //����˵������

	//cout << "����˵�����֣�" << std::endl;

	if (_tempSymEntry.sym == _IDENT) {
		char* id = new char[MAX_ID_LEN];
		strcpy(id, _tempSymEntry.name);
		//char* id = t.name;

		if (!insertSymbol(SymbolKind::VAR, id))
			error(109, wa->row);
		/*�ض���*/

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;	 //ȡ��һ����
	} else {
		// ����������������������ĵ�һ���ַ����Ǳ�ʶ��
		error(12, wa->row);
	}
}

void parser::analyzePro() {	 //����˵������

	if (_tempSymEntry.sym == _IDENT) {
		char* id = new char[MAX_ID_LEN];
		strcpy(id, _tempSymEntry.name);
		if (!insertSymbol(SymbolKind::PROD, id))
			error(109, wa->row);
		/*�ض���*/

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;
	} else {
		/*�����ײ�û�б�ʶ��*/
		error(20, wa->row);
	}

	if (_tempSymEntry.sym == _SEMICOLON) {
		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;
	} else {  //������û���ԷֺŽ���
		error(17, wa->row);
	}

	//���浱ǰdx��lev
	int tempdx = dx;
	int templev = lev;

	BLOCK();  //�ֳ����ڲ�����˵����Ĺ���

	//�ָ�
	dx = tempdx;
	lev = templev;

	if (_tempSymEntry.sym == _SEMICOLON) {	//end;��ķ��š�����������

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		while (_tempSymEntry.sym == _PROCEDURE) {  //������в��еĹ���˵��

			_tempSymEntry = wa->getSym();
			analyzePro();
		}
	} else {
		/*����˵��end��û�мӷֺ�*/
		error(22, wa->row);
	}
}

void parser::analyzeSent() {
	int i;
	int cx1;
	int cx2;

	switch (_tempSymEntry.sym) {
	case _IDENT: { /*��ֵ���*/

		i = searchSymbol(_tempSymEntry.name);
		if (i < 0) { /*û���ҵ����ŵ�����*/
			error(108, wa->row);
		}

		else if (symbolTable.at(i).kind != SymbolKind::VAR) {
			// ����ڷ��ű����ҵ��ˣ����Ǹñ�ʶ�����Ǳ�������
			/*�����޸ĵ���ֵ*/
			error(110, wa->row);
		}

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		if (_tempSymEntry.sym == _ASSIGN) {	 //�Ǹ�ֵ��

			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;
		} else {
			/*��������û�мӸ�ֵ��*/
			error(14, wa->row);
		}

		analyzeExpr();	//����ֵ���Ҳ��ı��ʽ

		if (i >= 0) {
			GEN(STO, lev - symbolTable.at(i).lev, symbolTable.at(i).addr);
		}

		break;
	}

	case _CALL: {
		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		if (_tempSymEntry.sym != _IDENT) {	//call ���治�Ǳ�ʶ��
			error(34, wa->row);
		} else {
			i = searchSymbol(_tempSymEntry.name);  //���ұ�ʶ����Ӧ�ķ��ű�

			if (i < 0) {
				error(108, wa->row);
			}

			else if (symbolTable.at(i).kind == SymbolKind::PROD) {	//���ͼ��

				GEN(CAL, lev - symbolTable.at(i).lev, symbolTable.at(i).addr);
			}

			else {	//call ��ӵĲ���proc

				/*���õı�ʾ�����ǹ�����*/
				error(34, wa->row);
			}

			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;
		}

		break;
	}

	case _IF: {
		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeCond();	//����if���������
		cx1 = cx;		//jpc���ռ�õ�λ��
		cx++;

		if (_tempSymEntry.sym == _THEN) {
			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			analyzeSent();														   //then������
			codeTable.insert(codeTable.begin() + cx1, CODE(JPC, 0, cx + offset));  //����then���֮��Ĵ����ַ

		} else {
			/*�������ȱ��then*/
			error(36, wa->row);
		}

		//cx = codeTable.size();

		//codeTable.push_back(CODE(JPC, 0, 0));
		//codeTable.at(cx1).offset = codeTable.size();
		break;
	}

	case _BEGIN: {
		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeSent();

		while (_tempSymEntry.sym != INVALID) {	//����д���е���

			if (_tempSymEntry.sym == _SEMICOLON) {	//tΪ";",˵�����滹�����

				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;

				analyzeSent();
			} else if (_tempSymEntry.sym == _END) {	 //tΪend����ʾ����������

				_tempSymEntry = wa->getSym();
				//if (t.sym == INVALID) return;
				return;
			}

			else {
				//���������ַ�����˵������������
				error(23, wa->row);
				return;
			}
			//analyzeSent();
		}

		/*����������*/
		break;
	}

	case _WHILE: {
		cx1 = cx;  // ��¼��ǰ�������λ�ã�����whileѭ���Ŀ�ʼλ�ã�Ҳ������������ת���ĵط�

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeCond();	//�ж�while������������

		cx2 = cx;  //jpc������ĵط�
		cx++;	   //ռһ��Codeλ��

		if (_tempSymEntry.sym == _DO) {
			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			analyzeSent();

			GEN(JMP, 0, cx1 + offset);	//��ת��cx1�������ٴν����ж��Ƿ����ѭ��
			codeTable.insert(codeTable.begin() + cx2, CODE(JPC, 0, cx + offset));
		} else {
			/*while����û�н�do*/
			error(39, wa->row);
		}

		break;
	}

	case _READ: {
		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		if (_tempSymEntry.sym == _LPAIR) {
			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			if (_tempSymEntry.sym == _IDENT) {
				i = searchSymbol(_tempSymEntry.name);
				if (i < 0) {
					/*δ˵���ı�ʶ��*/
					error(108, wa->row);
				}

				else {
					//codeTable.push_back(CODE(OPR, 0, OPR::READ));// ����16�Ŷ�ָ��Ӽ��̶�ȡ����
					//codeTable.push_back(CODE(STO, lev - symbolTable.at(i).lev, symbolTable.at(i).addr));// ����stoָ�����ָ������
					GEN(OPR, 0, OPR::READ);
					GEN(STO, lev - symbolTable.at(i).lev, symbolTable.at(i).addr);
				}
			}

			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			while (_tempSymEntry.sym == _COMMA) {  //�������������

				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;

				if (_tempSymEntry.sym == _IDENT) {
					i = searchSymbol(_tempSymEntry.name);
					if (i < 0) {
						/*û�ҵ���ʶ��*/
						error(108, wa->row);
					} else {
						//	codeTable.push_back(CODE(OPR, 0, OPR::READ));//������
						//codeTable.push_back(CODE(STO, lev - symbolTable.at(i).lev, symbolTable.at(i).addr));
						GEN(OPR, 0, OPR::READ);
						GEN(STO, lev - symbolTable.at(i).lev, symbolTable.at(i).addr);
					}
				}

				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;
			}

			if (_tempSymEntry.sym != _RPAIR) {
				/*�������Ų�ƥ��*/
				error(27, wa->row);
			}

		} else {
			/*����������*/
			error(29, wa->row);
		}

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		break;
	}

	case _WRITE: {
		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		if (_tempSymEntry.sym == _LPAIR) {
			_tempSymEntry = wa->getSym();
			analyzeExpr();
			GEN(OPR, 0, OPR::WRITE);

			while (_tempSymEntry.sym == _COMMA) {
				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;

				analyzeExpr();

				GEN(OPR, 0, OPR::WRITE);
			}

			if (_tempSymEntry.sym != _RPAIR) {
				/*�������Ų�ƥ��*/
				error(31, wa->row);
			} else {
				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;
			}
		}

		break;
	}

	default:  //����begin��ͷ
		//error(8, wa->row);
		break;
	}
}

void parser::analyzeCond() {
	int relop;
	if (_tempSymEntry.sym == _ODD) {  //һԪ�����

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeExpr();	//�������ʽ

		//codeTable.push_back(CODE(OPR, 0, OPR::ODD));//������ż�ж�ָ��
		GEN(OPR, 0, OPR::ODD);
	}

	else {	//��Ԫ�����

		analyzeExpr();				//������߱��ʽ
		relop = _tempSymEntry.sym;	//�����Ԫ�����

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeExpr();	//�����ұ߱��ʽ

		switch (relop) {
		case _EQ:
			//codeTable.push_back(CODE(OPR, 0, OPR::EQ));
			GEN(OPR, 0, OPR::EQ);
			break;

		case _UEQ:
			//codeTable.push_back(CODE(OPR, 0, OPR::UE));
			GEN(OPR, 0, OPR::UE);
			break;

		case _LOWER:
			//codeTable.push_back(CODE(OPR, 0, OPR::LT));
			GEN(OPR, 0, OPR::LT);
			break;

		case _BIGGER_EQ:
			//codeTable.push_back(CODE(OPR, 0,OPR::GE));
			GEN(OPR, 0, OPR::GE);
			break;

		case _BIGGER:
			//codeTable.push_back(CODE(OPR, 0, OPR::GT));
			GEN(OPR, 0, OPR::GT);
			break;

		case _LOWER_EQ:
			//codeTable.push_back(CODE(OPR, 0, OPR::LE));
			GEN(OPR, 0, OPR::LE);
			break;

		default:
			/*����Ĳ����߼������*/
			break;
		}
	}
}

void parser::analyzeExpr() {
	int addop;
	if (_tempSymEntry.sym == _PLUS || _tempSymEntry.sym == _MINUS) {  //���ʽ��������ͷ

		addop = _tempSymEntry.sym;

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeTerm();	//������Ĵ���

		if (addop == _MINUS) {
			//codeTable.push_back(CODE(OPR, 0, OPR::MINUS));//ȡ������
			GEN(OPR, 0, OPR::MINUS);
		}
	} else {
		analyzeTerm();	//���ʽ��Ŀ�ͷ��ֱ����ķ���
	}

	while (_tempSymEntry.sym == _PLUS || _tempSymEntry.sym == _MINUS) {
		addop = _tempSymEntry.sym;	//���������������addop��

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeTerm();

		if (addop == _PLUS) {  // �������Ϻ�����ղű�����ǼӺţ������ɼӷ�ָ��

			//codeTable.push_back(CODE(OPR, 0, OPR::ADD));
			GEN(OPR, 0, OPR::ADD);	//�ӷ�ָ��
		} else {
			//codeTable.push_back(CODE(OPR, 0, OPR::SUB));//����ָ��
			GEN(OPR, 0, OPR::SUB);
		}
	}
}

void parser::analyzeTerm() {
	int mulop;

	analyzeElem();

	while (_tempSymEntry.sym == _STAR || _tempSymEntry.sym == _DIV) {
		mulop = _tempSymEntry.sym;	/// �������������mulop��(�˷����ǳ���)

		_tempSymEntry = wa->getSym();
		if (_tempSymEntry.sym == INVALID)
			return;

		analyzeElem();	// �������Ӧ����һ�����ӣ��������ӷ���

		if (mulop == _STAR) {
			//codeTable.push_back(CODE(OPR, 0, OPR::MUL));//4�ų˷�ָ��
			GEN(OPR, 0, OPR::MUL);
		} else {
			//codeTable.push_back(CODE(OPR, 0, OPR::DIV));//����ָ��
			GEN(OPR, 0, OPR::DIV);
		}
	}
}

void parser::analyzeElem() {
	int i;
	while (_tempSymEntry.sym == _IDENT || _tempSymEntry.sym == _NUMBER || _tempSymEntry.sym == _LPAIR) {
		switch (_tempSymEntry.sym) {
		case _IDENT: {
			i = searchSymbol(_tempSymEntry.name);
			if (i < 0) {
				/*û���ҵ�����*/
				error(108, wa->row);
			}

			else if (symbolTable.at(i).kind == SymbolKind::CONST) {
				// ����ñ�ʶ��Ϊ����,������litָ��,��val�ŵ�ջ��
				//codeTable.push_back(CODE(LIT, 0, symbolTable.at(i).val));
				GEN(LIT, 0, symbolTable.at(i).val);

				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;
			}

			else if (symbolTable.at(i).kind == SymbolKind::VAR) {
				//codeTable.push_back(CODE(LOD, lev - symbolTable.at(i).lev,
				//symbolTable.at(i).addr));
				GEN(LOD, lev - symbolTable.at(i).lev,
					symbolTable.at(i).addr);

				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;
			} else if (symbolTable.at(i).kind == SymbolKind::PROD) {
				/*�ñ�ʶ��Ϊ������������*/
				error(43, wa->row);
				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;
			}

			break;
		}

		case _NUMBER:  //���ӷ�����������
		{
			int num = atoi(_tempSymEntry.name); /*Ҫ�жϷ�Χ*/

			//codeTable.push_back(CODE(LIT, 0, num));// ����litָ��������ֵ���泣���ŵ�ջ��
			GEN(LIT, 0, num);

			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			break;
		}

		case _LPAIR: {
			_tempSymEntry = wa->getSym();
			if (_tempSymEntry.sym == INVALID)
				return;

			analyzeExpr();

			if (_tempSymEntry.sym == _RPAIR) {
				_tempSymEntry = wa->getSym();
				if (_tempSymEntry.sym == INVALID)
					return;
			} else {
				/*�������Ų�ƥ��*/
				error(41, wa->row);
			}

			break;
		}
		}
	}
}

bool parser::insertSymbol(SymbolKind kind, const char* id) {
	if (searchSymbol(id) >= 0)	//�Ѿ��ҵ����ŵ�����
		return false;

	switch (kind) {
	case SymbolKind::CONST: {
		int num = atoi(_tempSymEntry.name);	 //Խ����
		symbolTable.push_back(SYMBOL(id, kind, num, 0, 0));
		break;
	}

	case SymbolKind::VAR: {
		symbolTable.push_back(SYMBOL(id, kind, 0xFFFFFFFF, lev, dx++));	 //����Ĭ��ֵ��-1
		break;
	}

	case SymbolKind::PROD: {
		symbolTable.push_back(SYMBOL(id, kind, 0, lev, cx + offset));
		break;
	}
	default:
		break;
	}

	return true;
}

int parser::searchSymbol(const char* id) {
	for (int i = 0; i < symbolTable.size(); i++) {
		SYMBOL s = symbolTable.at(i);
		if (!strcmp(id, s.name) && lev >= s.lev)  //s�ڵ�ǰ�㼰������������ͬ
			return i;
	}

	return -1;
}

int parser::error(int e, int eline) {
	std::ostringstream oss;

	switch (e) {
	case 0: {
		oss << e << ": " << eline << "���зǷ��ַ�";
		errors[etop++] = oss.str();
		break;
	}
	case 1: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "���г���14����ĸ�ĵ���";
		oss << e << ": " << eline << "���г���10����ĸ�ĵ���";
		errors[etop++] = oss.str();
		break;
	}
	case 2: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "�������ֿ�ͷ�ı�ʶ��";
		oss << e << ": " << eline << "�������ֿ�ͷ�ı�ʶ��";
		errors[etop++] = oss.str();
		break;
	}
	case 3: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "���г���10����ĸ�ı�ʶ��";
		oss << e << ": " << eline << "���г���10����ĸ�ı�ʶ��";
		errors[etop++] = oss.str();
		break;
	}
	case 4: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "�г����βû��.";
		oss << e << ": " << eline << "�г����βû��.";
		errors[etop++] = oss.str();
		break;
	}
	case 5: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "�г����ײ���ʶ������û��;";
		oss << e << ": " << eline << "�г����ײ���ʶ������û��;";
		errors[etop++] = oss.str();
		break;
	}
	case 6: {  //
		//errors[etop++] = itos(e) + ": " + itos(eline) + "�г����ײ�PRAGRAM����û�б�ʶ��";
		oss << e << ": " << eline << "�г����ײ�PROCEDURE����û�б�ʶ��";
		errors[etop++] = oss.str();
		break;
	}
	case 7: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "��û�г����ײ�";
		oss << e << ": " << eline << "��û�г����ײ�";
		errors[etop++] = oss.str();
		break;
	}
	case 8: {
		oss << e << ": " << eline << "�зֳ����ȱ����䲿��";
		errors[etop++] = oss.str();
		break;
	}
	case 9: {
		oss << e << ": " << eline << "�г���˵����,�ַ������д���";
		errors[etop++] = oss.str();
		break;
	}
	case 10: {
		oss << e << ": " << eline << "�г���˵������û��;��β";
		errors[etop++] = oss.str();
		break;
	}
	case 11: {
		oss << e << ": " << eline << "�г��������д���";
		errors[etop++] = oss.str();
		break;
	}
	case 12: {
		oss << e << ": " << eline << "�б���˵��,����ȱ�ٱ�ʶ��";
		errors[etop++] = oss.str();
		break;
	}
	case 13: {
		oss << e << ": " << eline << "�б���˵��û��;��β";
		errors[etop++] = oss.str();
		break;
	}
	case 14: {
		oss << e << ": " << eline << "��VAR����û�ж���ı�ʶ��";
		errors[etop++] = oss.str();
		break;
	}

	case 15: {
		oss << e << ": " << eline << "�зֳ������û��;��β";
		errors[etop++] = oss.str();
		break;
	}
	case 16: {
		oss << e << ": " << eline << "�й���˵������û�зֳ���";
		errors[etop++] = oss.str();
		break;
	}
	case 17: {
		oss << e << ": " << eline << "�й����ײ�����û��;��β";
		errors[etop++] = oss.str();
		break;
	}
	case 18: {
		oss << e << ": " << eline << "�г�������û��ָ������";
		errors[etop++] = oss.str();
		break;
	}
	case 19: {
		oss << e << ": " << eline << "�г�������û��=�ַ�";
		errors[etop++] = oss.str();
		break;
	}
	case 20: {
		oss << e << ": " << eline << "�й����ײ�PROCEDURE���沢û�б�ʶ��";
		errors[etop++] = oss.str();
		break;
	}
	case 21: {
		oss << e << ": " << eline << "��������û��;��β";
		errors[etop++] = oss.str();
		break;
	}
	case 22: {
		oss << e << ": " << eline << "��û��END��β�����ߴ������ȱ��;��β";
		errors[etop++] = oss.str();
		break;
	}
	case 23: {
		oss << e << ": " << eline << "�и�������BEGIN���治�����";
		errors[etop++] = oss.str();
		break;
	}
	case 24: {
		oss << e << ": " << eline << "�и�ֵ�����治�Ǳ��ʽ";
		errors[etop++] = oss.str();
		break;
	}
	case 25: {
		oss << e << ": " << eline << "�и�ֵ���û��:=��ֵ��";
		errors[etop++] = oss.str();
		break;
	}
	case 26: {
		oss << e << ": " << eline << "��READ���������зǷ�,�ַ�";
		errors[etop++] = oss.str();
		break;
	}
	case 27: {
		oss << e << ": " << eline << "��READ���ȱ��)�ַ�";
		errors[etop++] = oss.str();
		break;
	}
	case 28: {
		oss << e << ": " << eline << "��READ���ȱ�ٱ�ʶ��";
		errors[etop++] = oss.str();
		break;
	}
	case 29: {
		oss << e << ": " << eline << "��READ���ȱ��(�ַ�";
		errors[etop++] = oss.str();
		break;
	}
	case 30: {
		oss << e << ": " << eline << "��WRITE���������зǷ�,�ַ�";
		errors[etop++] = oss.str();
		break;
	}
	case 31: {
		oss << e << ": " << eline << "��WRITE���ȱ��)�ַ�";
		errors[etop++] = oss.str();
		break;
	}
	case 32: {
		oss << e << ": " << eline << "��WRITE���ȱ�ٱ�ʶ��";
		errors[etop++] = oss.str();
		break;
	}
	case 33: {
		oss << e << ": " << eline << "��WRITE���ȱ��(�ַ�";
		errors[etop++] = oss.str();
		break;
	}
	case 34: {
		oss << e << ": " << eline << "��CALL���û�й�������";
		errors[etop++] = oss.str();
		break;
	}
	case 35: {
		oss << e << ": " << eline << "��IF����THEN���治�ǺϷ����";
		errors[etop++] = oss.str();
		break;
	}
	case 36: {
		oss << e << ": " << eline << "��IF������������û��THEN";
		errors[etop++] = oss.str();
		break;
	}
	case 37: {
		oss << e << ": " << eline << "��IF�����治�ǺϷ�����";
		errors[etop++] = oss.str();
		break;
	}
	case 38: {
		oss << e << ": " << eline << "��WHILE����DO���治�ǺϷ����";
		errors[etop++] = oss.str();
		break;
	}
	case 39: {
		oss << e << ": " << eline << "��WHILE���û��DO";
		errors[etop++] = oss.str();
		break;
	}
	case 40: {
		oss << e << ": " << eline << "��WHILE�����治�ǺϷ�������";
		errors[etop++] = oss.str();
		break;
	}
	case 41: {
		oss << e << ": " << eline << "�б��ʽ���ұ�ȱ��)�ַ�";
		errors[etop++] = oss.str();
		break;
	}
	case 42: {
		oss << e << ": " << eline << "���зǷ����ʽ";
		errors[etop++] = oss.str();
		break;
	}
	case 43: {
		oss << e << ": " << eline << "���зǷ�����";
		errors[etop++] = oss.str();
		break;
	}
	case 44: {
		oss << e << ": " << eline << "���зǷ���";
		errors[etop++] = oss.str();
		break;
	}
	case 45: {
		oss << e << ": " << eline << "���зǷ���";
		errors[etop++] = oss.str();
		break;
	}
	case 46: {
		oss << e << ": " << eline << "���зǷ���";
		errors[etop++] = oss.str();
		break;
	}
	case 47: {
		oss << e << ": " << eline << "���зǷ���";
		errors[etop++] = oss.str();
		break;
	}
	case 48: {
		oss << e << ": " << eline << "�й�ϵ������зǷ����ʽ";
		errors[etop++] = oss.str();
		break;
	}
	case 49: {
		oss << e << ": " << eline << "��ODD�зǷ����ʽ";
		errors[etop++] = oss.str();
		break;
	}
	case 101: {
		oss << e << ": " << eline << "�и��Ǳ�����ֵ�����û�ж���";
		errors[etop++] = oss.str();
		break;
	}
	case 102: {
		oss << e << ": " << eline << "��read������Ǳ��������û�ж���";
		errors[etop++] = oss.str();
		break;
	}
	case 103: {
		oss << e << ": " << eline << "��read������Ǳ��������û�ж���";
		errors[etop++] = oss.str();
		break;
	}
	case 106: {
		oss << e << ": " << eline << "��call�����治�ǹ������ƻ����û�ж���";
		errors[etop++] = oss.str();
		break;
	}
	case 107: {
		oss << e << ": " << eline << "�����ӳ���";
		errors[etop++] = oss.str();
		break;
	}

	case 108: {
		oss << e << ": " << eline << "�иò���δ�����ı�ʶ��";
		errors[etop++] = oss.str();
		break;
	}

	case 109: {
		oss << e << ": " << eline << "�б�ʶ���ظ�����";
		errors[etop++] = oss.str();
		break;
	}

	case 110: {
		oss << e << ": " << eline << "�в����޸ĵ���ֵ";
		errors[etop++] = oss.str();
		break;
	}
	}
	return e;
}

void parser::printTable() {
	std::cout << std::setiosflags(std::ios::left) << std::setw(12) << "name"
			  << std::setiosflags(std::ios::left) << std::setw(12) << "sym"
			  << std::setiosflags(std::ios::left) << std::setw(12) << "value"
			  << std::setiosflags(std::ios::left) << std::setw(12) << "level"
			  << std::setiosflags(std::ios::left) << std::setw(12) << "address" << std::endl;

	for (int i = 0; i < symbolTable.size(); i++) {
		SYMBOL symbol = symbolTable.at(i);

		std::cout << std::setiosflags(std::ios::left) << std::setw(12) << symbol.name;

		if (symMap.find(symbol.kind) != symMap.end())
			std::cout << std::setiosflags(std::ios::left) << std::setw(12) << symMap[symbol.kind];
		/*
		name
sym
 "val:" 
 "lev:" 
 "addr:"
*/
		std::cout << std::setiosflags(std::ios::left) << std::setw(12) << symbol.val
				  << std::setiosflags(std::ios::left) << std::setw(12) << symbol.lev
				  << std::setiosflags(std::ios::left) << std::setw(12) << symbol.addr
				  << std::endl;
	}
}

void parser::printCode() {
	for (int i = 0; i < codeTable.size(); i++) {
		CODE inst = codeTable.at(i);

		std::cout << "No:" << i << "\t";
		std::cout << "op:" << opMap[inst.fun] << "\t";
		std::cout << "lev:" << inst.lev << "\t";
		std::cout << "addr:" << inst.offset << std::endl;
	}
}

void parser::generateFile(const std::string& filename) {
	// char _Drive[9];	 //�ļ���׺
	// char _Dir[99];
	// char _Filename[99];

	// _splitpath(filename, _Drive, _Dir, _Filename, NULL);

	// char targetSym[99], targetCode[99];	 //���ű��ļ���Ŀ������ļ�

	// strcat(targetSym, ".sym");

	// strcat(targetCode, ".pl0");

	FILE *fp1, *fp2;
	if ((fp1 = fopen((filename + ".sym").c_str(), "wb")) == NULL) {
		std::cout << "����.sym�ļ�ʧ��!" << std::endl;
		exit(-1);
	}
	for (int i = 0; i < symbolTable.size(); i++) {
		fwrite(&symbolTable[i], sizeof(SYMBOL), 1, fp1);
	}

	if ((fp2 = fopen((filename + ".pl0cache").c_str(), "wb")) == NULL) {
		std::cout << "����.pl0�ļ�ʧ��!" << std::endl;
		exit(-1);
	}
	for (int i = 0; i < codeTable.size(); i++) {
		fwrite(&codeTable[i], sizeof(CODE), 1, fp2);
	}

	fclose(fp1);
	fclose(fp2);
}
