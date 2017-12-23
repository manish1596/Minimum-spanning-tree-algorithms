#include <iostream>
#include <limits.h>
#include <queue>
#define VERTEX 9
#define EDGE 14


using namespace std;

/////////////////////FOR DISJOINT DATA SETS////////////////////
struct node
{
    node* p;
    int key;
    int r;
};

struct disjoint_sets
{
    node** disj;
};

disjoint_sets* create_disj_sets(int V)
{
    disjoint_sets* n=new disjoint_sets;
    n->disj=new node*[V];
    return n;
}

node* make_set(int k)
{
    node* n=new node;
    n->key=k;
    n->r=0;
    n->p=n;
    return n;
}

node* find_set(node* x)
{
    if(x!=x->p)
        x->p=find_set(x->p);
    return x->p;
}

void link(node* x, node* y)
{
    if((x->r)>(y->r))
    {
        y->p=x;
    }
    else
    {
        x->p=y;
        if((x->r)==(y->r))
            y->r++;
    }
}

void Union(node* x, node* y)
{
    link(find_set(x), find_set(y));
}

/////////////////////FOR DISJOINT DATA SETS////////////////////

/////////////////////FOR GRAPH////////////////////
struct AdjListNode
{
    int dest;

    AdjListNode* next;
};

struct AdjList
{
    char color;
    int p;
    int d;
    int f;
    AdjListNode* head;
};

struct queu
{
    AdjList* q;
    int fr;
    int re;
};

struct edge
{
    node* u;
    node* v;
    int w;
};

struct Graph
{
    int V;
    int edge_no;
    edge* E;
    AdjList* arr;
    node** disjSet;
};

AdjListNode* create_node(int x)
{
    AdjListNode* newnode;
    newnode=new AdjListNode;
    newnode->dest=x;
    newnode->next=NULL;
    return newnode;
}

Graph* create_graph(Graph* g, int v, int e)
{
    g->V=v;
    g->E=new edge[e];
    g->edge_no=0;
    g->arr=new AdjList[v];
    g->disjSet=new node*[v];
    int i;
    for(i=0; i<v; i++)
    {
        g->arr[i].head=NULL;
        g->arr[i].color='w';
        g->arr[i].p=INT_MIN;
        g->arr[i].d=INT_MAX;
        g->arr[i].f=INT_MAX;
    }
    for(i=0; i<v; i++)
    {
        g->disjSet[i]=make_set(i);
    }
    return g;
}

void reinitGraph(Graph* g)
{
    int i;
    for(i=0; i<g->V; i++)
    {
        g->arr[i].color='w';
        g->arr[i].p=INT_MIN;
        g->arr[i].d=INT_MAX;
        g->arr[i].f=INT_MAX;
    }
}

void add_edge_dir(Graph* g, int src, int dest, int weight)
{
    AdjListNode* newnode=create_node(dest);
    newnode->next=g->arr[src].head;
    g->arr[src].head=newnode;
    g->E[g->edge_no].u=g->disjSet[src];
    g->E[g->edge_no].v=g->disjSet[dest];
    g->E[g->edge_no].w=weight;
    g->edge_no++;
}

void add_edge_undir(Graph* g, int src, int dest, int weight)
{
    if(src==dest)
    {
        cout << "No self loops are possible in undirected graph.";
        return;
    }
    AdjListNode* newnode=create_node(dest);
    newnode->next=g->arr[src].head;
    g->arr[src].head=newnode;
    newnode=create_node(src);
    newnode->next=g->arr[dest].head;
    g->arr[dest].head=newnode;
    g->E[g->edge_no].u=g->disjSet[src];
    g->E[g->edge_no].v=g->disjSet[dest];
    g->E[g->edge_no].w=weight;
    g->edge_no++;
}

void printGraph(struct Graph* graph)
{
    int v;
    struct AdjListNode* pCrawl;
    for (v = 0; v < graph->V; ++v)
    {
        pCrawl = graph->arr[v].head;
        cout << "\n Adjacency list of vertex" << v << "\n head ";
        while (pCrawl)
        {
            cout << "-> " << pCrawl->dest;
            pCrawl = pCrawl->next;
        }
        cout << "\n";
    }
}




void sort_edges_by_wt(Graph* g)
{
    int i, j;
    edge temp;
    for(i=1; i<g->edge_no; i++)
    {
        j=i;
        temp=g->E[j];
        while(temp.w<g->E[j-1].w&&j>0)
        {
            g->E[j]=g->E[j-1];
            j--;
        }
        g->E[j]=temp;
    }
}

/////////////////////FOR GRAPH////////////////////

int main()
{
    /*disjoint_sets* d=create_disj_sets(10);
    for(int j=0; j<10; j++)
    {
        d->disj[j]=make_set(j);
    }
    //cout << (d->disj[2]->key);
    Union(d->disj[5], d->disj[3]);
    Union(d->disj[1], d->disj[7]);
    Union(d->disj[5], d->disj[1]);
    node* u=find_set(d->disj[5]);
    cout << u->key;*/


    int i;
    Graph* g=new Graph;
    Graph* MST=new Graph;
    create_graph(g, VERTEX, EDGE);
    create_graph(MST, VERTEX, VERTEX-1);
    add_edge_undir(g, 0, 1, 4);
    add_edge_undir(g, 1, 2, 8);
    add_edge_undir(g, 2, 3, 7);
    add_edge_undir(g, 3, 4, 9);
    add_edge_undir(g, 4, 5, 10);
    add_edge_undir(g, 5, 6, 2);
    add_edge_undir(g, 6, 7, 1);
    add_edge_undir(g, 0, 7, 8);
    add_edge_undir(g, 8, 6, 6);
    add_edge_undir(g, 2, 8, 2);
    add_edge_undir(g, 8, 7, 7);
    add_edge_undir(g, 2, 5, 4);
    add_edge_undir(g, 1, 7, 11);
    add_edge_undir(g, 5, 3, 14);
    sort_edges_by_wt(g);

    //for(int i=0; i<5; i++)
        //cout << g->E[i].w << " ";
    for(i=0; i<g->edge_no; i++)
    {
        if(find_set(g->E[i].u)!=find_set(g->E[i].v))
        {
            add_edge_undir(MST, g->E[i].u->key, g->E[i].v->key, g->E[i].w);
            Union(g->E[i].u, g->E[i].v);
        }
    }

    //printGraph(g);

    for(i=0; i<MST->edge_no; i++)
        cout << " (" << MST->E[i].u->key << "," << MST->E[i].v->key << ") ;";

    return 0;
}

