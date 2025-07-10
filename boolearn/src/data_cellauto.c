#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXRULEBITS 64
#define MAXSIZE 1024


int inbits,rulebits,rulebit[MAXRULEBITS],size;


void makerulebits(int rule)
	{
	int n,r;
	
	n=rule;
	
	for(r=0;r<rulebits;++r)
		{
		rulebit[r]=n%2;
		n/=2;
		}
	}
	
	
int wrap(int p)
	{
	if(p<0)
		return p+size;
	else if(p>=size)
		return p-size;
	else
		return p;
	}
	
	
void step(int *state)
	{
	int p,q,r,in,next[MAXSIZE];
	
	for(p=0;p<size;++p)
		{
		r=0;
		for(in=0;in<inbits;++in)
			{
			q=wrap(p+inbits-1-in);
			
			if(state[q])
				r+=1<<in;
			}
		
		next[wrap(p+inbits/2)]=rulebit[r];
		}
	
	for(p=0;p<size;++p)
		state[p]=next[p];
	}
	
	
int main(int argc,char* argv[])
	{
	char *id,datafile[20];
	int rule,i,items,p,state[MAXSIZE],s,steps;
	FILE *fp;
	
	if(argc==7)
		{
		inbits=atoi(argv[1]);
		rule=atoi(argv[2]);
		size=atoi(argv[3]);
		steps=atoi(argv[4]);
		items=atoi(argv[5]);
		id=argv[6];
		}
	else
		{
		printf("expected 6 arguments: inbits rule size steps items id\n");
		return 1;
		}
	
	rulebits=1<<inbits;
	
	makerulebits(rule);
	
	sprintf(datafile,"%s.dat",id);
	
	srand(time(NULL));

	fp=fopen(datafile,"w");
	fprintf(fp,"%d\n%d %d\n%d %d\n\n",items,2,size,2,size);
	
	for(i=0;i<items;++i)
		{
		for(p=0;p<size;++p)
			{
			state[p]=rand()%2;
			
			fprintf(fp,"%2d",state[p]);
			}
			
		fprintf(fp,"\n");
		
		for(s=0;s<steps;++s)
			step(state);
		
		for(p=0;p<size;++p)
			fprintf(fp,"%2d",state[p]);
			
		fprintf(fp,"\n\n");
		}
		
	fclose(fp);
	
	return 0;
	}
	
	