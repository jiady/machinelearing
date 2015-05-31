#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <fstream>
#include <vector>
#include <utility>
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))
#define INF HUGE_VAL

using namespace std;
int LAYERNUM=0;

struct node {
	vector<double> w;

};
vector<node>  nd;
node outnode;
int prob_l;
vector<double> labely;
vector<vector<pair<double,double> > > problem;
vector<vector<pair<double,double> > > test;

int max_dimension;
void read_problem(const char *filename,vector<vector<pair<double,double> > > & prob);
void train();

void exit_input_error(int line_num,int sit=0)
{
	fprintf(stderr,"Wrong input format at line %d with sit:%d \n", line_num,sit);
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

void initialize(){
	for(int i=0;i<LAYERNUM;i++){
		nd.at(i).w.resize(max_dimension+1);
		//cout<<"max"<<max_dimension<<endl;
		for(int j=1;j<=max_dimension;j++){
			nd.at(i).w.at(j)=1;
		}
		outnode.w.at(i)=double(rand())/RAND_MAX;
		//cout<<outnode.w.at(i)<<endl;
	}

}
inline double v(node & n,vector<pair<double,double> > & w){
	double sum=0;
	for(int i=0;i<w.size();i++){
		//cout<<i<<endl;
		//cout<<w.at(i).first<<endl;
		//cout<<n.w.size()<<endl;
		sum+=n.w.at(w.at(i).first)*w.at(i).second;
	}
	return sum;
}
inline  double vout(vector<double> & inner){
	double sum=0;
	for(int i=0;i<LAYERNUM;i++){
		sum+=outnode.w.at(i)*inner.at(i);
	}
	return sum;
}


inline double f(double x){
	return double(2)/(1+exp(-x))-1;
}
inline double fp(double x){
	return pow(f(x),2)*exp(-x)*2;
}
void train(){
	int max_iteration=1;
	double learn_rate=0.8;
	vector<double> inner_res(LAYERNUM);
	vector<double> origin_res(LAYERNUM);
	double outter_res;
	for(int i=0;i<max_iteration;i++){
		
		for(int p=0;p<problem.size();p++){
			for(int j=0;j<LAYERNUM;j++){
				//cout<<102<<endl;
				origin_res.at(j)=v(nd.at(j),problem.at(p));
				//cout<<104<<endl;
				inner_res.at(j)=f(origin_res.at(j));
				//cout<<105<<endl;
				//if(i+p==0)
					//cout<<origin_res.at(j)<<endl;
			}
			outter_res=vout(inner_res);
			double e=labely.at(p)-f(outter_res);
			if(p==0) cout<<e<<endl;
			double outer_local=e*fp(outter_res);
			//if(p==0) cout<<outer_local<<endl;
			for(int j=0;j<LAYERNUM;j++){
				outnode.w.at(j)+=learn_rate*outer_local*inner_res.at(j);
				
			}
			for(int j=0;j<LAYERNUM;j++){
				// if(p+j==0){
				// 	cout<<origin_res.at(j)<<endl;
				// 	cout<<fp(origin_res.at(j))<<endl;
				// 	cout<<outer_local<<endl;
				// 	cout<<outnode.w.at(j)<<endl;
				// }
				double local=fp(origin_res.at(j))*outer_local*outnode.w.at(j);
				//if(p==0) cout<<local<<endl;
				for(int l=0;l<problem.at(p).size();l++){
					nd.at(j).w[problem.at(p).at(l).first]+=learn_rate*local*problem.at(p).at(l).second;
					// if(j+p==0){
					// 	cout<<"index:"<<problem.at(p).at(l).first<<endl;
					// 	cout<<nd.at(j).w[problem.at(p).at(l).first]<<endl;
					// }
				}
				// if(p<10&& j==0){
				// 	cout<<nd.at(j).w[0]<<endl;
				// }
			}
		}
	}
}

double predicate(vector<pair<double,double> > & test){
	vector<double> inner_res(LAYERNUM);
	for(int j=0;j<LAYERNUM;j++){
		inner_res.at(j)=f(v(nd.at(j),test));
	}
	return f(vout(inner_res));
}

void predict(const char *filename){
	FILE *fp = fopen(filename,"w");
	for(int i=0;i<test.size();i++){
		double k=predicate(test.at(i));
		fprintf(fp, "%f\n",k);
	}
	fclose(fp);
}



void read_problem(const char *filename,vector<vector<pair<double,double> > > &problem){
	int max_index, inst_max_index, i;
	size_t elements, j;
	FILE *fp = fopen(filename,"r");
	char *endptr;
	char *idx, *val, *label;

	if(fp == NULL)
	{
		fprintf(stderr,"can't open input file %s\n",filename);
		exit(1);
	}

	prob_l = 0;
	elements = 0;
	max_line_len = 1024;
	line = Malloc(char,max_line_len);
	while(readline(fp)!=NULL)
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
		prob_l++;
	}
	rewind(fp);
	problem.resize(prob_l);
	max_index = 0;
	j=0;
	for(i=0;i<prob_l;i++)
	{
		inst_max_index = 0; // strtol gives 0 if wrong format
		readline(fp);
		
		label = strtok(line," \t\n");
		if(label == NULL) // empty line
			exit_input_error(i+1,0);

		labely.push_back( strtod(label,&endptr));
		if(endptr == label || *endptr != '\0')
			exit_input_error(i+1,1);

		while(1)
		{
			pair<double ,double> tmpv;
			idx = strtok(NULL,":");
			val = strtok(NULL," \t");


			if(val == NULL)
				break;

			errno = 0;
			tmpv.first= (int) strtol(idx,&endptr,10);
			if(endptr == idx || errno != 0 || *endptr != '\0' || tmpv.first <= inst_max_index)
				exit_input_error(i+1,2);
			else
				inst_max_index = tmpv.first;

			errno = 0;
			tmpv.second = strtod(val,&endptr);
			//cout<<tmpv.second<<endl;
			if(endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr))){
				printf("%s\n",endptr);
				printf("%d\n",(int)*endptr);
				exit_input_error(i+1,3+(int)j);			
			}
			problem.at(i).push_back(tmpv);
		}

		problem.at(i).push_back(make_pair(0,1));
		if(inst_max_index > max_index)
			max_index = inst_max_index;
	}
	max_dimension=max_index+1;
	for(i=0;i<prob_l;i++){
		problem.at(i).back().first=max_dimension;
		//cout<<problem.at(i)[0].first<<":"<<problem.at(i)[0].second<<endl;
	}
	free(line);
	fclose(fp);
}

int main(int argc,char **argv){
	//usage main train test result 59
	
	ifstream ftest(argv[2]);
	ofstream fout(argv[3]);
	LAYERNUM=atoi(argv[4]);
	nd.resize(LAYERNUM);
	outnode.w.resize(LAYERNUM);
	
	read_problem(argv[1],problem);
	initialize();
	train();
	cout<<"train over"<<endl;
	read_problem(argv[2],test);
	cout<<"read test"<<endl;
	predict(argv[3]);
	cout<<"over"<<endl;
	return 0;
}