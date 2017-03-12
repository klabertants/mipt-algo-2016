//
//  main.cpp
//  kthstr
//
//  Created by Дмитрий Ткаченко on 21/11/2016.
//  Copyright © 2016 BillionariesClub. All rights reserved.
//

//----------------------------
// Big thanks to Burunduk, and his lecture
// http://opentrains.mipt.ru/zksh/files/zksh2015/lectures/mipt-2014-burunduk1-ds.pdf (which I've visited by myself besides).
//----------------------------


//Includes block.
#include <ext/pb_ds/assoc_container.hpp>
#include <iostream>
#include <string>

//Namespaces block.
using namespace std;
using namespace __gnu_pbds;

//Is char digit?
bool isDigit(char c) {
    return (c >= '0' && c <= '9');
}

//Vars block.
int n, index;
string sample;
tree<string, null_type, less<string>, rb_tree_tag, tree_order_statistics_node_update> borTree;

int main() {
    
    //Philipp's magic cin boos.
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    
    //Code
    cin >> n;
    
    for (auto i = 0; i < n; i++) {
        cin >> sample;
        if (isDigit(sample[0])) {
            index = stoi(sample);
            cout << *(borTree.find_by_order(index - 1)) << endl;
        } else {
            borTree.insert(sample);
        }
    }
    
    return 0;
}
