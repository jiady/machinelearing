#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sstream>
#include <pthread.h>
#include <cstdlib>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <windows.h>
using namespace std;
ofstream fout;

struct timeval starttime, endtime;
struct timezone tz;

PROCESS_INFORMATION Train_Process(int data){
	int i,j;
	j=data%9;
	i=data/9;
	string input = "train00.txt",model = "model00.model";
	input[5] = model[5] = i+'0';
	input[6] = model[6] = j+'0';
	cout <<  "Process " <<  input <<  endl;
	fout <<  "Process " <<  input <<  endl;
	char str[128];
	sprintf(str, "..\\liblinear-1.96\\windows\\train.exe %s %s"
		, input.c_str(), model.c_str());
	LPSTR cmdLine = str;
	STARTUPINFO startupInfo={sizeof(startupInfo)};
	PROCESS_INFORMATION procInfo;
	startupInfo.wShowWindow=TRUE;
	startupInfo.dwFlags=STARTF_USESHOWWINDOW;
	BOOL ret = CreateProcess(
		NULL,
    	cmdLine,
    	NULL,
    	NULL,
    	FALSE,
    	0,
    	NULL,
    	NULL,
    	&startupInfo,
    	&procInfo);
	if(ret) CloseHandle(procInfo.hThread);
	return procInfo;
}

void wait(PROCESS_INFORMATION procInfo){
	DWORD dwExitCode;
	WaitForSingleObject(procInfo.hProcess, dwExitCode);
	CloseHandle(procInfo.hProcess);
	cout<<"Process id: "<<procInfo.dwProcessId<<endl;
	cout<<"Thread id: "<<procInfo.dwThreadId<<endl;
}

PROCESS_INFORMATION pid[27];

void Train(){
	gettimeofday(&starttime, &tz);
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 9;j++){
			int id=j+i*9;//0-26
			pid[id] = Train_Process(id);
		}
	for(int i = 0;i < 27;i++){
        cout<<"Waiting Process "<<i<<endl;
        wait(pid[i]);
	}
	gettimeofday(&endtime, &tz);
	long long cost = endtime.tv_usec - starttime.tv_usec;
	fout <<  "Total training time:" << double(cost)/1000000  <<  endl;
}

int main(){
	fout.open("time_record.txt");
	Train();
	fout.close();
	return 0;
}
