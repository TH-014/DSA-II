#include <bits/stdc++.h>
#include "HashTable.h"
using namespace std;

typedef long long ll;
typedef long double ld;

ll h1ChainCollision = 0, h2ChainCollision = 0, h1DHCollision = 0, h2DHCollision = 0, h1CPCollision = 0, h2CPCollision = 0;
ll h1SCProbe = 0, h2SCProbe = 0, h1DHProbe = 0, h2DHProbe = 0, h1CPProbe = 0, h2CPProbe = 0;
HashTable h1SC(0, separate_chain, hash1), h2SC(0, separate_chain, hash2);
HashTable h1DH(0, double_hashing, hash1), h2DH(0, double_hashing, hash2);
HashTable h1CP(0, custom_probing, hash1), h2CP(0, custom_probing, hash2);

void Init(ll n)
{
    h1SC.init(n);
    h2SC.init(n);
    h1DH.init(n);
    h2DH.init(n);
    h1CP.init(n);
    h2CP.init(n);
}

void Insert(const string& key, ll value)
{
    h1SC.insert(key, value, h1ChainCollision);
    h2SC.insert(key, value, h2ChainCollision);
    h1DH.insert(key, value, h1DHCollision);
    h2DH.insert(key, value, h2DHCollision);
    h1CP.insert(key, value, h1CPCollision);
    h2CP.insert(key, value, h2CPCollision);
}

void Delete(const string& key)
{
    h1SC.deleteKey(key);
    h2SC.deleteKey(key);
    h1DH.deleteKey(key);
    h2DH.deleteKey(key);
    h1CP.deleteKey(key);
    h2CP.deleteKey(key);
}

void Find(const string& key)
{
    h1SC.find(key, h1SCProbe);
    h2SC.find(key, h2SCProbe);
    h1DH.find(key, h1DHProbe);
    h2DH.find(key, h2DHProbe);
    h1CP.find(key, h1CPProbe);
    h2CP.find(key, h2CPProbe);
}

void print_report(ll n, ll N)
{
    ll digit=0;
    ll temp = n;
    while(temp>0)
    {
        temp/=10;
        digit++;
    }
    ld h1SCP = (h1SCProbe*1.0)/N;
    ld h2SCP = (h2SCProbe*1.0)/N;
    ld h1DHP = (h1DHProbe*1.0)/N;
    ld h2DHP = (h2DHProbe*1.0)/N;
    ld h1CPP = (h1CPProbe*1.0)/N;
    ld h2CPP = (h2CPProbe*1.0)/N;
    cout<<"-------------------------------------------------------------------------------------------------\n";
    cout<<"|  Hash  |  Collision           |             Hash1             |            Hash2              |\n";
    cout<<"| Table  |  Resolution          |-------------------------------|-------------------------------|\n";
    cout<<"|        |  Method              |        # of       |  Average  |        # of       |  Average  |\n";
    cout<<"|        |                      |     Collisions    |  Probe    |     Collisions    |  Probe    |\n";
    cout<<"-------------------------------------------------------------------------------------------------\n";
    cout<<"|        |  Chaining            |\t\t"<<h1ChainCollision<<"\t\t\t"<<h1SCP<<"\t \t\t"<<h2ChainCollision<<"\t\t\t"<<h2SCP<<"\t|\n";
    cout<<"|"<<" "<<n;
    for(ll i=0; i<7-digit; i++)
        cout<<" ";
    cout<<"|  Double Hashing      |\t\t"<<h1DHCollision<<"\t\t\t"<<h1DHP<<"\t\t\t"<<h2DHCollision<<"\t\t\t"<<h2DHP<<"\t|\n";
    cout<<"|        |  Custom Probing      |\t\t"<<h1CPCollision<<"\t\t\t"<<h1CPP<<"\t \t\t"<<h2CPCollision<<"\t\t\t"<<h2CPP<<"\t|\n";
    cout<<"-------------------------------------------------------------------------------------------------\n\n\n";
}

int main()
{
    srand(14);
    ll n, size, numOfPairs;
    cin>>size>>numOfPairs;
    n = getNextPrime(size);
    Init(n);
    freopen("report.txt", "a", stdout);
    ll v=0, temp=0;
    vector<string> generatedStrings;
    for(ll i=0; i<numOfPairs; i++){
        string s = getRandomString();
        if(!h1SC.find(s, temp))
        {
            v++;
            generatedStrings.push_back(s);
        }
        Insert(s, v);
    }
    for(ll i=0; i<numOfPairs/10; i++){
        ll index = rand()%generatedStrings.size();
        Find(generatedStrings[index]);
    }
    ll N = numOfPairs/10;
    ld h1SCP = (h1SCProbe*1.0)/N;
    ld h2SCP = (h2SCProbe*1.0)/N;
    ld h1DHP = (h1DHProbe*1.0)/N;
    ld h2DHP = (h2DHProbe*1.0)/N;
    ld h1CPP = (h1CPProbe*1.0)/N;
    ld h2CPP = (h2CPProbe*1.0)/N;
    cout<<"For N' = "<<size<<endl<<endl;
    print_report(n, N);
    return 0;
}