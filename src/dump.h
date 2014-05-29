#ifndef DUMP_H
#define DUMP_H

#include <stdio.h>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <map>

using namespace std;

inline int compare (const void * a, const void * b)
{
	return ( *(unsigned int*)a - *(unsigned int*)b );
}


struct Line
{
	unsigned int inst;
	int line;
	char fname[200];
	char funName[150];	
};

struct Die
{
	unsigned int start;
	unsigned int end;
	char funName[150];
	char fname[200];
};

struct NewLine
{
	unsigned int inst;
	int line;
};

struct NewDie
{
	unsigned int start;
	unsigned int end;
	char funName[150];
	char fname[200];
	NewLine *lines;
	int lineNum;
};


//int searchPC(unsigned int pc);

void read1(const char* file);

#endif
