#include <stdio.h>
#include <stdlib.h>

#define LMAX 20
#define WMAX 1024


int layers,width[LMAX];


int getwidths(char *widthfile)
	{
	FILE *fp;
	int l;
	
	fp=fopen(widthfile,"r");
	
	if(!fp)
		{
		printf("widthfile not found\n");
		return 0;
		}
	
	fscanf(fp,"%d",&layers);
	
	for(l=0;l<=layers;++l)
		fscanf(fp,"%d",&width[l]);
	
	fclose(fp);
	
	return 1;
	}
	
		
void printnet(char *netfile)
	{
	FILE *fp;
	int innodes,outnodes,nodes,edges;
	int l,out,in,i,j;
	
	innodes=1+width[0];
	outnodes=width[layers];
	
	nodes=1;
	for(l=0;l<=layers;++l)
		nodes+=width[l];
	
	edges=nodes-innodes;
	for(l=1;l<=layers;++l)
		edges+=width[l-1]*width[l];
	
	fp=fopen(netfile,"w");
	
	fprintf(fp,"%d  %d  %d  %d\n",nodes,innodes,outnodes,edges);
				
	out=1;
	in=out+width[0];
	
	for(l=1;l<=layers;++l)
		{
		for(i=0;i<width[l];++i)
			{	
			fprintf(fp,"%5d%5d%14.8lf\n",i+in,0,.0);
			
			for(j=0;j<width[l-1];++j)
				fprintf(fp,"%5d%5d%14.8lf\n",i+in,j+out,.0);
			}
			
		out+=width[l-1];
		in+=width[l];
		}
		
	fclose(fp);
	}
	
	
int main(int argc,char* argv[])
	{
	char *widthfile,*netfile;
	
	if(argc==3)
		{
		widthfile=argv[1];
		netfile=argv[2];
		}
	else
		{
		printf("expected 2 arguments: widthfile netfile\n");
		return 1;
		}
		
	if(!getwidths(widthfile))
		return 0;
	
	printnet(netfile);
		
	return 1;
	}
		
		
		
		