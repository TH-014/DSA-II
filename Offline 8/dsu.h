#include<bits/stdc++.h>
using namespace std;

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