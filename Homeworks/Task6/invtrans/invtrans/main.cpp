//
//  main.cpp
//  invtrans
//
//  Created by Дмитрий Ткаченко on 23/11/2016.
//  Copyright © 2016 BillionariesClub. All rights reserved.
//


//Libraries block.
#include <iostream>
#include <vector>

using namespace std;

//Consts block.
const int MAX_SIZE = 2 * 1e5 + 10;

//Defines block.
typedef vector<int> vi;

//Vars block.
int n, ind = 1;
vi prefixFunc(MAX_SIZE), zFunc(MAX_SIZE);

int main() {
    
    //Philipp's magic cin boost.
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    //Code
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> prefixFunc[i];
        if (prefixFunc[i] > 0) {
            zFunc[i + 1 - prefixFunc[i]] = prefixFunc[i];
        }
    }
    
    zFunc[0] = n;
    while (ind < n) {
        int jumper = ind;
        if (zFunc[ind] > 0) {
            for (int j = 1; j < zFunc[ind]; j++) {
                if (zFunc[ind + j] > zFunc[j]) {
                    break;
                }
                zFunc[ind + j] = min(zFunc[j], zFunc[ind] - j);
                jumper = ind + j;
            }
        }
        ind = jumper + 1;
    }
    
    for (int i = 0; i < n; i++) {
        cout << zFunc[i] << " ";
    }
    
    return 0;
}
