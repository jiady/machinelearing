#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
using namespace std;
void exit_with(const char * message){
	cout<<message<<endl;
	exit(0);
}

int main(int argc, char ** argv){
	if(argc<3) exit_with("usage: preprocess filenamesrc filnamedst");
	ifstream fin(argv[1]);
	ofstream fout(argv[2]);
	if(!fin){
		cout<<"can not open "<<argv[1]<<endl;
	}
	if(!fout){
		cout<<"can not open "<<argv[2]<<endl;
	}
	string str;
	int label=0;
	string lab;
	do{
		getline(fin,str);
		if(str.size()==0 || str.back()==EOF){
			//cout<<str.size()<<endl;
			break;
		}
		label=(str[0]=='A');
		if(label==0)
			label=-1;
		size_t first_space=str.find(" ");
		lab=to_string(label);
		str.erase(0,first_space);
		str.insert(0,lab);
		fout<<str<<endl;
	}while(str.size()>0);
	return 0;
}