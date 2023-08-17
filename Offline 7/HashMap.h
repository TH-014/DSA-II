#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

//polynomial rolling hash function
ll hash1(const string &s, ll m){
    ll p = 31;
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

// PJW Hash Function
// https://en.wikipedia.org/wiki/PJW_hash_function
ll auxHash(const string& s, ll m) {
    ll hashValue = 0;
    ll n = s.size();
    for (ll i = 0; i < n; i++) {
        hashValue = (hashValue << 4) + s[i];
        ll g = hashValue & 0xf0000000L;
        if (g != 0)
            hashValue ^= g >> 24;
        hashValue &= ~g;
    }
    if(hashValue == 0)
        return 1;
    if(hashValue < 0)
        hashValue = -hashValue;
    hashValue %= m;
    return hashValue;
}