/*
Author: Junyuan Zeng
*/

#include "dump.h"


using namespace std;

NewDie *ndlist;
NewLine buffer[100000];
int ndNum;

struct Elem
{
	char funName[150];
	char fname[250];
	int line;

	Elem(char *funName,char *fname,int line)
	{
		strcpy(this->funName,funName);
		strcpy(this->fname,fname);
		this->line = line;
	}
};


void read1(const char* file)
{
	FILE * pFile;
	unsigned int inst;
	char funName[150];
	int line;
	char fname[250];
	char oneLine[1000];
	char oldfname[250];
	char oldfunName[150];

	if((pFile = fopen(file,"r")) == NULL)
	{
		perror("error");
		return;
	}
	
	int count = 0;
	while(fgets(oneLine,1000,pFile)){
		if(oneLine[0] == '0')
			count++;
	}
	//count++;
	ndNum = count;
	fseek(pFile,0,SEEK_SET);	


	ndlist = (NewDie*) malloc(count*sizeof(NewDie));
	int i = 0;
	while(fscanf(pFile,"%x\t%s\t%d\t%s\n",&inst,funName,&line,fname) > 0){
			(ndlist+i)->start = inst;
			strcpy((ndlist+i)->funName,funName);
			strcpy((ndlist+i)->fname,fname);
		int j = 0;
		do{
			if(inst == 0 && strcmp(funName,"New_Function_Start") == 0){
				break;
			}
			buffer[j].inst = inst;
			buffer[j].line = line;
			j++;
		}while(fscanf(pFile,"%x\t%s\t%d\t%s\n",&inst,funName,&line,fname) > 0);
		(ndlist+i)->lines = (NewLine*)malloc(j*sizeof(NewLine));
		memcpy((ndlist+i)->lines,buffer,sizeof(NewLine)*j);
		(ndlist+i)->lineNum = j;
		i++;
	}

	i = 0;
	for(;i < count-1;i++){
		(ndlist+i)->end = (ndlist+i+1)->start;
	}
	(ndlist+i)->end = 0xffffffff;

	qsort(ndlist,count,sizeof(NewDie),compare);
	
	fclose(pFile);
}

int searchPC(unsigned int pc)
{	
	static int upper = -1, lower = -1, index;
	char funName[150],fname[300];
	int line;


	//Special Case:
	/*if(pc >= 0xc1473ec8 && pc <= 0xc147419d)
	{
		printf("/arch/x86/kernel/head_32.S\n");
		return 0;
	}*/
	//
	int begin = -1, end = ndNum, mid;
	//if(upper > pc && pc >= lower){
		do{
			mid = (begin + end) / 2; 
			if((ndlist+mid)->start <= pc && (ndlist+mid)->end > pc){
				upper = (ndlist+mid)->end;
				lower = (ndlist+mid)->start;
				index = mid;
				break;
			}
			else if((ndlist+mid)->start > pc){
				end = mid;
			}
			else{
				begin = mid;
			}			
		}while(end - begin > 1);
	//}

	if(end - begin <= 1){
		return -1;
	}

	
	strcpy(funName,(ndlist+mid)->funName);
	strcpy(fname,(ndlist+mid)->fname);
	
	NewLine * nl = (ndlist+mid)->lines;
	for(int i = 0;i < (ndlist+mid)->lineNum;i++){
		if(nl[i].inst == pc){
			line = nl[i].line;
			break;
		}
		if(i+1 == (ndlist+mid)->lineNum){
			line = nl[i].line;
			break;
		}
		if(nl[i].inst <  pc && pc < nl[i+1].inst){
			line = nl[i].line;
			break;
		}
	}
	printf("%x\t%s\t%d\t%s",pc,funName,line,fname);

	return 0;
}
