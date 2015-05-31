#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <sys/time.h>
using namespace std;
void exitwith(const char * m){
	cout<<m<<endl;
	exit(0);
}
int main(int argc,char **argv){
	if(argc<2) exitwith("Usage rdivide datasource");
	ifstream fin(argv[1]);
	if(!fin){
		exitwith("can not open datasource");
	}
	string str="train_p_0.txt";
	ofstream fout[12];
	for(int i=0;i<3;i++){
		str[6]='p';
		str[8]='0'+i;
		fout[i].open(str.c_str(),ios::out);
	}
	for(int i=0;i<9;i++){
		str[6]='n';
		str[8]='0'+i;
		fout[i+3].open(str.c_str(),ios::out);
	}
	int label=0;
	string tmp,subs,lab;
	int op=0;
	int t;
	char F;
	int k=0;
	timeval starttime,endtime;
	gettimeofday(&starttime,0);

	do{
		getline(fin,tmp);
		if(tmp.size()==0 || tmp.back()==EOF){
			break;
		}
		label=(tmp[0]=='A');
		F=tmp[0];
		if(label==0)
			label=-1;
		size_t first_space=tmp.find(" ");
		subs=tmp.substr(1,first_space);
		t=stoi(subs);
		lab=to_string(label);
		tmp.erase(0,first_space);
		tmp.insert(0,lab);	
		if(F=='A'){
			if(t<=46) k=0;
			else if(t<=61) k=1;
			else k=2;
			str[6]='p';
			str[8]='0'+k;
			op=str[8]-'0';
		}
		else{
			
			str[6]='n';
			str[8]='0'+rand()%9;
			op=3+str[8]-'0';
		
		}
		fout[op]<<tmp<<endl;
	}while(tmp.size()>0);
	for(int i=0;i<12;i++){
		fout[i].close();
	}
	gettimeofday(&endtime,0);
	double timeuse = 1000000*(endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;	
	timeuse /=1000;
	printf("use time:%f\n",timeuse);
	return 0;
}
