#include <bits/stdc++.h>
#include "HashMap.h"
using namespace std;

typedef long long ll;

class kvPair{
public:
    string key;
    ll value;
    kvPair(string k="", ll v=0){
        key = k;
        value = v;
    }
    bool operator==(const kvPair& ob){
        return key == ob.key;
    }
    bool operator==(const string& str){
        return key == str;
    }
};

//get next prime number
ll getNextPrime(ll n){
    ll i = n;
    while(true){
        bool isPrime = true;
        for(ll j=2; j*j<=i; j++){
            if(i%j == 0){
                isPrime = false;
                break;
            }
        }
        if(isPrime)
            return i;
        i++;
    }
}

//genarate random string
string getRandomString(){
    ll len = rand()%6 + 5;
    string s;
    for(ll i=0; i<len; i++){
        s += (char)('a' + rand()%26);
    }
    return s;
}

int main()
{
    srand(14);
    ll n;
    cin >> n;
    n = getNextPrime(n);
    vector<list<kvPair>> adjList(n);
    ll v=1;
    string t;
    for(ll i=0; i<10; i++){
        string s = getRandomString();
        if(i==6) t = s;
        ll h = hash1(s, n);
        cout<<s<<" "<<v<<endl;
        cout<<h<<" "<<hash2(s, n)<<" "<<auxHash(s, n)<<endl;
        adjList[h].push_back(kvPair(s, v));
        v++;
    }
    return 0;
}