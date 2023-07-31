#include<bits/stdc++.h>
using namespace std;

class Edge{
public:
    int ver1, ver2;
    double weight;
    Edge(int u, int v, double w)
    {
        ver1 = min(u,v);
        ver2 = max(u,v);
        weight = w;
    }
};

struct compEdge
{
    bool operator() (Edge a, Edge b)
    {
        if(a.weight==b.weight)
            return b.ver1<a.ver1;
        return b.weight<a.weight;
    }
};


vector<Edge> prim(int n, vector<Edge*> adj[], int root)
{
    vector<Edge> mst;
    priority_queue<Edge, vector<Edge>, compEdge> pqueue;
    bool visited[n]={};
    visited[root]=1;
    for(int i=0; i<adj[root].size(); i++)
        pqueue.push(*adj[root][i]);
    int nov/*number of visited nodes*/=1;
    while (nov<n)
    {
        Edge e = pqueue.top();
        pqueue.pop();
        int v;
        if(visited[e.ver1] && visited[e.ver2])
            continue;
        else if(visited[e.ver1]==0)
            v = e.ver1;
        else
            v = e.ver2;
        mst.push_back(e);
        visited[v]=1;
        nov++;
        for(int i=0; i<adj[v].size(); i++)
            pqueue.push(*adj[v][i]);
    }
    return mst;
}

int main()
{
    int n/*number of vertices*/, m/*number of edges*/;
    // freopen("in.txt","r",stdin);
    // freopen("prim.txt","w",stdout);
    cin>>n>>m;
    vector<Edge*> adj[n];
    for(int i=0; i<m; i++)
    {
        int u, v;
        double w;
        cin>>u>>v>>w;
        Edge *e = new Edge(u, v, w);
        adj[u].push_back(e);
        adj[v].push_back(e);
    }
    vector<Edge> mst = prim(n, adj, 0);
    double weight=0;
    for(int i=0; i<mst.size();i++)
        weight+=mst[i].weight;
    cout<<"Total weight = "<<weight<<"\n";
    cout<<"Root node = 0\n";
    for(int i=0; i<mst.size();i++)
        cout<<mst[i].ver1<<" "<<mst[i].ver2<<endl;
    return 0;
}