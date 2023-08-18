#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;
enum crMethod{separate_chain, double_hashing, custom_probing};
enum status{blank, occupied, deleted};
const ll c1 = 29, c2 = 37;

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

class Pair{
public:
    kvPair p;
    status st;
    Pair(string k="", ll v=0){
        p.key = k;
        p.value = v;
        st = blank;
    }
    bool operator==(const Pair& ob){
        if(st == occupied)
            return p == ob.p;
        else
            return false;
    }
    bool operator==(const string& str){
        if(st == occupied)
            return p == str;
        else
            return false;
    }
};

//polynomial rolling hash function
ll hash1(const string &s, ll m){
    ll p = 37;
    ll hashValue = 0;
    ll pPow = 1;
    for(char c : s){
        hashValue = (hashValue + (c - 'a' + 1) * pPow) % m;
        pPow = (pPow * p) % m;
    }
    if(hashValue < 0)
        hashValue += m;
    else if(hashValue == 0)
        hashValue=1;
    return hashValue;
}

// Jenkins "one_at_a_time" Hash Function
// https://en.wikipedia.org/wiki/Jenkins_hash_function
ll hash2(const string& s, ll m) {
    ll hashValue = 0;
    ll n = s.size();
    for (ll i = 0; i < n; i++) {
        hashValue += s[i];
        hashValue += (hashValue << 10);
        hashValue ^= (hashValue >> 6);
    }
    hashValue += (hashValue << 3);
    hashValue ^= (hashValue >> 11);
    hashValue += (hashValue << 15);
    if(hashValue == 0)
        return 1;
    if(hashValue < 0)
        hashValue = -hashValue;
    hashValue %= m;
    return hashValue;
}

//Auxiliary Hash Function for double hashing and custom probing
ll auxHash(const string& s, ll m) {
    ll p = 10000007;
    ll hashValue = 0;
    ll pPow = 1;
    for(char c : s){
        hashValue = (hashValue + (c - 'a' + 1) * pPow) % m;
        pPow = (pPow * p) % m;
    }
    if(hashValue < 0)
        hashValue += m;
    else if(hashValue == 0)
        hashValue=1;
    return hashValue;
}

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

//generate random string
string getRandomString(){
    ll len = rand()%6 + 5;
    string s;
    for(ll i=0; i<len; i++){
        s += (char)('a' + rand()%26);
    }
    return s;
}

//calculate double_hash index
ll doubleHash(ll hashValue, ll auxHashValue, ll n, ll i){
    return (hashValue + i*auxHashValue) % n;
}

//calculate custom_probing index
ll customProbe(ll hashValue, ll auxHashValue, ll n, ll i){
    return (hashValue + c1*i*auxHashValue + c2*i*i) % n;
}

//Hash Table class
class HashTable{
    crMethod method;
    ll n, size;
    vector<list<kvPair>> htList;
    vector<Pair> htPair;
    ll (*hashFunction)(const string&, ll);
    ll calcIndex(ll hashValue, ll auxHashValue, ll i){
        if(method==double_hashing)
            return doubleHash(hashValue, auxHashValue, n, i);
        else if(method==custom_probing)
            return customProbe(hashValue, auxHashValue, n, i);
    }
public:
    HashTable(ll n, crMethod method, ll (*hashFunction)(const string&, ll)){
        size = 0;
        this->method = method;
        this->hashFunction = hashFunction;
        if(n>0)
        {
            this->n = n;
            if(method == separate_chain){
                htList.resize(n);
            }
            else{
                htPair.resize(n);
            }
        }
    }
    void init(ll N)
    {
        this->n = N;
        if(method == separate_chain){
            htList.resize(n);
        }
        else{
            htPair.resize(n);
        }
    }
    bool find(const string& key, ll &probe){
        ll hashValue = hashFunction(key, n);
        if(method == separate_chain){
            for(auto it=htList[hashValue].begin(); it!=htList[hashValue].end(); it++){
                if(*it == key)
                {
                    probe += distance(htList[hashValue].begin(), it) + 1;
                    return true;
                }
            }
            return false;
        }
        else{
            ll i = 0;
            ll auxHashValue = auxHash(key, n);
            while(i<n){
                ll index = calcIndex(hashValue, auxHashValue, i);
                if(htPair[index].st == blank)
                    return false;
                else if(htPair[index].st == occupied && htPair[index].p == key)
                {
                    probe += i+1;
                    return true;
                }
                i++;
            }
            return false;
        }
    }
    void insert(const string& key, ll value, ll &collision){
        ll temp;
        if(find(key, temp))
            return;
        ll hashValue = hashFunction(key, n);
        if(method == separate_chain){
            for(auto & it : htList[hashValue])
            {
                if(it == key)
                    return;
            }
            if(!htList[hashValue].empty())
                collision++;
            htList[hashValue].push_back(kvPair(key, value));
            size++;
        }
        else{
            if(size == n)
                return;
            ll i = 0;
            ll auxHashValue = auxHash(key, n);
            while(i<n){
                ll index = calcIndex(hashValue, auxHashValue, i);
                if(htPair[index].st == blank || htPair[index].st == deleted){
                    htPair[index].p.key = key;
                    htPair[index].p.value = value;
                    htPair[index].st = occupied;
                    collision += i;
//                    if(i>0)
//                        collision++;
                    return;
                }
                else if(htPair[index].st == occupied && htPair[index].p == key)
                    return;
                i++;
            }
        }
    }
    void deleteKey(const string& key){
        ll hashValue = hashFunction(key, n);
        if(method == separate_chain){
            for(auto it=htList[hashValue].begin(); it!=htList[hashValue].end(); it++){
                if(*it == key)
                {
                    htList[hashValue].erase(it);
                    return;
                }
            }
        }
        else{
            ll i = 0;
            ll auxHashValue = auxHash(key, n);
            while(i<n){
                ll index = calcIndex(hashValue, auxHashValue, i);
                if(htPair[index].st == blank)
                    return;
                else if(htPair[index].st == occupied && htPair[index].p == key)
                {
                    htPair[index].st = deleted;
                    return;
                }
                i++;
            }
        }

    }
};