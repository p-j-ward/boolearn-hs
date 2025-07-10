#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "net.h"

	
int main(int argc,char* argv[])
	{
	FILE *fp;
	char *id,*genfile,datafile[20];
	int items,indata,i,n,bit;
	double **yin;
	
	if(argc==5)
		{
		netfile=argv[1];
		genfile=argv[2];
		items=atoi(argv[3]);
		id=argv[4];
		}
	else
		{
		printf("expected 4 arguments: netfile genfile items id\n");
		return 1;
		}
	
	if(!getnet())
		return 1;
	
	sprintf(datafile,"%s.dat",id);
	
	if(items==0)
		{
		fp=fopen(genfile,"r");
		
		if(!fp)
			{
			printf("genfile not found\n");
			
			return 1;
			}
		
		fscanf(fp,"%d%d",&items,&indata);
		
		if(1+indata!=innodes)
			{
			printf("1+indata != innodes\n");
			
			fclose(fp);
			
			return 1;
			}
			
		yin=malloc(items*sizeof(double*));
		
		for(i=0;i<items;++i)
			yin[i]=malloc(innodes*sizeof(double));
			
		for(i=0;i<items;++i)
			{
			for(n=1;n<innodes;++n)
				{
				fscanf(fp,"%d",&bit);
				
				yin[i][n]=bit ? 1. : -1.;
				}
				
			fscanf(fp,"%*d");
			}
			
		fclose(fp);
		}
	else
		{
		yin=malloc(items*sizeof(double*));
		
		for(i=0;i<items;++i)
			yin[i]=malloc(innodes*sizeof(double));
			
		srand(time(NULL));
		
		for(i=0;i<items;++i)
			{
			for(n=1;n<innodes;++n)
				yin[i][n]=rand()%2 ? 1. : -1.;
			}
		}
		
	fp=fopen(datafile,"w");
	fprintf(fp,"%d\n%d %d\n%d %d\n\n",items,2,innodes-1,2,outnodes);
			
	yinfer[0]=-1.;
	
	for(i=0;i<items;++i)
		{
		for(n=1;n<innodes;++n)
			{
			fprintf(fp,"%2d",yin[i][n]>0.);
			
			yinfer[n]=yin[i][n];
			}
			
		fprintf(fp,"\n");
		
		infer(wnet);
			
		for(n=0;n<outnodes;++n)
			fprintf(fp,"%2d",yinfer[n+nodes-outnodes]>0.);
			
		fprintf(fp,"\n\n");
		}
		
	fclose(fp);
	
	return 0;
	}
