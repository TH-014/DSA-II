#include<bits/stdc++.h>
#include "dsu.h"
using namespace std;

class Edge{
public:
    int ver1, ver2;
    int weight;
    Edge(int u, int v, int w)
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

vector<Edge> kruskal(int n, vector<Edge> edges)
{
    vector<Edge> mst;
    DSU dsu(n);
    sort(edges.begin(), edges.end(), compEdge);
    for(int i=0; i<edges.size(); i++)
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