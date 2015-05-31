#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
using namespace std;
int main(){
	ifstream fin("../perprocess/test.txt");
	ofstream fout("exp.txt");
	string label;
	int k=0;
	string tmp;
	int op=0;
	do{
		getline(fin,tmp);
		if(tmp.size()==0 || tmp.back()==EOF){
			break;
		}
		label=tmp.substr(0,2);
		k=stoi(label);
		fout<<k<<endl;
	}while(tmp.size()>0);
	fout.close();
	return 0;
}
