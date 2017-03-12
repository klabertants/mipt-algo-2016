//
//  main.cpp
//  virus
//
//  Created by Дмитрий Ткаченко on 30/11/2016.
//  Copyright © 2016 BillionariesClub. All rights reserved.
//

//Includes block.
#include <iostream>
#include <vector>
#include <string>

using namespace std;

const int ALPHABET_SIZE = 2;

typedef vector<int> vi;

#define forn(i, x) for (int i = 0; i < x; i++)

//Vars block.
int n;
string w, sample;

//Bor-tree.
class Bor{
private:
    
    struct vertex{
        int child[ALPHABET_SIZE];
        bool leaf, isBad, isBadCounted;
        int parent, link, go[ALPHABET_SIZE];
        int parentChar;
        
        vertex(int x) {
            for (int i = 0; i < ALPHABET_SIZE; i++) {
                child[i] = -1;
                go[i] = -1;
            }
            leaf = isBad = isBadCounted = false;
            link = parent = -1;
        }
        
    };
    
    vector<vertex> tree;
    vector<int> used;
public:
    
    Bor() {
        tree.push_back(vertex(0));
        used.push_back(-1);
        tree[0].isBad = false;
        tree[0].isBadCounted = true;
        tree[0].parentChar = 0;
    }
    
    void addString(const string &sample) {
        int v = 0;
        for (size_t i = 0; i < sample.length(); i++) {
            int c = sample[i] - '0';
            if (tree[v].child[c] == -1) {
                tree.push_back(vertex(0));
                used.push_back(-1);
                tree[tree.size() - 1].parent = v;
                tree[tree.size() - 1].parentChar = c;
                tree[v].child[c] = (int) (tree.size() - 1);
            }
            v = tree[v].child[c];
        }
        tree[v].leaf = true;
        tree[v].isBad = true;
        tree[v].isBadCounted = true;
    }
    
    int go(int v, int c);
    
    int getLink(int v) {
        if (tree[v].link == -1) {
            if (v == 0 || tree[v].parent == 0) {
                tree[v].link = 0;
            } else {
                tree[v].link = go(getLink (tree[v].parent), tree[v].parentChar);
            }
        }
        return tree[v].link;
    };
    
    bool determineBad(int v) {
        if (tree[v].isBadCounted) {
            return tree[v].isBad;
        } else {
            tree[v].isBad = determineBad(tree[v].link);
            tree[v].isBadCounted = true;
            return tree[v].isBad;
        }
    }
    
    void countAllLinks() {
        for (int i = 0; i < tree.size(); i++) {
            tree[i].link = getLink(i);
        }
    }
    
    void determineAllBad() {
        for (int i = 0; i < tree.size(); i++) {
            determineBad(i);
        }
    }
    
    void countAllGo() {
        for (int i = 0; i < tree.size(); i++) {
            for (int c = 0; c < ALPHABET_SIZE; c++) {
                tree[i].go[c] = go(i, c);
            }
        }
    }
    
    void print() {
        for (int i = 0; i < tree.size(); i++) {
            cout << "Number: " << i << endl;
            cout << "Is a leaf: " << tree[i].leaf << endl;
            cout << "Parent number: " << tree[i].parent << endl;
            cout << "Edge char from parent: " << (char) (tree[i].parentChar + '0') << endl;
            cout << "Suffix link is number: " << tree[i].link << endl;
            cout << "Is this vertex bad :" << tree[i].isBad << endl;
        }
    }
    
    int dfs(int v) {
        if (used[v] != -1) {
            return used[v];
        }
        if (tree[v].isBad) {
            used[v] = 0;
            return 0;
        }
        used[v] = 1;
        used[v] = dfs(go(v, 0)) || dfs(go(v, 1));
        return used[v];
    }
    
};

int Bor::go(int v, int c) {
    if (tree[v].go[c] == -1) {
        if (tree[v].child[c] != -1) {
            tree[v].go[c] = tree[v].child[c];
        } else {
            tree[v].go[c] = (v == 0) ? (0) : (go(getLink(v), c));
        }
    }
    return tree[v].go[c];
}


int main(int argc, const char * argv[]) {
    
    //Philipp's magic cin boost.
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    //Code.
    cin >> n;
    Bor bor;
    
    for (int i = 0; i < n; i++) {
        cin >> w;
        bor.addString(w);
    }
    
    bor.countAllLinks();
    bor.determineAllBad();
    bor.countAllGo();
    
    cout << (bor.dfs(0) ? "TAK" : "NIE");
    
    return 0;
}

