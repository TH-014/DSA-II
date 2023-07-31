#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;

class Edge{
public:
    int ver1, ver2, weight;
    Edge(int u, int v, int w)
    {
        ver1 = u;
        ver2 = v;
        weight = w;
    }
};

int bellmanFord(int vertex, int source, int destination, const vector<Edge> &edges, vector<int> &par1)
{
    vector<long long> distance(vertex, INT_MAX);
    distance[source] = 0;
    bool changed;
    for(int i=0; i<vertex; i++)
    {
        changed=0;
        for(auto e: edges)
        {
            if(distance[e.ver1]+e.weight<distance[e.ver2])
            {
                changed=1;
                distance[e.ver2] = distance[e.ver1]+e.weight;
                par1[e.ver2] = e.ver1;
            }
        }
        if(!changed) // As no vertex's distace is updated so we can terminate the algorithm in this step
            break;
        // cerr<<"Iteration-"<<i+1<<": "<<distance[destination]<<endl;
    }
    if(changed) //Negative cycle detected
        return INT_MIN;
    return distance[destination];
}

int dijkstra(int vertex, int source, int destination, vector<int> adj[], vector<int> cost[], vector<int> &par2)
{
    vector<long long> distance(vertex, INT_MAX);
    distance[source] = 0;
    priority_queue<pii, vector<pii>, greater<> > pqueue;
    pqueue.push(make_pair(0, source));
    while (!pqueue.empty())
    {
        int u = pqueue.top().second;
        if(u==destination)
            return distance[u];
        pqueue.pop();
        for(int i=0; i<adj[u].size(); i++)
        {
            if(distance[u] + cost[u][i] < distance[adj[u][i]])
            {
                distance[adj[u][i]] = distance[u] + cost[u][i];
                par2[adj[u][i]] = u;
                pqueue.push(make_pair(distance[adj[u][i]], adj[u][i]));
            }
        }
    }
    return distance[destination];
}

int main()
{
    int n/*number of vertices*/, m/*number of edges*/;
    int source, destination;
    // freopen("in.txt","r",stdin);
    // freopen("out.txt","w",stdout);
    cin>>n>>m;
    vector<Edge> edges;
    vector<int> adj[n], cost[n];
    for(int i=0; i<m; i++)
    {
        int u, v, w;
        cin>>u>>v>>w;
        Edge *e = new Edge(u, v, w);
        edges.push_back(*e);
        w = abs(w);
        adj[u].push_back(v);
        cost[u].push_back(w);
    }
    cin>>source>>destination;

    //Bellman Ford
    cout<<"Bellman Ford Algorithm:\n";
    vector<int> par1(n, -1);
    int bmfCost = bellmanFord(n, source, destination, edges, par1);
    if(bmfCost==INT_MIN)
        cout<<"Negative weight cycle present\n\n";
    else{
        cout<<bmfCost<<endl;
        int d = destination;
        vector<int> path1;
        while (d>-1)
        {
            path1.push_back(d);
            d = par1[d];
        }
        cout<<source;
        for(int i=path1.size()-2; i>=0; i--)
            cout<<" ->"<<path1[i];
        cout<<"\n\n";
    }

    //Dijkstra
    cout<<"Dijkstra Algorithm:\n";
    vector<int> par2(n, -1);
    cout<<dijkstra(n, source, destination, adj, cost, par2)<<endl;
    int d = destination;
    vector<int> path2;
    while (d>-1)
    {
        path2.push_back(d);
        d = par2[d];
    }
    cout<<source;
    for(int i=path2.size()-2; i>=0; i--)
        cout<<" ->"<<path2[i];
    cout<<endl;
    return 0;
}