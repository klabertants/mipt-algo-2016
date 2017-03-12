//
//  main.cpp
//  prof
//
//  Created by Дмитрий Ткаченко on 28/11/2016.
//  Copyright © 2016 BillionariesClub. All rights reserved.
//

//Includes block.
#include <iostream>
#include <vector>
#include <string>

using namespace std;

typedef vector<int> vi;

#define forn(i, x) for (int i = 0; i < x; i++)

//Vars block.
string text, subString;
int l, r, wordCount(0), prefixLength;
vi Z;

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

//Calculate interesting subwords!
void calculateWords(int pos) {
    l = r = 0;
    for (int j = (int) (subString.length() + 1); j < (int)(subString.length()) + 1 + pos; j++) {
        l = max(l, Z[j]);
    }
    for (int j = (int) (text.length() - pos - 1); j > 0; j--) {
        r = max(r, Z[text.length() + j + 1]);
        wordCount += (l < j) && (r >= j);
    }
}

int main(int argc, const char * argv[]) {
    
    //Philipp's magic cin boost.
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    //Code.
    cin >> text;
    
    forn(i, text.length()) {
        subString = "";
        for (int j = i; j < text.length(); j++) {
            subString += text[j];
        }
        Z = zFunction(subString + "$" + text);
        calculateWords(i);
    }
    
    cout << wordCount;
    
    return 0;
}

