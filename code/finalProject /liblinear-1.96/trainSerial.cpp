#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <iostream>
#include "linear.h"
#include <time.h>
#include <sys/time.h>
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))
#define INF HUGE_VAL
using namespace std;

void print_null(const char *s) {}

void exit_with_help()
{
	printf(
	"Usage: train [options] training_set_file [model_file]\n"
	"options:\n"
	"-s type : set type of solver (default 1)\n"
	"  for multi-class classification\n"
	"	 0 -- L2-regularized logistic regression (primal)\n"
	"	 1 -- L2-regularized L2-loss support vector classification (dual)\n"
	"	 2 -- L2-regularized L2-loss support vector classification (primal)\n"
	"	 3 -- L2-regularized L1-loss support vector classification (dual)\n"
	"	 4 -- support vector classification by Crammer and Singer\n"
	"	 5 -- L1-regularized L2-loss support vector classification\n"
	"	 6 -- L1-regularized logistic regression\n"
	"	 7 -- L2-regularized logistic regression (dual)\n"
	"  for regression\n"
	"	11 -- L2-regularized L2-loss support vector regression (primal)\n"
	"	12 -- L2-regularized L2-loss support vector regression (dual)\n"
	"	13 -- L2-regularized L1-loss support vector regression (dual)\n"
	"-c cost : set the parameter C (default 1)\n"
	"-p epsilon : set the epsilon in loss function of SVR (default 0.1)\n"
	"-e epsilon : set tolerance of termination criterion\n"
	"	-s 0 and 2\n"
	"		|f'(w)|_2 <= eps*min(pos,neg)/l*|f'(w0)|_2,\n"
	"		where f is the primal function and pos/neg are # of\n"
	"		positive/negative data (default 0.01)\n"
	"	-s 11\n"
	"		|f'(w)|_2 <= eps*|f'(w0)|_2 (default 0.001)\n"
	"	-s 1, 3, 4, and 7\n"
	"		Dual maximal violation <= eps; similar to libsvm (default 0.1)\n"
	"	-s 5 and 6\n"
	"		|f'(w)|_1 <= eps*min(pos,neg)/l*|f'(w0)|_1,\n"
	"		where f is the primal function (default 0.01)\n"
	"	-s 12 and 13\n"
	"		|f'(alpha)|_1 <= eps |f'(alpha0)|,\n"
	"		where f is the dual function (default 0.1)\n"
	"-B bias : if bias >= 0, instance x becomes [x; bias]; if < 0, no bias term added (default -1)\n"
	"-wi weight: weights adjust the parameter C of different classes (see README for details)\n"
	"-v n: n-fold cross validation mode\n"
	"-q : quiet mode (no outputs)\n"
	);
	exit(1);
}

void exit_input_error(int line_num)
{
	fprintf(stderr,"Wrong input format at line %d with sit\n", line_num);
	exit(1);
}

static char *line = NULL;
static int max_line_len;

static char* readline(FILE *input)
{
	int len;

	if(fgets(line,max_line_len,input) == NULL)
		return NULL;

	while(strrchr(line,'\n') == NULL)
	{
		max_line_len *= 2;
		line = (char *) realloc(line,max_line_len);
		len = (int) strlen(line);
		if(fgets(line+len,max_line_len-len,input) == NULL)
			break;
	}
	return line;
}
struct feature_node *x_space;
struct parameter param;
struct problem prob;
struct model* model_;
int flag_cross_validation;
int nr_fold;
double bias;

void read_problem(const char *,const char *);

void setparameter( struct parameter & param)
{
	//int i;
	//void (*print_func)(const char*) = NULL;	// default printing to stdout

	// default values

	param.solver_type = L2R_L2LOSS_SVC_DUAL;
	
	param.C = 1;
	param.eps = INF; // see setting below
	param.p = 0.1;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;
	//flag_cross_validation = 0;
	//bias = -1;

	
	if(param.eps == INF)
	{
		switch(param.solver_type)
		{
			case L2R_LR:
			case L2R_L2LOSS_SVC:
				param.eps = 0.01;
				break;
			case L2R_L2LOSS_SVR:
				param.eps = 0.001;
				break;
			case L2R_L2LOSS_SVC_DUAL:
			case L2R_L1LOSS_SVC_DUAL:
			case MCSVM_CS:
			case L2R_LR_DUAL:
				param.eps = 0.1;
				break;
			case L1R_L2LOSS_SVC:
			case L1R_LR:
				param.eps = 0.01;
				break;
			case L2R_L1LOSS_SVR_DUAL:
			case L2R_L2LOSS_SVR_DUAL:
				param.eps = 0.1;
				break;
		}
	}
}

int train_thread(const char * input_file_name1,const char *input_file_name2,const char *model_file_name){

	const char *error_msg;
	read_problem(input_file_name1,input_file_name2);
	setparameter(param);
	error_msg = check_parameter(&prob,&param);

	if(error_msg)
	{
		fprintf(stderr,"ERROR: %s\n",error_msg);
		exit(1);
	}

	model_=train(&prob, &param);
	if(save_model(model_file_name, model_))
	{
		fprintf(stderr,"can't save model to file %s\n",model_file_name);
		exit(1);
	}
	free_and_destroy_model(&model_);
	destroy_param(&param);
	free(prob.y);
	free(prob.x);
	free(x_space);
	free(line);

	return 0;
}

// read in a problem (in libsvm format)
void read_problem(const char *filename1,const char *filename2)
{
	int max_index, inst_max_index, i;
	size_t elements, j;
	FILE *fp[2];
	fp[0] = fopen(filename1,"r");
	fp[1] = fopen(filename2,"r");
	char *endptr;
	char *idx, *val, *label;

	if(fp[0] == NULL)
	{
		fprintf(stderr,"can't open input file %s\n",filename1);
		exit(1);
	}
	if(fp[1] == NULL)
	{
		fprintf(stderr,"can't open input file %s\n",filename2);
		exit(1);
	}

	prob.l = 0;
	elements = 0;
	int leng[2];
	leng[0]=leng[1]=0;
	max_line_len = 1024;
	line = Malloc(char,max_line_len);
	for(i=0;i<2;i++){
		while(readline(fp[i])!=NULL)
		{
			char *p = strtok(line," \t"); // label

			// features
			while(1)
			{
				p = strtok(NULL," \t");
				if(p == NULL || *p == '\n') // check '\n' as ' ' may be after the last feature
					break;
				elements++;
			}
			elements++; // for bias term
			prob.l++;
			leng[i]++;
		}
		rewind(fp[i]);
	}

	prob.bias=bias;

	prob.y = Malloc(double,prob.l);
	prob.x = Malloc(struct feature_node *,prob.l);
	x_space = Malloc(struct feature_node,elements+prob.l);

	max_index = 0;
	j=0;
	int k=0;
	for(i=0;i<prob.l;i++)
	{
		inst_max_index = 0; // strtol gives 0 if wrong format
		if(leng[0]==0){
			readline(fp[1]);
			leng[1]--;
		}
		else if(leng[1]==0){
			readline(fp[0]);
			leng[0]--;
		}else{
			k=rand()%2;
			if(k==0){
				readline(fp[0]);
				leng[0]--;
			}else{
				readline(fp[1]);
				leng[1]--;
			}
		}


		prob.x[i] = &x_space[j];
		label = strtok(line," \t\n");
		if(label == NULL) // empty line
			exit_input_error(i+1);

		prob.y[i] = strtod(label,&endptr);
		if(endptr == label || *endptr != '\0')
			exit_input_error(i+1);

		while(1)
		{
			idx = strtok(NULL,":");
			val = strtok(NULL," \t");

			if(val == NULL)
				break;

			errno = 0;
			x_space[j].index = (int) strtol(idx,&endptr,10);
			if(endptr == idx || errno != 0 || *endptr != '\0' || x_space[j].index <= inst_max_index)
				exit_input_error(i+1);
			else
				inst_max_index = x_space[j].index;

			errno = 0;
			x_space[j].value = strtod(val,&endptr);
			if(endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr))){
				printf("%s\n",endptr);
				printf("%d\n",(int)*endptr);
				exit_input_error(i+1);			
			}

			++j;
		}

		if(inst_max_index > max_index)
			max_index = inst_max_index;

		if(prob.bias >= 0)
			x_space[j++].value = prob.bias;

		x_space[j++].index = -1;
	}

	if(prob.bias >= 0)
	{
		prob.n=max_index+1;
		for(i=1;i<prob.l;i++)
			(prob.x[i]-2)->index = prob.n;
		x_space[j-2].index = prob.n;
	}
	else
		prob.n=max_index;

	fclose(fp[0]);
	fclose(fp[1]);
}

int main(int argc,char ** argv){
	// trainParallel nump numn train train 
	int  all;
	char posName[80], negName[80], modName[80];
	int nump=0,numn=0;
	nump=atoi(argv[1]);
	numn=atoi(argv[2]);
	timeval starttime,endtime;
	gettimeofday(&starttime,0);
	
	for(int mpid=0;mpid<nump*numn;mpid++){
		int p=mpid/numn;
		int n=mpid%numn;
		sprintf(posName,"%s_p_%d.txt",argv[3],p);
		sprintf(negName,"%s_n_%d.txt",argv[3],n);
		sprintf(modName,"%s%d%d.model",argv[4],p,n);
		
		train_thread(posName,negName,modName);
		
	}
	gettimeofday(&endtime,0);
		double timeuse = 1000000*(endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
		timeuse /=1000;

		cout<<"trainSerail use time "<<timeuse<<" ms."<<endl;
	return 0;
}