#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "linear.h"
#include <stdio.h>
#define MAX 210000
#define MIN -210000

int print_null(const char *s,...) {return 0;}

static int (*info)(const char *fmt,...) = &printf;

struct feature_node *x;
int max_nr_attr = 64;

struct model* model_;
struct model* models[20][20];
int flag_predict_probability=0;

void exit_input_error(int line_num)
{
	fprintf(stderr,"Wrong input format at line %d\n", line_num);
	exit(1);
}

static char *line = NULL;
static int max_line_len;
int minnum,maxnum;
double min_result[20];
double minmax(const struct feature_node *x){
	int i,j;
	double result = MIN;
	double temp;
	for( i = 0; i < maxnum; ++i )
		min_result[i] = MAX;

	for( i = 0; i < maxnum; ++i )//3
	{
		for( j = 0; j < minnum; ++j )//9
		{
			temp = predict(models[i][j], x);
			min_result[i] = (temp < min_result[i]) ? temp : min_result[i];
		}
		result = (min_result[i] > result) ? min_result[i] : result;
	}
	return result;
}

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

void do_predict(FILE *input, FILE *output)
{
	int correct = 0;
	int total = 0;
	double error = 0;
	double sump = 0, sumt = 0, sumpp = 0, sumtt = 0, sumpt = 0;
	int TP = 0, TN = 0, FP = 0, FN = 0;
	


	int nr_class=get_nr_class(model_);
	double *prob_estimates=NULL;
	int j, n;
	int nr_feature=get_nr_feature(model_);
	if(model_->bias>=0)
		n=nr_feature+1;
	else
		n=nr_feature;

	if(flag_predict_probability)
	{
		int *labels;

		if(!check_probability_model(model_))
		{
			fprintf(stderr, "probability output is only supported for logistic regression\n");
			exit(1);
		}

		labels=(int *) malloc(nr_class*sizeof(int));
		get_labels(model_,labels);
		prob_estimates = (double *) malloc(nr_class*sizeof(double));
		fprintf(output,"labels");
		for(j=0;j<nr_class;j++)
			fprintf(output," %d",labels[j]);
		fprintf(output,"\n");
		free(labels);
	}

	max_line_len = 1024;
	line = (char *)malloc(max_line_len*sizeof(char));

	while(readline(input) != NULL)
	{
		int i = 0;
		double target_label, predict_label;
		char *idx, *val, *label, *endptr;
		int inst_max_index = 0; // strtol gives 0 if wrong format

		label = strtok(line," \t\n");
		if(label == NULL) // empty line
			exit_input_error(total+1);

		target_label = strtod(label,&endptr);
		if(endptr == label || *endptr != '\0')
			exit_input_error(total+1);

		while(1)
		{
			if(i>=max_nr_attr-2)	// need one more for index = -1
			{
				max_nr_attr *= 2;
				x = (struct feature_node *) realloc(x,max_nr_attr*sizeof(struct feature_node));
			}

			idx = strtok(NULL,":");
			val = strtok(NULL," \t");

			if(val == NULL)
				break;
			errno = 0;
			x[i].index = (int) strtol(idx,&endptr,10);
			if(endptr == idx || errno != 0 || *endptr != '\0' || x[i].index <= inst_max_index)
				exit_input_error(total+1);
			else
				inst_max_index = x[i].index;

			errno = 0;
			x[i].value = strtod(val,&endptr);
			if(endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr)))
				exit_input_error(total+1);

			// feature indices larger than those in training are not used
			if(x[i].index <= nr_feature)
				++i;
		}

		if(model_->bias>=0)
		{
			x[i].index = n;
			x[i].value = model_->bias;
			i++;
		}
		x[i].index = -1;

		if(flag_predict_probability)
		{
			int j;
			predict_label = predict_probability(model_,x,prob_estimates);
			fprintf(output,"%g",predict_label);
			for(j=0;j<model_->nr_class;j++)
				fprintf(output," %g",prob_estimates[j]);
			fprintf(output,"\n");
		}
		else
		{
			predict_label = minmax(x);
			//fprintf(output,"%g\n",predict_label);
			fprintf( output, "%g\t%g\n",predict_label,target_label);
			
		}

		if(predict_label *target_label>0)
			++correct;
		error += (predict_label-target_label)*(predict_label-target_label);
		sump += predict_label;
		sumt += target_label;
		sumpp += predict_label*predict_label;
		sumtt += target_label*target_label;
		sumpt += predict_label*target_label;

		
		if( predict_label > 0 && target_label > 0 ) {
			TP++;
		}
		else if( predict_label > 0 && target_label < 0 ){ 
			FP++;
		}
		else if( predict_label < 0 && target_label < 0 ){ 
			TN++;
		}
		else if( predict_label < 0 && target_label > 0 ){ 
			FN++;
		}
		

		++total;
	}

	if(model_->param.solver_type==L2R_L2LOSS_SVR ||
	   model_->param.solver_type==L2R_L1LOSS_SVR_DUAL ||
	   model_->param.solver_type==L2R_L2LOSS_SVR_DUAL)
	{
		info("Mean squared error = %g (regression)\n",error/total);
		info("Squared correlation coefficient = %g (regression)\n",
			((total*sumpt-sump*sumt)*(total*sumpt-sump*sumt))/
			((total*sumpp-sump*sump)*(total*sumtt-sumt*sumt))
			);
	}
	else
	{
		info("Accuracy = %g%% (%d/%d)\n",(double) correct/total*100,correct,total);
		info("TP = %d\nFP = %d\nTN = %d\nFN = %d\n",TP,FP,TN,FN);
		info("TPR = %g\nFPR = %g\n", (double)TP/(TP+FN), (double)FP/(FP+TN));
		info("F1 = %g\n", (double)TP*2/(2*TP+FP+FN));
	}
	if(flag_predict_probability)
		free(prob_estimates);
}

void exit_with_help()
{
	printf(
	"Usage: predict [options] test_file model_file output_file\n"
	"options:\n"
	"-b probability_estimates: whether to output probability estimates, 0 or 1 (default 0); currently for logistic regression only\n"
	"-q : quiet mode (no outputs)\n"
	);
	exit(1);
}

int main(int argc, char **argv)
{
	FILE *input, *output;
	//minmaxPredict #min #max input model output
	//jdy
	// parse options
	
	
	maxnum=atoi(argv[1]);//3
	minnum=atoi(argv[2]);
	

	input = fopen(argv[3],"r");
	if(input == NULL)
	{
		fprintf(stderr,"can't open input file %s\n",argv[3]);
		exit(1);
	}

	output = fopen(argv[5],"w");
	if(output == NULL)
	{
		fprintf(stderr,"can't open output file %s\n",argv[3+2]);
		exit(1);
	}
	char post[30]="00.model";
	for(int i=0;i<maxnum;i++){
		for(int j=0;j<minnum;j++){
			post[0]=(char)'0'+i;
			post[1]=(char)'0'+j;
			char md[80]={0};
			strcat(md,argv[4]);
			strcat(md,post);
			//printf("%s\n", md);
			if(i==0 && j==0){
				model_=load_model(md);
			}
			if((models[i][j]=load_model(md))==0)
			{
				fprintf(stderr,"can't open model file %s\n",md);
				exit(1);
			}
		}
	}
	

	x = (struct feature_node *) malloc(max_nr_attr*sizeof(struct feature_node));
	do_predict(input, output);
	for(int i=0;i<maxnum;i++){
		for(int j=0;j<minnum;j++){
			free_and_destroy_model(&models[i][j]);
		}
	}
	free_and_destroy_model(&model_);
	free(line);
	free(x);
	fclose(input);
	fclose(output);
	return 0;
}

