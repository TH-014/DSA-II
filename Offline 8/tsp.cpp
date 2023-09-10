#include<bits/stdc++.h>
using namespace std;

/// Kruskal's algorithm
int *parent;

//sorting on the basis of column 3
bool sortcol(const vector<int>& v1, const vector<int>& v2)
{
    return v1[2] < v2[2];
}

//find parent function
int find(int i)
{
    if(parent[i]==i)
        return i;
    else
    {
        int res=find(parent[i]);
        parent[i]=res;
        return res;
    }
}

//Kruskal's Algorithm
vector<vector<int>> Kruskal(vector<vector<int>> vct, int vNum, int eNum)
{
    //sorting
    sort(vct.begin(),vct.end(), sortcol); //O(Elog|E|)
    for(int i=0;i<vNum;i++)
    {
        parent[i]=i;
    }
    int cnt=1;
    vector<vector<int>> ans;
    for(int i=0;i<eNum;i++) //O(|E|)
    {
        int pu=find(vct[i][0]);
        int pv=find(vct[i][1]);
        if(pu!=pv&&cnt<vNum)
        {
            vector<int> v3={vct[i][0],vct[i][1]};
            ans.push_back(v3);
            parent[pv]=pu;
            cnt++;
        }
        else
            continue;
    }
    return ans;
}


///DFS
vector<int> DFS(vector<int> adj[], int v, int src)
{
    vector<int> marked(v, -1);
    stack<int> Stack;
    vector<int> visited;
    Stack.push(src);
    while(!Stack.empty())
    {
        int top = Stack.top();
        if(marked[top]==-1)
            visited.push_back(top);
        marked[top]=0;
        Stack.pop();
        for(int i=0; i<adj[top].size(); i++)
        {
            if(marked[adj[top][i]]==-1)
                Stack.push(adj[top][i]);
        }
    }
    return visited;
}

// to turn a bit on
int turnBitOn(int n, int i)
{
    return n | (1<<i) ;
}

// to check if a bit is on
bool bitIsOn(int n, int i)
{
    bool isOn = (bool) (n & (1<<i));
    return isOn;
}


///Random Graph Generator With Metric Properties
vector<vector<int>> CreateRandomGraph(int v)
{
    vector<vector<int>> graph(v, vector<int> (v,0));
    for(int i=0; i<v; i++)
    {
        for(int j=i+1; j<v; j++)
        {
            int weight = rand()%151 + 50;
            graph[i][j] = weight;
            graph[j][i] = weight;
        }
    }
    for(int i=0; i<v; i++)
    {
        for(int j=i+1; j<v; j++)
        {
            for(int k=0; k<v; k++)
            {
                if(i!=k && j!=k)
                {
                    if(graph[i][k]+graph[k][j] < graph[i][j])
                    {
                        graph[i][j] = graph[i][k]+graph[k][j];
                        graph[j][i] = graph[i][k]+graph[k][j];
                    }
                }
            }
        }
    }
    return graph;
}

///Exact TSP Algorithm
int Exact_TSP(int i, int mask, vector<vector<int>> &graph, int v, vector<vector<int>> &dp)
{
    //vector<vector<int>> dp (v, vector<int> (1<<v, -1));
    if (mask == (1<<v)-1)
        return graph[i][0];

    if (dp[i][mask] != -1)
        return dp[i][mask];

    int mn=10000000, res;

    for(int j=0; j<v; j++)
    {
        if(!bitIsOn(mask, j) && j!=i)
        {
            res=Exact_TSP(j, turnBitOn(mask, j), graph, v, dp) + graph[i][j];
            mn = min(mn, res);
        }
    }
    dp[i][mask]=mn;
    return mn;
}


///Approximation TSP
vector<int> Metric_Approximation_TSP(vector<vector<int>> graph, int v)
{
    parent=new int[v];
    vector<vector<int>> vec;
    for(int i=0; i<v; i++)
    {
        for(int j=i+1; j<v; j++)
        {
            vector<int> v2={i,j,graph[i][j]};
            vec.push_back(v2);
        }
    }

    vector<vector<int>> ans= Kruskal(vec, v, vec.size());

    vector<int> adj[v];
    for(int i=0; i<ans.size(); i++)
    {
        adj[ans[i][0]].push_back(ans[i][1]);
        adj[ans[i][1]].push_back(ans[i][0]);
    }
    vector<int> tour = DFS(adj, v, 0);
    return tour;
}

///Calculating Tour Length
 int Calculate_Tour_Length(vector<int> path, vector<vector<int>> graph)
 {
     int src,dst;
     int len = path.size();
     src=path[0];
     dst=path[len-1];
     int tLen = graph[src][dst];
     for(int i=0; i<len-1; i++)
     {
         tLen += graph[i][i+1];
     }
     return tLen;
 }

int main()
{
    int v=8, lenExact, lenAprx;
    int X;
    cin >> X;
    vector<int> pathAprx, lenExactList, lenAprxList;
    vector<vector<int>> dp (v, vector<int> (1<<v, -1));

    for(int x=0; x<X; x++)
    {
        vector<vector<int>> G = CreateRandomGraph(v);
        pathAprx = Metric_Approximation_TSP(G, v);
        lenAprx=Calculate_Tour_Length(pathAprx, G);
        //cout << lenAprx <<endl;
        lenAprxList.push_back(lenAprx);
        lenExact = Exact_TSP(0, 0, G, v, dp);
        lenExactList.push_back(lenExact);
        //cout << lenExact <<endl;
    }

    for(int i=0; i<lenExactList.size(); i++)
    {
        cout << i+1 << " : " << (float) lenAprxList[i]/lenExactList[i] <<endl;
    }

    return 0;
}
