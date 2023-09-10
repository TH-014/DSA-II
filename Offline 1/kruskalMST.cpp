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

bool compEdge(Edge e1, Edge e2)
{
    if(e1.weight==e2.weight)
        return e1.ver1<e2.ver1;
    return e1.weight<e2.weight;
}

class DSU{
int N;
int *parent;
public:
    DSU(int n)
    {
        N = n;
        parent = new int[n];
        for(int i=0; i<n; i++)
            parent[i] = i;
    }
    int find_parent(int u)
    {
        if(parent[u]==u)
            return u;
        parent[u] = find_parent(parent[u]);
        return parent[u];
    }
    void Union(int u, int v)
    {
        int pu = find_parent(u);
        int pv = find_parent(v);
        if(pu!=pv)
            parent[pv] = pu;
    }
    ~DSU()
    {
        delete [] parent;
    }
};

vector<Edge> kruskal(int n, int m, vector<Edge> &edges)
{
    vector<Edge> mst;
    DSU dsu(n);
    sort(edges.begin(), edges.end(), compEdge);
    for(int i=0; i<m; i++)
    {
        int p1 = dsu.find_parent(edges[i].ver1);
        int p2 = dsu.find_parent(edges[i].ver2);
        if(p1!=p2)
        {
            mst.push_back(edges[i]);
            dsu.Union(edges[i].ver1, edges[i].ver2);
        }
    }
    return mst;
}

int main()
{
    int n/*number of vertices*/, m/*number of edges*/;
    // freopen("in.txt","r",stdin);
    // freopen("kruskal.txt","w",stdout);
    cin>>n>>m;
    vector<Edge> edges;
    vector<Edge> mst;
    for(int i=0; i<m; i++)
    {
        int u, v;
        double w;
        cin>>u>>v>>w;
        Edge *e = new Edge(u, v, w);
        edges.push_back(*e);
    }
    mst = kruskal(n, m, edges);
    double weight=0;
    for(int i=0; i<mst.size();i++)
        weight+=mst[i].weight;
    cout<<"Total weight = "<<weight<<"\n";
    for(int i=0; i<mst.size();i++)
        cout<<mst[i].ver1<<" "<<mst[i].ver2<<endl;
    return 0;
}