//
//  main.cpp
//  substrcmp
//
//  Created by Дмитрий Ткаченко on 21/11/2016.
//  Copyright © 2016 BillionariesClub. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>

using namespace std;

//Defines block.
typedef long long ll;

//Consts block.
const ll MOD = 1e9 + 7;
const ll MAGIC_NUMBER = 47;

//Vars block.
string sample;
vector<ll> prefixHash, powers;
ll a, b, c, d, q, hash1, hash2;

//Distance between two numbers.
ll distance(ll x, ll y) {
    return abs(x - y);
}

//Displaced character.
ll displace(char c){
    return ll(c - 'a' + 1);
}

//Hashing string prefixes.
pair<vector<ll>, vector<ll>> hashing(string& sample) {
    pair<vector<ll>, vector<ll>> answer;
    answer.second.resize(sample.length());
    answer.second[0] = 1;
    for (auto i = 1; i < sample.length(); i++) {
        answer.second[i] = answer.second[i - 1] * MAGIC_NUMBER;
        answer.second[i] %= MOD;
    }
    answer.first.resize(sample.length());
    answer.first[0] = displace(sample[0]);
    for (auto i  = 1; i < sample.length(); i++) {
        answer.first[i] = answer.first[i - 1] + displace(sample[i]) * answer.second[i];
        answer.first[i] %= MOD;
    }
    return answer;
}

int main() {
    
    //Philipp's magic cin boost.
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    //Code
    cin >> sample;
    pair<vector<ll>, vector<ll>> vectorsPair = hashing(sample);
    prefixHash = vectorsPair.first; powers = vectorsPair.second;
    cin >> q;
    for (auto i = 0; i < q; i++) {
        cin >> a >> b >> c >> d;
        if (distance(--c, --d) != distance(--a, --b)) {
            cout << "No\n";
            continue;
        }
        if (a == 0) {
            hash1 = prefixHash[b];
        } else {
            hash1 = prefixHash[b] - prefixHash[a - 1] + MOD;
        }
        if (c == 0) {
            hash2 = prefixHash[d];
        } else {
            hash2 = prefixHash[d] - prefixHash[c - 1] + MOD;
        }
        hash1 %= MOD; hash1 *= powers[c]; hash2 %= MOD; hash2 *= powers[a];
        cout << ((hash1 % MOD == hash2 % MOD) ? "Yes\n" : "No\n");
    }
    
    
    return 0;
}
