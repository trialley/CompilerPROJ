#include "lex.h"

/*暴露给外界的数据*/
char symTB[29][15] = {
"const","var","procedure","begin","end","odd","if","then","call","while","do","read","write",//0-12
"+","-","*","/","=","#","<","<=",">",">=",":=",";",",",".","(",")"//13-28
};//关键字与算符界符表
Lexword word[word_max_num];
int wordnum = 0;//标记文件中单词数量





LexicalAnalysis::LexicalAnalysis (string filename) {
	this->filename = "../" + filename;
}

//******************************************
//判断是否为字母
bool LexicalAnalysis::IsLetter (char letter) {
	if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'))
		return true;
	else
		return false;

}
//******************************************
//判断是否为数字 
bool LexicalAnalysis::IsDigit (char digit) {
	if (digit >= '0' && digit <= '9')
		return true;
	else
		return false;
}
//******************************************
//查找关键字表，找到返回类别（位置+1）
int LexicalAnalysis::Retract (char* te) {
	for (int i = 0; i < 13; i++) {
		if (strcmp (te, symTB[i]) == 0) {
			return i + 1;
		}
	}
	return -1;
}
//******************************************
//判断是否为算符或界符，找到返回位置+1，否则返回-1
int LexicalAnalysis::IsSoj (char soj) {

	for (int i = 13; i < 29; i++) {
		if (soj == symTB[i][0]) {
			return i + 1;
		}

	}
	return -1;
}
//******************************************
//判断是否为算符或界符，找到返回位置+1，否则返回-1
int LexicalAnalysis::IsSoj (char* soj) {

	for (int i = 13; i < 29; i++) {
		if (strcmp (soj, symTB[i]) == 0) {
			return i + 1;
		}

	}
	return -1;
}
//******************************************
//从标识符表中查找标识符，如果没有则插入，最后返回指针（位置）
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
//从数字表表中查找数字，如果没有则插入，最后返回指针（位置）
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
//将一个大写字符转换为小写字符
char LexicalAnalysis::tran (char te) {
	if (te >= 'A' && te <= 'Z') {
		te = te + 32;
		return te;
	} else
		return te;
}
//******************************************
//过滤注释，并且统一将大写转换为小写
//void LexicalAnalysis::deal()
//{
//	if (!sourfile.is_open() || !destfile.is_open()) {
//		cout << "错误！源文件未打开成功\n";
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
//根据文件，循环处理一个字符串，并输出到word以及文件以及终端
int LexicalAnalysis::getsym () {

	if (!sourfile.is_open () || !destfile.is_open ()) {
		cout << "错误！源文件未打开成功\n";
		return -1;
	}
	int row = 1;
	ch = ' ';
	destfile << left << setw (15) << "name" << left << setw (15) << "sym" << left << setw (15) << "value&addr" << left << setw (15) << "row" << "\n";
	while (ch != EOF) {
		p = 0;
		sourfile.get (ch);
		while (ch == ' ' || ch == 10)//过滤空格与换行
		{
			if (ch == 10) {
				row++;
			}
			ch = sourfile.get ();
		}

		if (ch == EOF) {
			return 1;
		}


		if (IsLetter (ch))//读到的是字母，则要么是关键字，要么是标识符
		{
			strToken[p++] = tran (ch);
			sourfile.get (ch);
			while (IsDigit (ch) || IsLetter (ch))//如果是数字或字母，则继续读取
			{
				strToken[p++] = tran (ch);
				sourfile.get (ch);
			}
			sourfile.seekg (-1, ios::cur);//回退一格
			code = Retract (strToken);//查找关键字表
			if (code == -1) {//没找到，插入标识符表
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
			while (IsDigit (ch))//如果是数字，则继续读取
			{
				strToken[p++] = ch;
				sourfile.get (ch);
			}
			sourfile.seekg (-1, ios::cur);//回退一格

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
					sourfile.seekg (-1, ios::cur);//回退一格
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
					sourfile.seekg (-1, ios::cur);//回退一格
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
//词法分析入口，通过文件名对文件进行词法分析
bool LexicalAnalysis::lexanalysis () {
	bool flag = false;
	sourfile.open (filename);//打开源文件进行读
	string tempfilename = filename.substr (0, filename.length () - 4) + "_lex.txt";
	destfile.open (tempfilename, ios::out);//新建文件进行写
	if (!sourfile.is_open () || !destfile.is_open ()) {
		cout << "未成功打开文件" << filename << endl;
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