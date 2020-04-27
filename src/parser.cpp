#include "parser.h"

parser::parser(const std::string& filename) {
	wa = new lexer(filename);
	wa->readLine();

	t = wa->getSym();
	if (t.sym == INVALID)
		return;

	BLOCK();  //�﷨��������

	vector<CODE>::iterator iter = codeTable.begin();
	codeTable.insert(iter, CODE(JMP, 0, mainEntry));  //���м������׼���

	generateFile(filename);

	if (!etop) {  //û�д���
		cout << endl;
		cout << "compile success" << endl;
	} else {  //����������

		cout << endl;
		for (int i = 0; i < etop; i++) {
			cout << errors[i] << endl;
		}
		cout << "compile failed" << endl;
		exit(-1);
	}
}

void parser::BLOCK() {
	lev++;
	dx = 3;

	if (t.sym == $CONST) {	//��CONST����
		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeConst();

		while (t.sym == $COMMA)	 //����
		{
			t = wa->getSym();
			if (t.sym == INVALID)
				return;

			analyzeConst();
		}

		if (t.sym == $SEMICOLON) {	//�����ֺ�

			t = wa->getSym();
			if (t.sym == INVALID)
				return;
		} else {  //�Ȳ��Ƕ���Ҳ���Ƿֺ�
			/*���������û����;����*/
			error(10, wa->row);
		}
	}

	if (t.sym == $VAR) {  //��VAR����

		//cout << "����˵�����֣�" << endl;
		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeVar();

		while (t.sym == $COMMA)	 //����
		{
			t = wa->getSym();
			if (t.sym == INVALID)
				return;

			analyzeVar();
		}

		if (t.sym == $SEMICOLON) {	//�����˷ֺ�

			t = wa->getSym();
			if (t.sym == INVALID)
				return;
		}

		else {
			/*�������û����;����*/
			error(13, wa->row);
		}
	}

	if (t.sym == $PROCEDURE) {	//�ǹ���˵��

		// cout << "����˵�����֣�" << endl;

		t = wa->getSym();
		if (t.sym == INVALID)
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

	//cout << "����˵�����֣�" << endl;

	if (t.sym == $IDENT) {	//��ȡ��ʶ��

		char* id = new char[MAX_ID_LEN];
		strcpy(id, t.name);

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		if (t.sym == $ASSIGN)  //�Ǹ�ֵ��
		{
			// ������ǵȺţ����Ǹ�ֵ����:=���׳�1�Ŵ���
			error(19, wa->row);
		} else if (t.sym == $EQ) {
			t = wa->getSym();
			if (t.sym == INVALID)
				return;

			if (t.sym == $NUMBER) {	 //������

				if (!insertSymbol(SymbolKind::CONST, id))
					error(109, wa->row);
				/*�ض���*/;	 //���뵽���ű�ʧ��

				t = wa->getSym();
				if (t.sym == INVALID)
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

	//cout << "����˵�����֣�" << endl;

	if (t.sym == $IDENT) {
		char* id = new char[MAX_ID_LEN];
		strcpy(id, t.name);
		//char* id = t.name;

		if (!insertSymbol(SymbolKind::VAR, id))
			error(109, wa->row);
		/*�ض���*/

		t = wa->getSym();
		if (t.sym == INVALID)
			return;	 //ȡ��һ����
	} else {
		// ����������������������ĵ�һ���ַ����Ǳ�ʶ��
		error(12, wa->row);
	}
}

void parser::analyzePro() {	 //����˵������

	if (t.sym == $IDENT) {
		char* id = new char[MAX_ID_LEN];
		strcpy(id, t.name);
		if (!insertSymbol(SymbolKind::PROD, id))
			error(109, wa->row);
		/*�ض���*/

		t = wa->getSym();
		if (t.sym == INVALID)
			return;
	} else {
		/*�����ײ�û�б�ʶ��*/
		error(20, wa->row);
	}

	if (t.sym == $SEMICOLON) {
		t = wa->getSym();
		if (t.sym == INVALID)
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

	if (t.sym == $SEMICOLON) {	//end;��ķ��š�����������

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		while (t.sym == $PROCEDURE) {  //������в��еĹ���˵��

			t = wa->getSym();
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

	switch (t.sym) {
	case $IDENT: { /*��ֵ���*/

		i = searchSymbol(t.name);
		if (i < 0) { /*û���ҵ����ŵ�����*/
			error(108, wa->row);
		}

		else if (symbolTable.at(i).kind != SymbolKind::VAR) {
			// ����ڷ��ű����ҵ��ˣ����Ǹñ�ʶ�����Ǳ�������
			/*�����޸ĵ���ֵ*/
			error(110, wa->row);
		}

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		if (t.sym == $ASSIGN) {	 //�Ǹ�ֵ��

			t = wa->getSym();
			if (t.sym == INVALID)
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

	case $CALL: {
		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		if (t.sym != $IDENT) {	//call ���治�Ǳ�ʶ��
			error(34, wa->row);
		} else {
			i = searchSymbol(t.name);  //���ұ�ʶ����Ӧ�ķ��ű�

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

			t = wa->getSym();
			if (t.sym == INVALID)
				return;
		}

		break;
	}

	case $IF: {
		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeCond();	//����if���������
		cx1 = cx;		//jpc���ռ�õ�λ��
		cx++;

		if (t.sym == $THEN) {
			t = wa->getSym();
			if (t.sym == INVALID)
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

	case $BEGIN: {
		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeSent();

		while (t.sym != INVALID) {	//����д���е���

			if (t.sym == $SEMICOLON) {	//tΪ";",˵�����滹�����

				t = wa->getSym();
				if (t.sym == INVALID)
					return;

				analyzeSent();
			} else if (t.sym == $END) {	 //tΪend����ʾ����������

				t = wa->getSym();
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

	case $WHILE: {
		cx1 = cx;  // ��¼��ǰ�������λ�ã�����whileѭ���Ŀ�ʼλ�ã�Ҳ������������ת���ĵط�

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeCond();	//�ж�while������������

		cx2 = cx;  //jpc������ĵط�
		cx++;	   //ռһ��Codeλ��

		if (t.sym == $DO) {
			t = wa->getSym();
			if (t.sym == INVALID)
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

	case $READ: {
		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		if (t.sym == $LPAIR) {
			t = wa->getSym();
			if (t.sym == INVALID)
				return;

			if (t.sym == $IDENT) {
				i = searchSymbol(t.name);
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

			t = wa->getSym();
			if (t.sym == INVALID)
				return;

			while (t.sym == $COMMA) {  //�������������

				t = wa->getSym();
				if (t.sym == INVALID)
					return;

				if (t.sym == $IDENT) {
					i = searchSymbol(t.name);
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

				t = wa->getSym();
				if (t.sym == INVALID)
					return;
			}

			if (t.sym != $RPAIR) {
				/*�������Ų�ƥ��*/
				error(27, wa->row);
			}

		} else {
			/*����������*/
			error(29, wa->row);
		}

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		break;
	}

	case $WRITE: {
		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		if (t.sym == $LPAIR) {
			t = wa->getSym();
			analyzeExpr();
			GEN(OPR, 0, OPR::WRITE);

			while (t.sym == $COMMA) {
				t = wa->getSym();
				if (t.sym == INVALID)
					return;

				analyzeExpr();

				GEN(OPR, 0, OPR::WRITE);
			}

			if (t.sym != $RPAIR) {
				/*�������Ų�ƥ��*/
				error(31, wa->row);
			} else {
				t = wa->getSym();
				if (t.sym == INVALID)
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
	if (t.sym == $ODD) {  //һԪ�����

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeExpr();	//�������ʽ

		//codeTable.push_back(CODE(OPR, 0, OPR::ODD));//������ż�ж�ָ��
		GEN(OPR, 0, OPR::ODD);
	}

	else {	//��Ԫ�����

		analyzeExpr();	//������߱��ʽ
		relop = t.sym;	//�����Ԫ�����

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeExpr();	//�����ұ߱��ʽ

		switch (relop) {
		case $EQ:
			//codeTable.push_back(CODE(OPR, 0, OPR::EQ));
			GEN(OPR, 0, OPR::EQ);
			break;

		case $UEQ:
			//codeTable.push_back(CODE(OPR, 0, OPR::UE));
			GEN(OPR, 0, OPR::UE);
			break;

		case $LOWER:
			//codeTable.push_back(CODE(OPR, 0, OPR::LT));
			GEN(OPR, 0, OPR::LT);
			break;

		case $BIGGER_EQ:
			//codeTable.push_back(CODE(OPR, 0,OPR::GE));
			GEN(OPR, 0, OPR::GE);
			break;

		case $BIGGER:
			//codeTable.push_back(CODE(OPR, 0, OPR::GT));
			GEN(OPR, 0, OPR::GT);
			break;

		case $LOWER_EQ:
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
	if (t.sym == $PLUS || t.sym == $MINUS) {  //���ʽ��������ͷ

		addop = t.sym;

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeTerm();	//������Ĵ���

		if (addop == $MINUS) {
			//codeTable.push_back(CODE(OPR, 0, OPR::MINUS));//ȡ������
			GEN(OPR, 0, OPR::MINUS);
		}
	} else {
		analyzeTerm();	//���ʽ��Ŀ�ͷ��ֱ����ķ���
	}

	while (t.sym == $PLUS || t.sym == $MINUS) {
		addop = t.sym;	//���������������addop��

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeTerm();

		if (addop == $PLUS) {  // �������Ϻ�����ղű�����ǼӺţ������ɼӷ�ָ��

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

	while (t.sym == $STAR || t.sym == $DIV) {
		mulop = t.sym;	/// �������������mulop��(�˷����ǳ���)

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeElem();	// �������Ӧ����һ�����ӣ��������ӷ���

		if (mulop == $STAR) {
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
	while (t.sym == $IDENT || t.sym == $NUMBER || t.sym == $LPAIR) {
		switch (t.sym) {
		case $IDENT: {
			i = searchSymbol(t.name);
			if (i < 0) {
				/*û���ҵ�����*/
				error(108, wa->row);
			}

			else if (symbolTable.at(i).kind == SymbolKind::CONST) {
				// ����ñ�ʶ��Ϊ����,������litָ��,��val�ŵ�ջ��
				//codeTable.push_back(CODE(LIT, 0, symbolTable.at(i).val));
				GEN(LIT, 0, symbolTable.at(i).val);

				t = wa->getSym();
				if (t.sym == INVALID)
					return;
			}

			else if (symbolTable.at(i).kind == SymbolKind::VAR) {
				//codeTable.push_back(CODE(LOD, lev - symbolTable.at(i).lev,
				//symbolTable.at(i).addr));
				GEN(LOD, lev - symbolTable.at(i).lev,
					symbolTable.at(i).addr);

				t = wa->getSym();
				if (t.sym == INVALID)
					return;
			} else if (symbolTable.at(i).kind == SymbolKind::PROD) {
				/*�ñ�ʶ��Ϊ������������*/
				error(43, wa->row);
				t = wa->getSym();
				if (t.sym == INVALID)
					return;
			}

			break;
		}

		case $NUMBER:  //���ӷ�����������
		{
			int num = atoi(t.name); /*Ҫ�жϷ�Χ*/

			//codeTable.push_back(CODE(LIT, 0, num));// ����litָ��������ֵ���泣���ŵ�ջ��
			GEN(LIT, 0, num);

			t = wa->getSym();
			if (t.sym == INVALID)
				return;

			break;
		}

		case $LPAIR: {
			t = wa->getSym();
			if (t.sym == INVALID)
				return;

			analyzeExpr();

			if (t.sym == $RPAIR) {
				t = wa->getSym();
				if (t.sym == INVALID)
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
		int num = atoi(t.name);	 //Խ����
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
	ostringstream oss;

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
	for (int i = 0; i < symbolTable.size(); i++) {
		SYMBOL symbol = symbolTable.at(i);

		cout << "name:" << symbol.name << "\t";

		if (symMap.find(symbol.kind) != symMap.end())
			cout << "sym:" << symMap[symbol.kind] << "\t";
		else
			cout << "sym:"
				 << "û��"
				 << "\t";

		cout << "val:" << symbol.val << "\t";
		cout << "lev:" << symbol.lev << "\t";
		cout << "addr:" << symbol.addr << endl;
	}
}

void parser::printCode() {
	for (int i = 0; i < codeTable.size(); i++) {
		CODE inst = codeTable.at(i);

		cout << "No:" << i << "\t";
		cout << "op:" << opMap[inst.fun] << "\t";
		cout << "lev:" << inst.lev << "\t";
		cout << "addr:" << inst.offset << endl;
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
		cout << "����.sym�ļ�ʧ��!" << endl;
		exit(-1);
	}
	for (int i = 0; i < symbolTable.size(); i++) {
		fwrite(&symbolTable[i], sizeof(SYMBOL), 1, fp1);
	}

	if ((fp2 = fopen((filename + ".pl0cache").c_str(), "wb")) == NULL) {
		cout << "����.pl0�ļ�ʧ��!" << endl;
		exit(-1);
	}
	for (int i = 0; i < codeTable.size(); i++) {
		fwrite(&codeTable[i], sizeof(CODE), 1, fp2);
	}

	fclose(fp1);
	fclose(fp2);
}
