#include "dump.h"
#include <stack>
#include <string>

using namespace std;

extern int searchPC(unsigned int pc);

//#define GTMEP
//#define GALL
#define GALL

int main(int argc, char*argv[])
{

	unsigned int pc;
	char op[150];
	char opr[100];
	char temp[300000];
	int i;
	unsigned int cr3;

	FILE * file, * file1;

	char funName[150], oldFunName[150];
 	char fname[200], oldfname[200];
	unsigned int oldcr3 = 0;
	unsigned int l = 0;
   
#ifdef GALL
	char * filename = argv[1];
	read1(filename);
	
    char * log = argv[2];
	file = fopen(log,"r");	
	if(file == NULL){
		perror("error");
		return 1;
	}
	
	fgets(temp,300,file);	
	while(fgets(temp,300,file)){
		sscanf(temp,"%x\t%x",&cr3,&pc);
		printf("%x\t",cr3);
		searchPC(pc);
		printf("\t%s",temp+22);
	}
	fclose(file);

#endif

#ifdef GTEMP
	file = fopen("temp","w");
	file1 = fopen("all","r");

	if(file1 == NULL){
		perror("error");
		return 1;
	}


	while(fgets(temp,300000,file1)){	

		sscanf(temp,"%x\t%x\t%s\t%d\t%s",&cr3,&pc,funName,&i,fname);
		
		if(cr3 == 1)
			printf("here");

		if(pc < 0xc1000000)
			continue;

	/*	if(strcmp(funName,oldFunName) == 0 && strcmp(fname,oldfname) == 0){
			continue;
		}*/

		l++;

		if(oldcr3 == cr3)
			continue;
		oldcr3 = cr3;
		fprintf(file,"%u\t%x\t%x\t%d\t%s\t%s\n",l,cr3,pc,i,funName,fname);
		//l++;
		//strcpy(oldFunName,funName);
		//strcpy(oldfname,fname);
	}
	fclose(file);
#endif


#ifdef GTRACE

	stack<string> st;
	stack<string> tmp;
	file = fopen("all","r");
	file1 = fopen("traceAll","w");
	
	if(file == NULL){
		perror("error");
		return 1;
	}

//	int layer = 1;
//	strcpy(oldFunName,"NO");
	
	int ifcall = 0;
	while(fgets(temp,300000,file)){
		sscanf(temp,"%x\t%x\t%s\t%u\t%s\t%s",&cr3,&pc,funName,&l,fname,op);

		if(pc < 0xc0000000)
			continue;

		if(strcmp(op,"call") == 0){
			ifcall = 1;
			//st.push(temp);
		}
		else{
			if(ifcall == 1){
				st.push(temp);
				ifcall = 0;
			}
			if(strcmp(op,"ret") == 0 || strcmp(op,"iret") == 0){
				if(st.size() > 0){
					char tempFunName[2000];
					unsigned int cr3,pc;
					strcpy(temp,st.top().c_str());
					sscanf(temp,"%x\t%x\t%s",&cr3,&pc,tempFunName);
				
					if(strcmp(tempFunName,funName) == 0)
						st.pop();
				}
			}
		}


		if(pc == 0xc101ebe5 || pc == 0xc1025d2f|| pc == 0xc10bef7f){
			while(st.size() > 0){
				tmp.push(st.top());
				st.pop();
			}

			while(tmp.size() > 0){
				string temp = tmp.top();
				fprintf(file1,"%s",temp.c_str());
				tmp.pop();
			}

			fprintf(file1,"CHECK_IT_OUT!");
			fprintf(file1,"\n");
			fprintf(file1,"\n");
			fprintf(file1,"\n");
			fprintf(file1,"\n");
		}

		/*if(strcmp(funName,oldFunName) == 0)
			continue;

		if(strcmp(op,"ret") == 0 || strcmp(op,"iret") == 0){
			
		}
		
		fprintf(file1,"%s\t%s\n",funName,fname,++i);
		strcpy(oldFunName,funName);*/
	}

//void printFun(FILE *ifile, FILE *ofile, int tag){
//}



#endif




	return 0;
}
