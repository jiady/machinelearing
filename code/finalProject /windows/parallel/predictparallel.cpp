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

PROCESS_INFORMATION Predict_Process(int data){
	int i,j;
	j=data%9;
	i=data/9;
	string input = "../testx.txt",model = "model00.model",output = "res00.txt";
	output[3] = model[5] = i+'0';
	output[4] = model[6] = j+'0';
	cout <<  "start predicting from " <<  model <<  endl;
	fout <<  "start predicting from " <<  model <<  endl;
	char str[128];
	sprintf(str, "..\\liblinear-1.96\\windows\\predict.exe %s %s %s"
		, input.c_str(), model.c_str(), output.c_str());
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

void Predict(){
	gettimeofday(&starttime, &tz);
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 9;j++){
			int id=j+i*9;//0-26
			pid[id] = Predict_Process(id);
		}
	for(int i = 0;i < 27;i++){
        cout<<"Waiting Process "<<i<<endl;
        wait(pid[i]);
	}
	gettimeofday(&endtime, &tz);
	long long cost = endtime.tv_usec - starttime.tv_usec;
	fout <<  "Predicting time:" <<double(cost)/1000000 <<  endl;
}

int main(){
	fout.open("predict_time_record.txt");
	Predict();
	fout.close();
	return 0;
}
