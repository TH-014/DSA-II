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
    cout<<"|        |  Chaining            |\t"<<h1ChainCollision<<"\t\t"<<h1SCP<<"\t \t"<<h2ChainCollision<<"\t\t"<<h2SCP<<"\t|\n";
    cout<<"|"<<"  "<<n<<" |  Double Hashing      |\t"<<h1DHCollision<<"\t\t"<<h1DHP<<"\t \t"<<h2DHCollision<<"\t\t"<<h2DHP<<"\t|\n";
    cout<<"|        |  Custom Probing      |\t"<<h1CPCollision<<"\t\t"<<h1CPP<<"\t \t"<<h2CPCollision<<"\t\t"<<h2CPP<<"\t|\n";
    cout<<"-------------------------------------------------------------------------------------------------\n\n\n";
}

int main()
{
    srand(14);
    ll n, size, numOfPairs;
   cin>>size>>numOfPairs;
    n = getNextPrime(size);
    Init(n);
//    freopen("report.txt", "w", stdout);
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
    cout<<"For N' = "<<size<<endl;
    print_report(n, N);
    return 0;
}