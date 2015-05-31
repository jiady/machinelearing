#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;
int main(int argc , char ** argv){
	ifstream finp,finn;
	//main folder
	ofstream fout;
	int op=0;
	string tmp;
	for(int i=0;i<3;i++){
		for(int j=0;j<9;j++){
			string str="train00.txt";
			string divp="train_p_0.txt",divn="train_n_0.txt";
			str[5]='0'+i;
			str[6]='0'+j;
			fout.open(str.c_str(),ios::out);
			divp[8]='0'+i;
			divn[8]='0'+j;
			divp.insert(0,argv[1]);
			divn.insert(0,argv[1]);
			finn.open(divn.c_str());
			finp.open(divp.c_str());
			bool flagn=true,flagp=true;
			while(flagn && flagp){
				if(rand()%2==0){
					getline(finp,tmp);
					if(tmp.size()==0){
						flagp=false;
						break;
					}else{
						fout<<tmp<<endl;
					}
				}else{
					getline(finn,tmp);
					if(tmp.size()==0){
						flagn=false;
						break;
					}else{
						fout<<tmp<<endl;
					}
				}
			}
			while(flagn){
				getline(finp,tmp);
				if(tmp.size()==0){
					flagp=false;
					break;
				}else{
					fout<<tmp<<endl;
				}
			}
			while(flagp){
				getline(finn,tmp);
				if(tmp.size()==0){
					flagn=false;
					break;
				}else{
					fout<<tmp<<endl;
				}
			}
			fout.close();
			finn.close();
			finp.close();
		}
	}
	
	return 0;
}
