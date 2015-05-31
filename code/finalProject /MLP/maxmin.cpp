#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <vector>
using namespace std;
int main(){
	ifstream fin[3][9];
	string filename;
	ifstream fexp("exp.txt");
	ofstream fout("predict.txt");
	ofstream Result("result.txt");
	for(int i=0;i<3;i++)
		for(int j=0;j<9;j++){
			filename="resut";
			filename.push_back('0'+i);
			filename.push_back('0'+j);
			filename.append(".txt");
			fin[i][j].open(filename);
		}
	bool a[3],r;
	string tmp,label;
	int t;
	bool bk=false;
	vector<bool> lb;
	while(true){
		a[0]=a[1]=a[2]=true;
		for(int i=0;i<3;i++){
			for(int j=0;j<9;j++){
				getline(fin[i][j],tmp);
				if(tmp.size()==0|| tmp.back()==EOF){
					bk=true;
					break;
				}
				t=stoi(tmp);
				a[i]=a[i]&&(t==1);
			}
			if(bk) break;
		}
		if(bk) break;
		r=a[0]||a[1]||a[2];
		if(r){
			label="1";
		}else{
			label="-1";
		}
		fout<<label<<endl;
		lb.push_back(r);
	}
	for(int i=0;i<3;i++)
		for(int j=0;j<9;j++){
			fin[i][j].close();
	}
	string label1,label2;
	int count=0,countright=0,TP=0,FN=0,FP=0,TN=0;
	for(int i=0;i<lb.size();i++){
		getline(fexp,label1);
		if(label1.size()==0 ){
			break;
		}
		t=stoi(label1);
		if(lb[i]==true && t==1){
			TP++;
		}else if(lb[i]==true && t!=1){
			FP++;
		}else if(lb[i]==false && t==1){
			FN++;
		}else{
			TN++;
		}
	}
	double accu=double(TP+TN)/double(TP+TN+FP+FN);
	Result<<"accu:"<<accu<<endl;
	Result<<"TP:"<<TP<<endl;
	Result<<"TN:"<<TN<<endl;
	Result<<"FN:"<<FN<<endl;
	Result<<"FP:"<<FP<<endl;
	fout.close();
	return 0;
}
