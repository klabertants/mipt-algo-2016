//
//  main.cpp
//  decimal
//
//  Created by Дмитрий Ткаченко on 23/11/2016.
//  Copyright © 2016 BillionariesClub. All rights reserved.
//

//Libraries block.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

//Vars block.
string sample, reversedSample;
vector<int> Z, P;
int periodLength, periodStart = 0;

//Z-function.
vector<int> zFunction (string s) {
    int n = (int) s.length();
    vector<int> z (n);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r)
            z[i] = min (r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            ++z[i];
        if (i + z[i]-1 > r)
            l = i,  r = i + z[i] - 1;
    }
    return z;
}

//Prefix-function
vector<int> prefixFunction (string s) {
    int n = (int) s.length();
    vector<int> result;
    result.resize(n);
    for (int i = 1; i < n; ++i) {
        int j = result[i - 1];
        while (j > 0 && s[i] != s[j]) {
            j = result[j - 1];
        }
        if (s[i] == s[j]) {
            j++;
        }
        result[i] = j;
    }
    return result;
}

int main() {
    
    //Philipp's magic cin boost.
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    //Code.
    cin >> sample;
    
    reversedSample = sample.substr(2, sample.size() - 2);
    reverse(reversedSample.begin(), reversedSample.end());
    periodLength = (int)reversedSample.length();
    Z = zFunction(reversedSample);
    P = prefixFunction(reversedSample);
    
    for (auto i = 1; i < reversedSample.length(); i++) {
        auto k = (i + 1) - P[i];
        auto j = i;
        if (i < reversedSample.length() - 1) {
            j += Z[i + 1];
        }
        if ((i + 1) % k == 0) {
            if (k + reversedSample.length() - j - 1 < periodLength) {
                periodLength = (int) (k + reversedSample.length() - j - 1);
                periodStart = j;
            }
        } else {
            if (i + 1 + reversedSample.length() - j - 1 < periodLength) {
                periodLength = (int) (i + 1 + reversedSample.length() - j - 1);
                periodStart = j;
            }
        }
    }
    
    cout << sample[0] << sample[1];
    periodStart = (int) (sample.length() - 1 - periodStart);
    ++(++periodLength);
    
    for (auto i = 2; i < periodStart; i++) {
        cout << sample[i];
    }
    
    cout << '(';
    
    for (auto i = periodStart; i < periodLength; i++) {
        cout << sample[i];
    }
    
    cout << ')';
    return 0;
}
