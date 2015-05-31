#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <map>
using namespace std;
map<int,int> A;
map<int,int> B;
map<int,int> C;
map<int,int> D;

int main(){
	ifstream fin("../train.txt");
	ofstream fout("tongji.txt");
	
	int label=0;
	string subs;
	string tmp;
	int op=0;
	int t=0;
	do{
		getline(fin,tmp);
		if(tmp.size()==0 || tmp.back()==EOF){
			//cout<<str.size()<<endl;
			break;
		}
		size_t first_space=tmp.find(",");
		subs=tmp.substr(1,first_space);
		t=stoi(subs);
		if(tmp[0]=='A'){
			A[t]++;
		}else if(tmp[0]=='B'){
			B[t]++;
		}else if(tmp[0]=='C'){
			C[t]++;
		}else if(tmp[0]=='D'){
			D[t]++;
		}
		//fout<<tmp<<endl;
	}while(tmp.size()>0);
	int sum=0;
	for(auto m:A){
		sum+=m.second;
		fout<<'A'<<m.first<<":"<<m.second<<"\t#\t"<<sum<<endl;
	}
	sum=0;
	for(auto m:B){
		sum+=m.second;
		fout<<'B'<<m.first<<":"<<m.second<<"\t#\t"<<sum<<endl;
	}
	sum=0;
	for(auto m:C){
		sum+=m.second;
		fout<<'C'<<m.first<<":"<<m.second<<"\t#\t"<<sum<<endl;
	}
	sum=0;
	for(auto m:D){
		sum+=m.second;
		fout<<'D'<<m.first<<":"<<m.second<<"\t#\t"<<sum<<endl;
	}
	return 0;
}
