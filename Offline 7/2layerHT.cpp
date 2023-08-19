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

//Hash Table class
class HashTable{
public:
    crMethod method;
    ll n, size;
    vector<list<kvPair>> htList;
    string Key;
    ll (*hashFunction)(const string&, ll);
    HashTable(ll n, string k, crMethod method = separate_chain, ll (*hashFunction)(const string&, ll)=hash1){
        size = 0;
        Key = k;
        this->method = method;
        this->hashFunction = hashFunction;
        this->n = n;
        htList.resize(n);
    }
    kvPair find(const string& key){
        ll hashValue = hashFunction(key, n);
        if(method == separate_chain){
            for(auto it=htList[hashValue].begin(); it!=htList[hashValue].end(); it++){
                if(*it == key)
                {
                    // probe += distance(htList[hashValue].begin(), it) + 1;
                    return *it;
                }
            }
            return kvPair();
        }
    }
    void insert(const string& key, ll value){
        ll temp;
        ll hashValue = hashFunction(key, n);
        if(method == separate_chain){
            for(auto & it : htList[hashValue])
            {
                if(it == key)
                    return;
            }
            htList[hashValue].push_back(kvPair(key, value));
            size++;
        }
    }
    bool operator==(HashTable &ob)
    {
        return Key == ob.Key;
    }
    bool operator==(string &k)
    {
        return Key == k;
    }
};

class HashTable1{
    vector<list<HashTable>> layer1ht;
    ll n;
    ll (*hashFunction)(const string&, ll);
public:
    HashTable1(ll n, ll (*hashFunction)(const string&, ll)=hash2){
        this->n = n;
        layer1ht.resize(n);
        this->hashFunction = hashFunction;
    }
    void insert(string &k1, string &k2, ll val)
    {
        ll hashValue = hashFunction(k1, n);
        for(auto & it : layer1ht[hashValue])
        {
            if(it == k1)
            {
                it.insert(k2, val);
                return;
            }       
        }
        layer1ht[hashValue].push_back(HashTable(n, k1));
        layer1ht[hashValue].back().insert(k2, val);
    }
    kvPair find(string &k1, string &k2)
    {
        ll hashValue = hashFunction(k1, n);
        for(auto & it : layer1ht[hashValue])
        {
            if(it == k1)
            {
                return it.find(k2);
            }
        }
        return kvPair();
    }
    vector<kvPair> find(string &k1)
    {
        vector<kvPair> v;
        ll hashValue = hashFunction(k1, n);
        for(auto & it : layer1ht[hashValue])
        {
            if(it == k1)
            {
                for(auto & it2 : it.htList)
                {
                    for(auto & it3 : it2)
                    {
                        v.push_back(it3);
                    }
                }
                return v;
            }
        }
        return v;
    }
};


int main()
{
    ll n, op;
    cin>>n;
    n = getNextPrime(n);
    HashTable1 ht(n);
    while (1)
    {
        string cmd;
        cin>>cmd;
        if(cmd=="I")
        {
            string k1, k2;
            ll val;
            cin>>k1>>k2>>val;
            ht.insert(k1, k2, val);
        }
        else if(cmd=="F1")
        {
            string k1, k2;
            cin>>k1>>k2;
            ll val = ht.find(k1, k2).value;
            if(val==0)
                cout<<"Not found"<<endl;
            else
                cout<<val<<endl;
        }
        else if(cmd=="F2")
        {
            string k;
            cin>>k;
            vector<kvPair> v = ht.find(k);
            if(v.empty())
            {
                cout<<"Not found"<<endl;
                continue;
            }
            for(auto & it : v)
            {
                cout<<it.key<<" "<<it.value<<endl;
            }
            cout<<endl;
        }
        else
            break;
    }
    return 0;
}
/*
100
I BD Dhaka 500 
I BD Ctg 400 
I BD Syl 100
I USA Ny 4000
I USA wdc 4500 
I Ind Kolkata 50
I Ind Delhi 55
F1 USA Ny
F1 USA London
F2 BD
F2 Canada
E
*/