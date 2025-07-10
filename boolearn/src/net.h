#define WMAX 1024

int nodes,innodes,outnodes,edges;
int *indeg,*outdeg,**inedge,**outedge,*innode,*outnode;

char *netfile;
double *wnet,*yinfer;

int getnet();
void infer(double*);