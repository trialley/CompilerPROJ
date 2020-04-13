#include "tree.h"
//***************************************************************************
//**printspace(int m)������
//**��ӡ����Ϊm�Ŀո�
//***************************************************************************
void printspace(int m)
{
	for (int i = 0; i<m; i++)
		cout << " ";

};
//****************************************************************************
//**makenode()������
//**����һ���ӽڵ㲢�����´������ӽڵ㡣
//****************************************************************************
Node* Node::makenode() 
{
	Node *q = new Node();
	this->child[count++] = q;
	q->parent = this;
	return q;
}
//****************************************************************************
//**makenode()������
//**����һ���������ݵ��ӽڵ㲢�����´������ӽڵ㡣
//****************************************************************************
Node* Node::makenode(string content)
{
	Node *q = new Node(content);
	this->child[count++] = q;
	q->parent = this;
	return q;
}

//****************************************************************************
//**show(Node*p, int repeat)������
//**���ն˴�ӡ�ڵ�p�����������repeatΪһ���п�ʼ��ӡ��λ��
//****************************************************************************
void Tree::show(Node*p, int repeat)
{
	string temp = "|--- ";
	int templen = temp.length();
	
	if (p->count == 0) {
		cout << temp<< p->content << "\n";
		if (outfile.is_open())
		{
			outfile << temp<< p->content << "\n";
		}
	}
	else if (p->count > 0) {
		if (p == root) {
			cout << p->content;
			if (outfile.is_open())
			{
				outfile << p->content;
			}
			repeat = repeat + p->content.length();
		}
		else {
			cout << temp<< p->content;
			if (outfile.is_open())
			{
				outfile << temp<< p->content;
			}
			repeat = repeat + templen + p->content.length();
		}
		
		
		for (int i = 0; i < p->count; i++) {
			if (i == 0) {
				show(p->child[i], repeat);
			}
			else {
				printspace(repeat);
				cout << "|\n";
				printspace(repeat);
				if (outfile.is_open())
				{
					for (int i = 0; i<repeat; i++)
						outfile << " ";
					outfile << "|\n";
					for (int i = 0; i<repeat; i++)
						outfile << " ";
				}
				show(p->child[i], repeat);

				
				
			}
			
			
		}
	}
	
}