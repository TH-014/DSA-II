#include<bits/stdc++.h>
#include "binomial_heap.h"
using namespace std;

binomial_heap call_union(string s)
{
    binomial_heap h;
    vector<int> v;
    for(int i=2; i<s.length();i++)
    {
        int num=0;
        while(s[i]!=' ' && i<s.length())
        {
            num = (s[i]-'0')+ num*10;
            i++;
        }
        v.push_back(num);
    }
    for(int i=0; i<v.size(); i++)
        h.Insert(v[i]);
    return h;
}

int main()
{
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    // freopen("err.txt", "w", stderr);
    binomial_heap myheap, uheap;
    string str;
    while (getline(cin, str))
    {
        int num=0, i;
        char cmd = str[0];
        for(i=2; i<str.length();i++)
        {
            if(str[i]>='0' && str[i]<='9')
                num = (str[i]-'0')+ num*10;
        }
        //cerr<<cmd<<" "<<num<<endl;
        switch (cmd)
        {
        case 'F':
            cout<<"Find-Min returned "<<myheap.FindMin()<<endl;
            break;
        case 'E':
            cout<<"Extract-Min returned "<<myheap.ExtractMin()<<endl;
            break;
        case 'I':
            myheap.Insert(num);
            break;
        case 'U':
            uheap = call_union(str);
            myheap.Union(uheap);
            break;
        case 'P':
            myheap.Print();
            break;
        default:
            break;
        }
    }
    return 0;
}