#include<iostream>
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
using namespace std;
ofstream fout;


struct timeval starttime, endtime;
struct timezone tz;

void Train_Process(int data){
	int i,j;
	j=data%9;
	i=data/9;
	string input = "train00.txt",model = "resut00.txt";
	input[5] = model[5] = i+'0';
	input[6] = model[6] = j+'0';
	cout <<  "Process " <<  input <<  endl;
	fout <<  "Process " <<  input <<  endl;
	execlp("./mlp", input.c_str(), "../preprocess/test.txt",model.c_str(), NULL);
	
}



void Train(){
	pid_t pid;
	gettimeofday(&starttime, &tz);
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 9;j++){
			int id=j+i*9;//0-26
			pid=fork();
			if(pid>0)
				continue;
			else{
				Train_Process(id);
				return;
			}
		}
	while(wait(NULL) != -1);
	gettimeofday(&endtime, &tz);
	double timeuse = 1000000*(endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
	fout <<  "Total training time:" << double(timeuse)/1000  << "ms" <<endl;
}



int main()
{
	fout.open("train_time_record.txt");
	Train();
	fout.close();
	return 0;
}
