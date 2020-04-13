#include <iostream>
#include <vector> 
#include <string> 
#include <fstream>  
using namespace std;

int main(){
	
	ifstream openfile("../test_lex.txt");
	string temp1,temp2,temp3;
	openfile>>temp1>>temp2>>temp3;
	cout<<temp1<<temp2<<temp3;
	
	return 0;
} 
