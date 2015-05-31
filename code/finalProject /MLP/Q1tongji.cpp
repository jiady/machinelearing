#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string.h>
using namespace std;

int main(int argc,char **argv){
	//main ,exp,test,res
	ifstream fin1(argv[1]);
	ifstream fin2(argv[2]);
	ofstream fout(argv[3]);
	int TP,FN,FP,TN;
	TP=FN=FP=TN=0;
	string ep,re;
	while(true){
		getline(fin1,ep);
		if(ep.size()==0) break;
		getline(fin2,re);
		if(re.size()==0) break;
		double a=stod(ep);
		double b=stod(re);
		if(a>=0 && b>=0){
			TP++;
		}else if(a>=0 && b<0){
			FN++;
		}else if(a<1 && b>=0){
			FP++;
		}else {
			TN++;
		}
	}
	fout<<"AC\t"<<double(TP+TN)/(TP+FP+TN+FN)<<endl;
	fout<<"TP\t"<<TP<<endl;
	fout<<"FP\t"<<FP<<endl;
	fout<<"TN\t"<<TN<<endl;
	fout<<"FN\t"<<FN<<endl;
	fout.close();
	return 0;
}