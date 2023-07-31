#include<bits/stdc++.h>
using namespace std;

#define inf 100000000

vector<int> bfs(int n, int source, vector<vector<int> > &aug_mat)
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
            }
        }
    }

    return parent;
}

set<int> ford_fulkerson(int lvl1, int n, int source, int sink, vector<vector<int> > &aug_mat)
{
    // vector<vector<int> > temp = aug_mat; // to get tho flow along edges
    int mxFlow=0;
    vector<int> path;
    vector<int> parent = bfs(n, source, aug_mat);
    int sk = sink;
    while (parent[sk]>-1)
    {
        path.push_back(sk);
        sk = parent[sk];
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
        parent.clear();

        //call bfs for other nodes
        parent = bfs(n, source, aug_mat);
        int sk = sink;
        while (parent[sk]>-1)
        {
            path.push_back(sk);
            sk = parent[sk];
        }
        path.push_back(source);
        for(int i=0; i<path.size()/2; i++)
            swap(path[i], path[path.size()-i-1]);
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
    set<int> minCut;
    for(int i=0; i<=lvl1; i++)
    {
        if(aug_mat[source][i]>0)
        {
            vector<int> vis = bfs(n, i, aug_mat);
            for(int j=lvl1+1; j<n-1; j++)
            {
                if(vis[j]>-1)
                {
                    minCut.insert(j-lvl1-1);////////////////////////////////
                }
            }
        }
    }
    return minCut;
}

vector<int> isEliminated(int id, int n, vector<int> &win, vector<int> &rem, vector<vector<int> > aug_mat)
{
    int lvl1 = (n*(n-1))/2, lvl2 = n;
    int vertex = lvl1+lvl2+2;
    for(int i=0; i<id; i++)
    {
        int idx = i*n+id+1-((i+1)*(i+2))/2;
        aug_mat[0][idx] = 0;
    }
    for(int i=id+1; i<n; i++)
    {
        int idx = id*n+i+1-((id+1)*(id+2))/2;
        aug_mat[0][idx] = 0;
    }
    for(int i=0; i<n; i++)
    {
        aug_mat[lvl1+i+1][vertex-1] = win[id]+rem[id]-win[i];
    }

    set<int> retval = ford_fulkerson(lvl1, vertex, 0, vertex-1, aug_mat);
    vector<int> winning;
    for(int x: retval)
        winning.push_back(x);
    return winning;
}

vector<vector<int> > network(int n, vector<int> &win,vector<int> &rem, vector<vector<int> > &schedule)
{
    int lvl1 = (n*(n-1))/2, lvl2 = n;
    int vertex = lvl1+lvl2+2;
    vector<vector<int> > aug_mat(vertex, vector<int> (vertex, 0));
    int idx;
    for(int i=0; i<n; i++)
    {
        for(int j=i+1; j<n; j++)
        {
            idx = i*n+j+1-((i+1)*(i+2))/2;
            aug_mat[0][idx]=schedule[i][j];
            int tmp1 = lvl1+1+i, tmp2 = lvl1+1+j;
            aug_mat[idx][tmp1] = inf;
            aug_mat[idx][tmp2] = inf;
        }
    }
    return aug_mat;
}

int main()
{
    // freopen("in_2.txt","r",stdin);
    // freopen("out_2.txt","w",stdout);
    int n;
    cin>>n;
    string tname[n];
    vector<int> win(n), rem(n);
    vector<vector<int> > schedule(n, vector<int> (n));
    for(int i=0; i<n; i++)
    {
        int tmp;
        cin>>tname[i]>>win[i]>>tmp>>rem[i];
        for(int j=0; j<n; j++)
        {
            cin>>schedule[i][j];
        }
    }
    int maxpts_index = max_element(win.begin(), win.end())-win.begin(); 
    if(n==2)
    {
        if(win[1-maxpts_index]+rem[1-maxpts_index]<win[maxpts_index])
        {
            cout<<tname[1-maxpts_index]<<" is eliminated.\n";
            cout<<"They can win at most "<<win[1-maxpts_index]<<" + "<<rem[1-maxpts_index]<<" = "<<win[1-maxpts_index]+rem[1-maxpts_index]<<" games.\n";
            cout<<tname[maxpts_index]<<" has won a total of "<<win[maxpts_index]<<" games.\n";
        }
        return 0;
    }
    vector<vector<int> > aug_mat = network(n, win, rem, schedule);
    for(int i=0; i<n; i++)
    {
        vector<int> winning = isEliminated(i, n, win, rem, aug_mat);
        if(winning.size()>0)
        {
            cout<<tname[i]<<" is eliminated.\n";
            cout<<"They can win at most "<<win[i]<<" + "<<rem[i]<<" = "<<win[i]+rem[i]<<" games.\n";
            if(win[i]+rem[i]<win[maxpts_index])
                cout<<tname[maxpts_index]<<" has won a total of "<<win[maxpts_index]<<" games.\n";
            else{
                //codes for mincut
                int tot_rem=0, tot_win=0;
                for(int j=0; j<winning.size(); j++)
                {
                    tot_win+=win[winning[j]];
                    for(int k=j+1; k<winning.size(); k++)
                        tot_rem+=schedule[winning[j]][winning[k]];
                }
                for(int j=0; j<winning.size(); j++)
                {
                    cout<<tname[winning[j]];
                    if(j<winning.size()-2)
                        cout<<", ";
                    else if(j == winning.size()-2)
                        cout<<" and ";
                    else
                        cout<<" ";
                }
                cout<<"have won a total of "<<tot_win<<" games.\n";
                cout<<"They play each other "<<tot_rem<<" times.\n";
                double avg_pts = ((tot_rem+tot_win)*1.0)/winning.size();
                cout<<"So on average, each of the teams wins "<<tot_rem+tot_win<<"/"<<winning.size()<<" = "<<avg_pts<<" games.\n";
            }
            cout<<"\n";
        }
    }

    return 0;
}