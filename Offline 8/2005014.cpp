#include<bits/stdc++.h>
#include "kruskal.h"
using namespace std;

class Graph{
public:
    int V;
    vector<vector<int> > adj;
    vector<Edge> edges;
    Graph(int n)
    {
        V = n;
        adj.resize(n, vector<int> (n,0));
    }
    void addEdge(int u, int v, int w)
    {
        Edge e(u,v,w);
        adj[u][v] = adj[v][u] = w;
        edges.push_back(e);
    }
    void make_graph(vector<Edge> &edge_list)
    {
        for(int i=0; i<edge_list.size(); i++)
        {
            int u = edge_list[i].ver1;
            int v = edge_list[i].ver2;
            int w = edge_list[i].weight;
            addEdge(u,v,w);
        }
    }
};

Graph Create_Random_Graph(int v)
{
    Graph g(v);
    for(int i=0; i<v-1; i++)
    {
        int w = rand()%151 + 50;
        g.addEdge(i, i+1, w);
    }
    for(int i=0; i<v; i++)
    {
        for(int j=i+2; j<v; j++)
        {
            int minw = max(50, abs(g.adj[i][j-1]-g.adj[j][j-1]));
            int maxw = min(200, (g.adj[i][j-1]+g.adj[j][j-1]));
            int w = rand()%(maxw-minw+1) + minw;
            g.addEdge(i, j, w);
        }
    }
    return g;
}

int Calculate_Tour_Length(vector<int> cycle, Graph g)
{
    int sum = 0;
    for(int i=0; i<cycle.size()-1; i++)
        sum += g.adj[cycle[i]][cycle[i+1]];
    sum += g.adj[cycle[0]][cycle[cycle.size()-1]];
    return sum;
}

vector<int> dfs(Graph &mst)
{
    vector<int> cycle;
    stack<int> s;
    vector<bool> visited(mst.V, false);
    s.push(0);
    while(!s.empty())
    {
        int u = s.top();
        s.pop();
        if(!visited[u])
        {
            cycle.push_back(u);
            visited[u] = true;
            for(int i=0; i<mst.V; i++)
            {
                if(mst.adj[u][i]!=0 && !visited[i])
                    s.push(i);
            }
        }
    }
    cycle.push_back(0);
    return cycle;
}

int Metric_Approximation_TSP(Graph g)
{
    vector<Edge> mst_edges = kruskal(g.V, g.edges);
    Graph mst(g.V);
    mst.make_graph(mst_edges);
    vector<int> cycle = dfs(mst);
    // for(int i=0; i<cycle.size(); i++)
    //    cout<<cycle[i]<<" ";
    // cout<<endl;
    return Calculate_Tour_Length(cycle, g);
}

int setBit(int mask, int pos) {
    return mask | (1<<pos);
}

bool isSet(int mask ,int pos) {
    return (mask & (1<<pos));
}

int bitmask_dp(vector<vector<int> > &adj, vector<vector<int> > &dp, int n, int i, int mask) 
{
    if(mask == (1<<n) - 1)
        return adj[i][0];
    if(dp[i][mask] != -1)
        return dp[i][mask];
    int ans = 1e8;
    for (int j = 0; j<n; j++)
    {
        if (!isSet(mask,j))
        {
            int result = bitmask_dp(adj, dp, n, j, setBit(mask, j)) + adj[i][j];
            ans = min(ans, result);
        }
    }
    dp[i][mask] = ans;
    return dp[i][mask];
}

int Exact_TSP(Graph g)
{
    int n = g.V;
    vector<vector<int> > dp(n, vector<int> ((1<<n),-1));
    int mincost = bitmask_dp(g.adj, dp, n, 0, 1);
    return mincost;
}

int main()
{
    srand(time(0));
    cout<<"Enter the number of testcases: ";
    int x;
    cin>>x;
    for(int i=0; i<x; i++)
    {
        int exact, approx;
        long double ratio;
        Graph g = Create_Random_Graph(20);
        // for(int j=0; j<g.V; j++)
        // {
        //     for(int k=0; k<g.V; k++)
        //         cout<<g.adj[j][k]<<" ";
        //     cout<<endl;
        // }
        exact = Exact_TSP(g);
        approx = Metric_Approximation_TSP(g);
        cout<<"Testcase "<<i+1<<": "<<exact<<" "<<approx<<"\n";
        ratio = (long double)approx/exact;
        cout<<"( "<<i+1<<", "<<ratio<<" )"<<endl;
    }
    return 0;
}