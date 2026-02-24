#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "net.h"


int getnet()
	{
	int n,e,in,out;
	FILE *fp;

	// if netfile argument is -, read from stdin, otherwise from file
	char *buf = NULL;
	if (0 == strcmp(netfile, "-"))
		{
		// read stdin into a buffer, as we make multiple basses
        size_t bufsize = 8192, len = 0;
        buf = malloc(bufsize);
        int c;
        while ((c = fgetc(stdin)) != EOF)
			{
            if (len + 1 >= bufsize) { bufsize *= 2; buf = realloc(buf, bufsize); }
            buf[len++] = c;
        	}
        buf[len] = '\0';
        fp = fmemopen(buf, len, "r");
		} 
	else 
		{
		fp=fopen(netfile,"r");
		if(!fp)
			{
			printf("netfile not found\n");
			return 0;
			}
		}
	
	fscanf(fp,"%d%d%d%d",&nodes,&innodes,&outnodes,&edges);
	
	indeg=malloc(nodes*sizeof(int));
	outdeg=malloc(nodes*sizeof(int));
	
	wnet=malloc(edges*sizeof(double));
	
	yinfer=malloc(nodes*sizeof(double));
	
	for(n=0;n<nodes;++n)
		{
		indeg[n]=0;
		outdeg[n]=0;
		}
	
	for(e=0;e<edges;++e)
		{
		fscanf(fp,"%d%d%lf",&in,&out,&wnet[e]);
		
		++indeg[in];
		++outdeg[out];
		}
	
	
	inedge=malloc(nodes*sizeof(int*));
	outedge=malloc(nodes*sizeof(int*));
	
	innode=malloc(edges*sizeof(int));
	outnode=malloc(edges*sizeof(int));
	
	for(n=0;n<nodes;++n)
		{
		inedge[n]=malloc(indeg[n]*sizeof(int));
		outedge[n]=malloc(outdeg[n]*sizeof(int));
		}
		
	// second pass down file
	rewind(fp);
	fscanf(fp,"%*d%*d%*d%*d");
	
	for(n=0;n<nodes;++n)
		{
		indeg[n]=0;
		outdeg[n]=0;
		}
		
	for(e=0;e<edges;++e)
		{
		fscanf(fp,"%d%d%*f",&in,&out);
		
		inedge[in][indeg[in]]=e;
		++indeg[in];
		
		outedge[out][outdeg[out]]=e;
		++outdeg[out];
		
		innode[e]=in;
		outnode[e]=out;
		}
		
	fclose(fp);
	free(buf);
	return 1;
	}

/*	
void infer(double *winfer)
	{
	int n,d,e,out;
	double dot;
	
	for(n=innodes;n<nodes;++n)
		{
		dot=0.;
			
		for(d=0;d<indeg[n];++d)
			{
			e=inedge[n][d];
			out=outnode[e];
				
			dot+=winfer[e]*yinfer[out];
			}
				
		yinfer[n]=dot>0. ? 1. : -1.;
		}
*/		
		
	void infer(double *winfer)
	{
	int n,d,e,out;
	double dot,ysgn;
	
	for(n=innodes;n<nodes;++n)
		{
		dot=0.;
			
		for(d=0;d<indeg[n];++d)
			{
			e=inedge[n][d];
			out=outnode[e];
			ysgn=yinfer[out]>0. ? 1. : -1.;	
			
			dot+=winfer[e]*ysgn;
			}
				
		yinfer[n]=dot;
		}
	}
	
	
