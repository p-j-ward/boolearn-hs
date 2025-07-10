#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define LMAX 20
#define WMAX 1024


int layers,width[LMAX];
double w0,weight[WMAX],andorfrac;


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
	

double urand()
	{
	return ((double)rand())/RAND_MAX;
	}
	
	
int randsgn()
	{
	return 2*(rand()%2)-1;
	}
	
	
void andorweight(int inputs)
	{
	int i,r1,r2;
	
	for(i=0;i<inputs;++i)
		weight[i]=0.;
	
	if(urand()>andorfrac)
		{
		w0=0.;
		
		r1=rand()%inputs;
		
		weight[r1]=randsgn()*sqrt((inputs+1.)/1.);
		}
	else
		{
		w0=randsgn()*sqrt((inputs+1.)/3.);
		
		r1=rand()%inputs;
		
		weight[r1]=randsgn()*sqrt((inputs+1.)/3.);
		
		do	{
			r2=rand()%inputs;
			}
		while(r2==r1);
		
		weight[r2]=randsgn()*sqrt((inputs+1.)/3.);
		}
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
			andorweight(width[l-1]);
				
			fprintf(fp,"%5d%5d%14.8lf\n",i+in,0,w0);
			
			for(j=0;j<width[l-1];++j)
				fprintf(fp,"%5d%5d%14.8lf\n",i+in,j+out,weight[j]);
			}
			
		out+=width[l-1];
		in+=width[l];
		}
		
	fclose(fp);
	}
	
	
int main(int argc,char* argv[])
	{
	char *widthfile,*netfile;
	
	if(argc==4)
		{
		andorfrac=atof(argv[1]);
		widthfile=argv[2];
		netfile=argv[3];
		}
	else
		{
		printf("expected 3 arguments: andorfrac widthfile netfile\n");
		return 1;
		}
		
	if(!getwidths(widthfile))
		return 0;
		
	srand(time(NULL));
	
	printnet(netfile);
		
	return 1;
	}
		
		
		
		