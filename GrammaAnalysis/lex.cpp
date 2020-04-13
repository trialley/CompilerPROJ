#include "lex.h"

/*��¶����������*/
char symTB[29][15] = {
"const","var","procedure","begin","end","odd","if","then","call","while","do","read","write",//0-12
"+","-","*","/","=","#","<","<=",">",">=",":=",";",",",".","(",")"//13-28
};//�ؼ�������������
Lexword word[word_max_num];
int wordnum = 0;//����ļ��е�������





LexicalAnalysis::LexicalAnalysis (string filename) {
	this->filename = "../" + filename;
}

//******************************************
//�ж��Ƿ�Ϊ��ĸ
bool LexicalAnalysis::IsLetter (char letter) {
	if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'))
		return true;
	else
		return false;

}
//******************************************
//�ж��Ƿ�Ϊ���� 
bool LexicalAnalysis::IsDigit (char digit) {
	if (digit >= '0' && digit <= '9')
		return true;
	else
		return false;
}
//******************************************
//���ҹؼ��ֱ��ҵ��������λ��+1��
int LexicalAnalysis::Retract (char* te) {
	for (int i = 0; i < 13; i++) {
		if (strcmp (te, symTB[i]) == 0) {
			return i + 1;
		}
	}
	return -1;
}
//******************************************
//�ж��Ƿ�Ϊ����������ҵ�����λ��+1�����򷵻�-1
int LexicalAnalysis::IsSoj (char soj) {

	for (int i = 13; i < 29; i++) {
		if (soj == symTB[i][0]) {
			return i + 1;
		}

	}
	return -1;
}
//******************************************
//�ж��Ƿ�Ϊ����������ҵ�����λ��+1�����򷵻�-1
int LexicalAnalysis::IsSoj (char* soj) {

	for (int i = 13; i < 29; i++) {
		if (strcmp (soj, symTB[i]) == 0) {
			return i + 1;
		}

	}
	return -1;
}
//******************************************
//�ӱ�ʶ�����в��ұ�ʶ�������û������룬��󷵻�ָ�루λ�ã�
int LexicalAnalysis::InsertId (char* te) {
	for (int i = 0; i < id; i++) {
		if (strcmp (te, idTB[i]) == 0) {
			return i;
		}
	}
	strcpy_s (idTB[id++], strlen (te) + 1, te);
	return id - 1;

}
//******************************************
//�����ֱ���в������֣����û������룬��󷵻�ָ�루λ�ã�
int LexicalAnalysis::InsertNum (char* te) {
	for (int i = 0; i < id; i++) {
		if (strcmp (te, numTB[i]) == 0) {
			return i;
		}
	}
	strcpy_s (numTB[num++], strlen (te) + 1, te);
	return num - 1;

}
//******************************************
//��һ����д�ַ�ת��ΪСд�ַ�
char LexicalAnalysis::tran (char te) {
	if (te >= 'A' && te <= 'Z') {
		te = te + 32;
		return te;
	} else
		return te;
}
//******************************************
//����ע�ͣ�����ͳһ����дת��ΪСд
//void LexicalAnalysis::deal()
//{
//	if (!sourfile.is_open() || !destfile.is_open()) {
//		cout << "����Դ�ļ�δ�򿪳ɹ�\n";
//		return;
//	}
//	char tempx;
//
//	do
//	{
//		tempx = sourfile.get();
//		if (tempx == '/')
//		{
//			char tempy = sourfile.get();
//			if (tempy == '/')
//			{
//				while (sourfile.get() != 10);
//				tempx = sourfile.get();
//			}
//			else if (tempy == '*')
//			{
//				char tempz1 = sourfile.get();
//				char tempz2 = sourfile.get();
//
//				while (!(tempz1 == '*'&&tempz2 == '/'))
//				{
//
//					tempz1 = tempz2;
//					tempz2 = sourfile.get();
//					if (tempz1 == EOF || tempz2 == EOF)
//					{
//						tempx = EOF;
//						break;
//					}
//
//				}
//
//			}
//			else
//			{
//
//				destfile << tran(tempx) << tran(tempy);
//
//			}
//		}
//		else
//		{
//			//if (tempx != 10 && tempx != 13 && tempx != 9 && tempx != EOF )
//			if (tempx != EOF)
//				destfile << tran(tempx);
//		}
//	} while (tempx != EOF);
//	return;
//}
//******************************************
//�����ļ���ѭ������һ���ַ������������word�Լ��ļ��Լ��ն�
int LexicalAnalysis::getsym () {

	if (!sourfile.is_open () || !destfile.is_open ()) {
		cout << "����Դ�ļ�δ�򿪳ɹ�\n";
		return -1;
	}
	int row = 1;
	ch = ' ';
	destfile << left << setw (15) << "name" << left << setw (15) << "sym" << left << setw (15) << "value&addr" << left << setw (15) << "row" << "\n";
	while (ch != EOF) {
		p = 0;
		sourfile.get (ch);
		while (ch == ' ' || ch == 10)//���˿ո��뻻��
		{
			if (ch == 10) {
				row++;
			}
			ch = sourfile.get ();
		}

		if (ch == EOF) {
			return 1;
		}


		if (IsLetter (ch))//����������ĸ����Ҫô�ǹؼ��֣�Ҫô�Ǳ�ʶ��
		{
			strToken[p++] = tran (ch);
			sourfile.get (ch);
			while (IsDigit (ch) || IsLetter (ch))//��������ֻ���ĸ���������ȡ
			{
				strToken[p++] = tran (ch);
				sourfile.get (ch);
			}
			sourfile.seekg (-1, ios::cur);//����һ��
			code = Retract (strToken);//���ҹؼ��ֱ�
			if (code == -1) {//û�ҵ��������ʶ����
				value = InsertId (strToken);
				word[wordnum].name = strToken;
				word[wordnum].sym = IDENT;
				word[wordnum].row = row;
				wordnum++;
				//cout << left << setw(15) << idTB[value] << left << setw(15) << IDENT << left << setw(15) << value << "\n";
				destfile << left << setw (15) << idTB[value] << left << setw (15) << IDENT << left << setw (15) << value << left << setw (15) << row << "\n";
			} else {
				word[wordnum].name = strToken;
				word[wordnum].sym = code;
				word[wordnum].row = row;
				wordnum++;
				//cout << left << setw(15) << symTB[code - 1] << left << setw(15) << code << left << setw(15) << other << "\n";
				destfile << left << setw (15) << symTB[code - 1] << left << setw (15) << code << left << setw (15) << other << left << setw (15) << row << "\n";

			}

		} else if (IsDigit (ch)) {
			strToken[p++] = ch;
			sourfile.get (ch);
			while (IsDigit (ch))//��������֣��������ȡ
			{
				strToken[p++] = ch;
				sourfile.get (ch);
			}
			sourfile.seekg (-1, ios::cur);//����һ��

			value = InsertNum (strToken);
			word[wordnum].name = strToken;
			word[wordnum].sym = NUMBER;
			word[wordnum].row = row;
			wordnum++;
			string temp (strToken);
			int num = atoi (temp.c_str ());
			//cout << left << setw(15) << numTB[value] << left << setw(15) << NUMBER << left << setw(15) << value << "\n";
			destfile << left << setw (15) << numTB[value] << left << setw (15) << NUMBER << left << setw (15) << value << left << setw (15) << row << "\n";

		} else if ((code = IsSoj (ch)) != -1) {
			strToken[p++] = ch;
			if (ch == ':') {
				sourfile.get (ch);
				if (ch == '=') {
					strToken[p++] = ch;
					word[wordnum].name = strToken;
					word[wordnum].sym = code;
					word[wordnum].row = row;
					wordnum++;
					//cout << left << setw(15) << strToken << left << setw(15) << code << left << setw(15) << other << "\n";
					destfile << left << setw (15) << strToken << left << setw (15) << code << left << setw (15) << other << left << setw (15) << row << "\n";
				} else {
					sourfile.seekg (-1, ios::cur);//����һ��
					printf ("error\n");
					return -1;
				}
			} else if (ch == '>' || ch == '<') {
				sourfile.get (ch);
				if (ch == '=') {

					strToken[p++] = ch;
					code = IsSoj (strToken);
					word[wordnum].name = strToken;
					word[wordnum].sym = code;
					word[wordnum].row = row;
					wordnum++;
					//cout << left << setw(15) << strToken << left << setw(15) << code << left << setw(15) << other << "\n";
					destfile << left << setw (15) << strToken << left << setw (15) << code << left << setw (15) << other << left << setw (15) << row << "\n";
				} else {
					sourfile.seekg (-1, ios::cur);//����һ��
					word[wordnum].name = strToken;
					word[wordnum].sym = code;
					word[wordnum].row = row;
					wordnum++;
					//cout << left << setw(15) << strToken << left << setw(15) << code << left << setw(15) << other << "\n";
					destfile << left << setw (15) << strToken << left << setw (15) << code << left << setw (15) << other << left << setw (15) << row << "\n";
				}
			} else {
				word[wordnum].name = strToken;
				word[wordnum].sym = code;
				word[wordnum].row = row;
				wordnum++;
				//cout << left << setw(15) << strToken << left << setw(15) << code << left << setw(15) << other << "\n";
				destfile << left << setw (15) << strToken << left << setw (15) << code << left << setw (15) << other << left << setw (15) << row << "\n";
			}

		} else {
			printf ("error\n");
			return -1;
		}
		ch = ' ';
		memset (strToken, 0, sizeof (strToken));
		//memset(x, 0, sizeof(x));

	}
	return 1;


}
//******************************************
//�ʷ�������ڣ�ͨ���ļ������ļ����дʷ�����
bool LexicalAnalysis::lexanalysis () {
	bool flag = false;
	sourfile.open (filename);//��Դ�ļ����ж�
	string tempfilename = filename.substr (0, filename.length () - 4) + "_lex.txt";
	destfile.open (tempfilename, ios::out);//�½��ļ�����д
	if (!sourfile.is_open () || !destfile.is_open ()) {
		cout << "δ�ɹ����ļ�" << filename << endl;
		return flag;
	}
	int flagint = getsym ();
	sourfile.close ();
	destfile.close ();
	if (flagint == 1)
		flag = true;
	return flag;
	//printf("\n\n\n\n");
	//printf("id table\n");
	//for (int i = 0; i < id; i++)
	//{
	//	printf("%-15s%-15d\n", idTB[i], i);
	//}
	//printf("\n\n\n\n");
	//printf("num table\n");
	//for (int i = 0; i < num; i++)
	//{
	//	printf("%-15s%-15d\n", numTB[i], i);
	//}
}