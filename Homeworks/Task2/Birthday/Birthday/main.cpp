//
//  main.cpp
//  Birthday
//
//  Created by Дмитрий Ткаченко on 18/09/16.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

class Graph{
public:
    vector<vector<int>> buff;
    int left, right;
    
    void clear() {
        buff.clear();
    }
    
    void resize(int size) {
        buff.resize(size);
    }
    
    bool incid(int l, int r) {
        bool ans = false;
        for (int i = 0; i < buff[l].size(); i++) {
            if (buff[l][i] == r) {
                ans = true;
            }
        }
        return ans;
    }
    
    pair<set<int>, set<int>> control() {
        int i;
        
        cmpr.assign(right, -1);
        was.assign(left, false);
        
        for(i = 0; i < left; i++) {
            was.assign(left, false);
            cmprVisit(i);
        }
        
        vector<bool> isFree(left, true);
        for(i = 0; i < right; i++)
            if(cmpr[i] != -1)
                isFree[cmpr[i]] = false;
        
        Right.assign(right, false);
        was.assign(left, false);
        for(i = 0; i < left; i++)
            if(isFree[i])
                controlVisit(i);
        
        pair<set<int>, set<int>> result;
        for(i = 0; i < right; i++)
            if(cmpr[i] != -1) {
                if(Right[i])
                    result.second.insert(i);
                else
                    result.first.insert(cmpr[i]);
            }
        return result;
    }
    
private:
    
    bool cmprVisit(int u) {
        was[u] = true;
        for(vector<int>::iterator it = buff[u].begin(); it != buff[u].end(); it++)
            if(cmpr[*it] == -1 || (!was[cmpr[*it]] && cmprVisit(cmpr[*it]))) {
                cmpr[*it] = u;
                return true;
            }
        return false;
    }
    
    void controlVisit(int u) {
        was[u] = true;
        for(vector<int>::iterator it = buff[u].begin(); it != buff[u].end(); it++)
            if(!Right[*it] && !was[cmpr[*it]]) {
                controlVisit(cmpr[*it]);
                Right[*it] = true;
            }
    }
    
    vector<int> cmpr;
    vector<bool> was, Right;
    
};

int test_count, boys_count, girls_count, tmp_id;

int main(int argc, const char * argv[]) {
    
    cin >> test_count;
    
    //Read vars.
    for (int i = 0; i < test_count; i++) {
        Graph graph, antigraph;
        cin >> boys_count >> girls_count;
        antigraph.resize(boys_count);
        antigraph.left = boys_count;
        antigraph.right = girls_count;
        graph.resize(boys_count);
        graph.left = boys_count;
        graph.right = girls_count;
        for (int j = 0; j < boys_count; j++) {
            tmp_id = 1;
            while (tmp_id >= 0) {
                cin >> tmp_id;
                tmp_id--;
                if (tmp_id >= 0) {
                    graph.buff[j].push_back(tmp_id);
                }
            }
        }
        //Making antigraph.
        for (int j = 0; j < boys_count; j++) {
            for (int k = 0; k < girls_count; k++) {
                if (!graph.incid(j, k)) {
                    antigraph.buff[j].push_back(k);
                }
            }
        }
        pair<set<int>, set<int>> ans;
        set<int> lans, rans;
        ans = antigraph.control();
        for (int j = 0; j < boys_count; j++) {
            if (ans.first.find(j) == ans.first.end()) {
                lans.insert(j);
            }
        }
        for (int j = 0; j < girls_count; j++) {
            if (ans.second.find(j) == ans.second.end()) {
                rans.insert(j);
            }
        }
        cout << lans.size() + rans.size() << endl << lans.size() << " " << rans.size() << endl;
        for (auto k : lans) {
            cout << k + 1 << " ";
        }
        cout << endl;
        for (auto k : rans) {
            cout << k + 1 << " ";
        }
        cout << endl;
    }
    
}
