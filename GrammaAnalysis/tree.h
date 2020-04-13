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
	Node* makenode ();//创建一个子节点并返回新创建的子节点
	Node* makenode (string content);//根据内容创建一个子节点并返回新创建的子节点



	string content;
	int count;
	Node* child[max_child];
	Node* parent;


};
class Tree {
public:
	Tree () { root = new Node ("主程序"); }
	Tree (string filename) {
		root = new Node ("主程序");
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

