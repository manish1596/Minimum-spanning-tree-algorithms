#include <iostream>
#include <limits.h>
#define VERTEX 9
#define EDGE 14
#define START 1

using namespace std;

/////////////////FOR IMPLEMENTING PRIORITY QUEUE//////////////////////
struct heap_node
{
    int key;
    int pr;
    int vertex_no;
};

void min_heapify(heap_node* h, int* h_r, int heapsize, int pow, int k)
{
    int i, j, temp, left, right;
    if(k<pow)
    {
        left=2*k;
        right=2*k+1;
        if(left==heapsize)
        {
            if(h[k].key>h[left].key)
            {
                h_r[h[k].vertex_no]=left;
                h_r[h[left].vertex_no]=k;
                swap(h[k], h[left]);
            }
        }
        else
        {
            if(left<heapsize)
            {
                if((h[k].key>h[left].key)||(h[k].key>h[right].key))
                {
                    if(h[left].key<h[right].key)
                    {
                        h_r[h[k].vertex_no]=left;
                        h_r[h[left].vertex_no]=k;
                        swap(h[left], h[k]);
                        min_heapify(h, h_r, heapsize, pow, left);
                    }
                    else
                    {
                        h_r[h[k].vertex_no]=right;
                        h_r[h[right].vertex_no]=k;
                        swap(h[right], h[k]);
                        min_heapify(h, h_r, heapsize, pow, right);
                    }
                }
            }
        }
    }
}

void build_min_heap(heap_node* h, int* h_r, int heapsize)
{
    int pow, temp=heapsize/2;
    pow=1;
    while(pow<=heapsize)
        pow *= 2;
    pow=pow/2;
    while(temp>0)
        {
            min_heapify(h, h_r, heapsize, pow, temp);
            temp--;
        }
}

void decrease_key(heap_node* h, int* h_r, int heapsize, int n, int new_key)
{
    int p, k;
    h[n].key=new_key;
    k=n;
    p=k/2;
    while(h[k].key<h[p].key && p>0)
    {
        h_r[h[k].vertex_no]=p;
        h_r[h[p].vertex_no]=k;
        swap(h[k], h[p]);
        k=p;
        p=p/2;
    }
}

heap_node extract_min(heap_node* h, int* h_r, int heapsize)
{
    int pow;
    heap_node temp=h[1];
    h[1]=h[heapsize];
    pow=1;
    while(pow<=(heapsize-1))
        pow *= 2;
    pow=pow/2;
    min_heapify(h, h_r, heapsize-1, pow, 1);
    return temp;
}

void printHeap(heap_node* h, int heapsize)
{
    int i;
    for(i=1; i<=heapsize; i++)
    {
        cout << "KEY=" << h[i].key << " ";
    }
    cout << "\n";
}





/////////////////FOR IMPLEMENTING PRIORITY QUEUE//////////////////////

/////////////////////////FOR GRAPH////////////////////////////////////

struct AdjListNode
{
    int dest;
    int wt;
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

struct edge
{
    int u;
    int v;
    int w;
};

struct Graph
{
    int V;
    int edge_no;
    edge* E;
    heap_node* h;
    int* heap_rec;                 //heap_rec is the array which stores the position of the individual vertices in the heap array. It is needed for constant time reference to the vertices from the array.
    AdjList* arr;
};

AdjListNode* create_node(int x, int weight)
{
    AdjListNode* newnode;
    newnode=new AdjListNode;
    newnode->dest=x;
    newnode->wt=weight;
    newnode->next=NULL;
    return newnode;
}

Graph* create_graph(Graph* g, int v, int e)
{
    g->V=v;
    g->E=new edge[e];
    g->edge_no=0;
    g->arr=new AdjList[v];
    g->h=new heap_node[v+1];
    g->heap_rec=new int[v+1];
    int i;
    for(i=0; i<v; i++)
    {
        g->arr[i].head=NULL;
        g->arr[i].color='w';
        g->arr[i].p=INT_MIN;
        g->arr[i].d=INT_MAX;
        g->arr[i].f=INT_MAX;
    }
    for(i=1; i<=v; i++)
    {
        g->h[i].key=INT_MAX;
        g->h[i].pr=INT_MIN;
        g->h[i].vertex_no=i;       //This does not store the index, it stores one more than the index.
        g->heap_rec[i]=i;          //This does not store the index, it stores one more than the index.
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
    AdjListNode* newnode=create_node(dest, weight);
    newnode->next=g->arr[src].head;
    g->arr[src].head=newnode;
    g->E[g->edge_no].u=src;
    g->E[g->edge_no].v=dest;
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
    AdjListNode* newnode=create_node(dest, weight);
    newnode->next=g->arr[src].head;
    g->arr[src].head=newnode;
    newnode=create_node(src, weight);
    newnode->next=g->arr[dest].head;
    g->arr[dest].head=newnode;
    g->E[g->edge_no].u=src;
    g->E[g->edge_no].v=dest;
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
            cout << "**" << pCrawl->wt << "**";
            pCrawl = pCrawl->next;
        }
        cout << "\n";
    }
}

/////////////////////////FOR GRAPH////////////////////////////////////

//////////////////////PRIM'S ALGORITHM////////////////////////////////



Graph* Prim(Graph* g, int s)          //s is the vertex no. not its index
{
    int temp1, temp2;
    heap_node temp;
    AdjListNode* aln;
    Graph* MST=new Graph;
    create_graph(MST, VERTEX, VERTEX-1);
    int heapsize=g->V;
    g->h[s].key=0;
    //
    //cout <<"&&&" << g->h[s].key << "&&&";
    printHeap(g->h, heapsize);
    cout << "\n";
    //
    build_min_heap(g->h, g->heap_rec, heapsize);
    //
    printHeap(g->h, heapsize);
    cout << "\n";
    //
    while(heapsize>0)
    {
        //
        printHeap(g->h, heapsize);
        cout << "\n";
        //
        temp=extract_min(g->h, g->heap_rec, heapsize);
        heapsize--;
        g->heap_rec[temp.vertex_no]=-1;
        //
        printHeap(g->h, heapsize);
        cout << "\n";
        //
        if(temp.pr!=INT_MIN)
            add_edge_undir(MST, (temp.pr)-1, (temp.vertex_no)-1, temp.key);
        temp1=(temp.vertex_no-1);
        aln=g->arr[(temp.vertex_no-1)].head;
        while(aln!=NULL)
        {
            temp2=g->heap_rec[(aln->dest)+1];
            if(temp2!=-1)
            {
                if((aln->wt)<(g->h[temp2].key))
                {
                    g->h[temp2].pr=temp1+1;
                    decrease_key(g->h, g->heap_rec, heapsize, temp2, aln->wt);
                }
            }
            aln=aln->next;
        }
    }
    return MST;
}





//////////////////////PRIM'S ALGORITHM////////////////////////////////

int main()
{
    /*int i,heapsize=10;
    heap_node e;
    heap_node* h=new heap_node[11];
    int* heap_rec=new int[11];
    for(i=1; i<=10; i++)
        heap_rec[i]=i;
    for(i=1; i<=10; i++)
        h[i].vertex_no=i;
    h[1].key=3;
    h[2].key=2;
    h[3].key=42;
    h[4].key=7;
    h[5].key=17;
    h[6].key=11;
    h[7].key=23;
    h[8].key=1;
    h[9].key=5;
    h[10].key=34;
    build_min_heap(h, heap_rec, heapsize);
    for(i=1; i<=heapsize; i++)
        cout << h[i].vertex_no << " ";
    cout <<"\n";
    for(i=1; i<=heapsize; i++)
        cout << h[i].key << " ";
    cout <<"\n";
    for(i=1; i<=heapsize; i++)
        cout << heap_rec[i] << " ";
    decrease_key(h, heap_rec, heapsize, h[6].vertex_no , 1);
    cout <<"\n";
    for(i=1; i<=heapsize; i++)
        cout << h[i].vertex_no << " ";
    cout <<"\n";
    for(i=1; i<=heapsize; i++)
        cout << h[i].key << " ";
    cout <<"\n";
    for(i=1; i<=heapsize; i++)
        cout << heap_rec[i] << " ";
    cout <<"\n";
    decrease_key(h, heap_rec, heapsize, 6 , 1);
    //for(i=1; i<=heapsize; i++)
        //cout << h[i].vertex_no << " ";
    //cout <<"\n";
    //for(i=1; i<=heapsize; i++)
        //cout << h[i].key << " ";
    //cout <<"\n";
    for(i=1; i<=heapsize; i++)
        cout << heap_rec[i] << " ";
    e=extract_min(h, heap_rec, heapsize);
    cout << "***" << e.key;*/


    int i, start;
    Graph* g=new Graph;
    Graph* MST;
    create_graph(g, VERTEX, EDGE);
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
    start=START;


    MST=Prim(g, start);

    printGraph(MST);

    for(i=0; i<MST->edge_no; i++)
        cout << " (" << MST->E[i].u << "," << MST->E[i].v << ") ;";

    return 0;
}
