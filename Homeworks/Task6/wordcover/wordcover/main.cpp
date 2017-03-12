//
//  main.cpp
//  wordcover
//
//  Created by Дмитрий Ткаченко on 30/11/2016.
//  Copyright © 2016 BillionariesClub. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>

using namespace std;

//Consts block.
const int MAX_SIZE = 2.6 * 1e5;

//Vars block.
string sample;
int answer[MAX_SIZE], reversed[MAX_SIZE], backInsert[MAX_SIZE];
int pointer;

//Helpful functions.
bool compareChars(int x, int y) {
    return sample[x] == sample[y];
}

int main() {
    
    //Philipp's magic cin boost.
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    //Code.
    cin >> sample;
    for (int i = 1; i < sample.size(); i++) {
        while (pointer && !compareChars(pointer, i)) {
            pointer = backInsert[pointer - 1];
        }
        pointer += compareChars(pointer, i);
        backInsert[i] = pointer;
        if (pointer == 0 || reversed[answer[pointer - 1]] + answer[pointer - 1] + 1 < i) {
            answer[i] = i;
            reversed[i] = i;
        } else {
            answer[i] = answer[pointer - 1];
            reversed[answer[i]] = i;
        }
    }
    
    for (int i = 0; i < sample.size(); i++) {
        cout << answer[i] + 1 << " ";
    }
    return 0;
}
