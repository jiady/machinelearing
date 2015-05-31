#include <iostream>
#include  <fstream>
#include  <cstdio>
#include  <cstdlib>
#include <time.h>
#include <string>
#include <unistd.h>
#include <sys/types.h> 
#include <wait.h>
#include <signal.h>
#include <sstream>
#include <pthread.h>
#include <cstdlib>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <cstdlib>
#include <sys/stat.h>
#include <fstream>
using namespace std;
double thd[]={-8,-6,-4,-2,-1,-0.5,0,0.5,1,2,4,6,8};
int sizethd=sizeof(thd)/sizeof(double);


int main(int argc, char ** argv){
	//usage: Roc input output
	ifstream input(argv[1]);
	ofstream output(argv[2]);
	if(!input ){
		cout<<"can not open input"<<endl;
		exit(0);
	}
	if(!output){
		cout<<"can not open output"<<endl;
		exit(0);
	}	
	int FP[sizethd],TP[sizethd],TN[sizethd],FN[sizethd];
	for(int i=0;i<sizethd;i++){
		FP[i]=TP[i]=TN[i]=FN[i]=0;
	}
	string line;
	while(getline(input,line)){
		if(line.size()==0)
			break;
		int ix=line.find("\t");
		double a=stod(line);
		double b=stod(line.substr(ix));
		for(int i=0;i<sizethd;i++){
			if(a>=thd[i] && b>=0){
				TP[i]++;
			}else if(a>=thd[i] && b<0){
				FP[i]++;
			}else if(a<thd[i] && b>=0){
				FN[i]++;
			}else if(a<thd[i] && b<0){
				TN[i]++;
			}
		}
	}
	for(int i=0;i<sizethd;i++){
		output<<"thd:\t"<<thd[i]<<"\tTP\t"<<TP[i]<<"\tTN\t"<<TN[i]<<"\tFP\t"<<FP[i]<<"\tFN\t"<<FN[i]<<endl;
	}

	return 0;
}