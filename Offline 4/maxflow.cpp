#include<bits/stdc++.h>
using namespace std;

vector<int> bfs(int n, int source, int sink, vector<vector<int> > &aug_mat)
{
    vector<bool> visited(n, 0);
    vector<int> parent(n, -1);
    vector<int> path;
    visited[source] = 1;
    queue<int> Q;
    Q.push(source);
    while (Q.size()>0)
    {
        int u = Q.front();
        Q.pop();
        for(int i=0; i<n; i++)
        {
            if(aug_mat[u][i]>0 && visited[i]==0)
            {
                visited[i]=1;
                Q.push(i);
                parent[i] = u;
                if(i==sink)
                    break;
            }
        }
    }
    while (parent[sink]>-1)
    {
        path.push_back(sink);
        sink = parent[sink];
    }
    path.push_back(source);
    for(int i=0; i<path.size()/2; i++)
        swap(path[i], path[path.size()-i-1]);
    // for(int i=0; i<path.size(); i++)
    // {
    //     cerr<<path[i];
    //     if(i<path.size()-1)
    //         cerr<<" -> ";
    //     else
    //         cerr<<"\n";
    // }
    return path;
}

int ford_fulkerson(int n, int source, int sink, vector<vector<int> > &aug_mat)
{
    // vector<vector<int> > temp = aug_mat; // to get tho flow along edges
    int mxFlow=0;
    vector<int> path = bfs(n, source, sink, aug_mat);
    while (path.size()>1)
    {
        int mincap=INT_MAX;
        for(int i=0; i<path.size()-1; i++)
            mincap = min(mincap, aug_mat[path[i]][path[i+1]]);
        // cerr<<"mincap = "<<mincap<<"\n\n";
        mxFlow += mincap;
        for(int i=0; i<path.size()-1; i++)
        {
            aug_mat[path[i]][path[i+1]] -= mincap;
            aug_mat[path[i+1]][path[i]] += mincap;
        }
        path.clear();
        path = bfs(n, source, sink, aug_mat);
    }

    // Printing flow along edges
    // for(int i=0; i<n; i++)
    // {
    //     for(int j=0; j<n; j++)
    //     {
    //         if(temp[i][j]-aug_mat[i][j]>0)
    //             cout<<i<<" -> "<<j<<" : "<<temp[i][j]-aug_mat[i][j]<<endl;
    //     }
    // }
    return mxFlow;
}

int main()
{
    int n, m;
    cin>>n>>m;
    vector<vector<int> > aug_mat(n, vector<int> (n, 0));
    for(int i=0; i<m; i++)
    {
        int u, v, cap;
        cin>>u>>v>>cap;
        aug_mat[u][v] = cap; //considering directed graph
    }
    int source, sink;
    cin>>source>>sink;
    cout<<ford_fulkerson(n, source, sink, aug_mat)<<endl;
    return 0;
}