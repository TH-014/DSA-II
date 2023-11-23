#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

vector<vector<ll> > multiplication(int n, int pw, const vector<vector<ll> > &cost)
{
    if(pw==1)
        return cost;
    vector<vector<ll> > matL1 = multiplication(n, pw/2, cost);
    vector<vector<ll> > matL2(n+1, vector<ll> (n+1, INT_MAX));
    for(int i=1; i<=n; i++)
    {
        for(int j=1; j<=n; j++)
        {
            //calculate matL2[i][j]
            for(int k=1; k<=n; k++)
            {
                matL2[i][j] = min(matL2[i][j], matL1[i][k]+matL1[k][j]);
            }
        }
    }
    if(pw%2>0)
    {
        for(int i=1; i<=n; i++)
        {
            for(int j=1; j<=n; j++)
            {
                //calculate matL2[i][j]
                for(int k=1; k<=n; k++)
                {
                    matL2[i][j] = min(matL2[i][j], matL1[i][k]+cost[k][j]);
                }
            }
        }
    }
    return matL2;
}

vector<vector<ll> > mat_multiplication(int n, const vector<vector<ll> > &cost)
{
    return multiplication(n, n, cost);
}

vector<vector<ll> > floyd_warshall(int n,const vector<vector<ll> > &cost)
{
    vector<vector<ll> > dist = cost;
    for(int i=1; i<=n; i++)
    {
        for(int j=1; j<=n; j++)
        {
            for(int k=1; k<=n; k++)
            {
                // for(int i=1; i<=n; i++)
                // {
                    // dist[i][j] = min(dist[i][j], dist[i][k]+dist[k][j]);
                    dist[j][k] = min(dist[j][k], dist[j][i]+dist[i][k]);
                    // dist[i][j] = min(dist[i][j], dist[i][k]+dist[k][j]);
                    // dist[i][j] = min(dist[i][j], dist[i][k]+dist[k][j]);
                    // dist[i][j] = min(dist[i][j], dist[i][k]+dist[k][j]);
                    // dist[i][j] = min(dist[i][j], dist[i][k]+dist[k][j]);
                // }
            }
        }
    }
    return dist;
}

void print_mat(vector<vector<ll> > &dist, int n)
{
    cout<<"Shortest distance matrix\n";
    for(int i=1; i<=n; i++)
    {
        for(int j=1; j<=n; j++)
        {
            if(dist[i][j]>=INT_MAX)
                cout<<"INF ";
            else
                cout<<dist[i][j]<<" ";
        }
        cout<<"\n";
    }
}

int main()
{
    int n/*number of vertices*/, m/*number of edges*/;
    freopen("in.txt","r",stdin);
    freopen("mat.txt","w",stdout);
    cin>>n>>m;
    vector<vector<ll> > cost(n+1, vector<ll> (n+1, INT_MAX));
    for(int i=0; i<=n; i++)
        cost[i][i]=0;
    for(int i=0; i<m; i++)
    {
        int u, v;
        ll w;
        cin>>u>>v>>w;
        cost[u][v] = w;
    }

    // Matrix Multiplication
    vector<vector<ll> > dist1 = mat_multiplication(n, cost);
    print_mat(dist1, n);

    fclose(stdout);
    freopen("fw1.txt","w",stdout);
    
    // Floyd-Warshall
    vector<vector<ll> > dist2 = floyd_warshall(n, cost);
    print_mat(dist2, n);
    return 0;
}