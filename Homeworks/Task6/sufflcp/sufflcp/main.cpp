//
//  main.cpp
//  sufflcp
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
string sample;
int n;
vi suffixArray, reversedSuffixArray, LCP;

//Solver-function.
void countLCP() {
    int jumper(0), ind;
    forn(i, n) {
        if (jumper) jumper--;
        if (reversedSuffixArray[i] == n - 1) {
            LCP[n - 1] = -1;
            jumper = 0;
        } else {
            ind = suffixArray[reversedSuffixArray[i] + 1];
            while ( (i + jumper < n) && (ind + jumper < n) &&
                   (sample[i + jumper] == sample[ind + jumper])) jumper++;
            LCP[reversedSuffixArray[i]] = jumper;
        }
    }
}

int main(int argc, const char * argv[]) {
    
    //Philipp's magic cin boost.
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    //Code.
    cin >> n >> sample;
    sample += "$";
    suffixArray.resize(n), reversedSuffixArray.resize(n), LCP.resize(n);
    
    forn(i, n) {
        cin >> suffixArray[i];
        suffixArray[i]--;
        reversedSuffixArray[suffixArray[i]] = i;
    }
    
    countLCP();
    
    forn(i, n - 1) cout << LCP[i] << " ";
    
    return 0;
}
