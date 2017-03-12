//
//  main.cpp
//  trie
//
//  Created by Дмитрий Ткаченко on 01/12/2016.
//  Copyright © 2016 BillionariesClub. All rights reserved.
//

//Includes block.
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

const long long ALPHABET_SIZE = 26;

typedef vector<long long> vi;

#define forn(i, x) for (long long i = 0; i < x; i++)

//Vars block.
long long n, k, number, edge, m;
char c;
string w;

//Bor-tree.
class Bor{
public:
    struct vertex{
        long long child[ALPHABET_SIZE];
        long long leaf;
        long long parent, link, go[ALPHABET_SIZE];
        long long parentChar;
        long long numCounted;
        bool isCounted;
        bool isOld;
        
        vertex(long long x) {
            for (long long i = 0; i < ALPHABET_SIZE; i++) {
                child[i] = -1;
                go[i] = -1;
            }
            leaf = 0;
            isCounted = false;
            numCounted = 0;
            link = parent = -1;
            parentChar = 0;
            isOld = false;
        }
        
    };
    
    vector<vertex> tree;
    
    Bor() {
        tree.push_back(vertex(0));
        tree[0].parentChar = 0;
        tree[0].isCounted = true;
        tree[0].isOld = true;
    }
    
    Bor(long long size) {
        for (long long i = 0; i < size; i++) {
            tree.push_back(vertex(0));
            tree[i].isOld = true;
        }
        tree[0].isCounted = true;
        
    }
    
    void addString(const string &sample) {
        long long v = 0;
        for (size_t i = 0; i < sample.length(); i++) {
            long long c = sample[i] - 'a';
            if (tree[v].child[c] == -1) {
                tree.push_back(vertex(0));
                tree[tree.size() - 1].parent = v;
                tree[tree.size() - 1].parentChar = c;
                tree[v].child[c] = (long long) (tree.size() - 1);
            }
            v = tree[v].child[c];
        }
        tree[v].leaf++;
    }
    
    long long go(long long v, long long c);
    
    long long getLink(long long v) {
        if (tree[v].link == -1) {
            if (v == 0 || tree[v].parent == 0) {
                tree[v].link = 0;
            } else {
                tree[v].link = go(getLink (tree[v].parent), tree[v].parentChar);
            }
        }
        return tree[v].link;
    };
    
    void print() {
        for (long long i = 0; i < tree.size(); i++) {
            cout << "Number: " << i << endl;
            cout << "Is a leaf: " << tree[i].leaf << endl;
            cout << "Parent number: " << tree[i].parent << endl;
            cout << "Edge char from parent: " << (char) (tree[i].parentChar + 'a') << endl;
            cout << "Suffix link is number: " << tree[i].link << endl;
            cout << "Badcount is: " << tree[i].numCounted << endl;
        }
    }
    
    long long countTerminals(long long v) {
        if (tree[v].isCounted) {
            return tree[v].numCounted;
        } else {
            tree[v].isCounted = true;
            return tree[v].numCounted = countTerminals(getLink(v)) + tree[v].leaf;
        }
    }
    
    long long countAns() {
        long long ans = 0;
        for (long long i = 0; i < tree.size(); i++) {
            ans += (long long)(countTerminals(i) * tree[i].isOld);
        }
        return ans;
    }
};

long long Bor::go(long long v, long long c) {
    if (tree[v].go[c] == -1) {
        if (tree[v].child[c] != -1) {
            tree[v].go[c] = tree[v].child[c];
        } else {
            tree[v].go[c] = (v == 0) ? (0) : (go(getLink(v), c));
        }
    }
    return tree[v].go[c];
}


int main() {
    
    //Philipp's magic cin boost.
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    //Code.
    cin >> n;
    Bor bor(n);
    
    for (long long i = 0; i < n; i++) {
        cin >> k;
        while (k > 0) {
            cin >> number;
            number--;
            cin >> c;
            edge = c - 'a';
            bor.tree[i].child[edge] = number;
            bor.tree[number].parent = i;
            bor.tree[number].parentChar = edge;
            k--;
        }
    }
    
    cin >> m;
    
    for (long long i = 0; i < m; i++) {
        cin >> w;
        bor.addString(w);
    }
    
    cout << bor.countAns();
    
    return 0;
}

