#pragma once
#include"include.h"
#define max_child 100
class Node {
public:
	Node () { parent = NULL; count = 0; }
	Node (string content) {
		parent = NULL;
		count = 0;
		this->content = content;
	}
	~Node () {
		for (int i = 0; i < count; i++) {
			delete child[i];
		}
	}
	Node* makenode ();//����һ���ӽڵ㲢�����´������ӽڵ�
	Node* makenode (string content);//�������ݴ���һ���ӽڵ㲢�����´������ӽڵ�



	string content;
	int count;
	Node* child[max_child];
	Node* parent;


};
class Tree {
public:
	Tree () { root = new Node ("������"); }
	Tree (string filename) {
		root = new Node ("������");
		//this->filename = filename; 
		this->filename = filename.substr (0, filename.length () - 4) + "_lextree.txt";
	}
	~Tree () { delete root; }

	void showtree () {
		outfile.open (filename, ios::out);
		show (root, 0);
		outfile.close ();
	}
	void show (Node* p, int repeat);
	Node* getroot () { return root; }


private:
	string filename;
	ofstream outfile;
	Node* root;
};

