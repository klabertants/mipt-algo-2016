//
//  main.cpp
//  matching
//
//  Created by Дмитрий Ткаченко on 21/10/2016.
//  CopyRight © 2016 Dmitry Tkachenko. All Rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int n, m;
vector<pair<int, int>> Left;
vector<int> Right;
vector<vector<pair<int, int>>> LeftEdges;
vector<pair<int, int>> pairToRight;
vector<int> location;
vector<pair<int, int>> Next;
vector<int> numMax;
vector<int> used;

bool try_kuhn (int v) {
    if (used[v] == true)  {
        return false;
    }
    used[v] = true;
    for (size_t i = 0; i < LeftEdges[v].size(); ++i) {
        int to = LeftEdges[v][i].first;
        if (pairToRight[to].first == -1) {
            if (numMax[v] == -1 || Right[numMax[v]] < Right[to]) {
                Next[v].first = to;
                Next[v].second = LeftEdges[v][i].second;
                numMax[v] = to;
            }
        } else if (try_kuhn (pairToRight[to].first)) {
            if (numMax[v] == -1 || Right[numMax[v]] < Right[numMax[pairToRight[to].first]]) {
                Next[v].first = to;
                Next[v].second = LeftEdges[v][i].second;
                numMax[v] = numMax[pairToRight[to].first];
            }
        }
    }
    if (numMax[v] != -1) {
        return true;
    } else {
        return false;
    }
}

int main() {
    int k;
    cin >> n >> m >> k;
    Right.resize(m);
    LeftEdges.resize(n);
    pairToRight.resize(m, make_pair(-1, -1));
    location.resize(n);
    Next.resize(n, make_pair(-1, -1));
    numMax.resize(n, -1);
    used.resize(n, false);
    
    for (int i = 0; i < n; ++i) {
        int a;
        cin >> a;
        Left.push_back(make_pair(a, i));
    }
    sort(Left.begin(), Left.end());
    reverse(Left.begin(), Left.end());
    for (int i = 0; i < n; ++i) {
        location[Left[i].second] = i;
    }
    for (int i = 0; i < m; ++i) {
        cin >> Right[i];
    }
    
    for (int i = 0; i < k; ++i) {
        int from, to;
        cin >> from >> to;
        from--;to--;
        LeftEdges[location[from]].push_back(make_pair(to, i + 1));
    }
    
    for (int i = 0; i < n; ++i) {
        if (try_kuhn (i)) {
            int j = i;
            while (pairToRight[Next[j].first].first != -1) {
                
                int k = pairToRight[Next[j].first].first;
                pairToRight[Next[j].first] = make_pair(j, Next[j].second);
                j = k;
            }
            pairToRight[Next[j].first] = make_pair(j, Next[j].second);
            
            Next.assign(n, make_pair(-1, -1));
            numMax.assign(n, -1);
            used.assign(n, -1);
        }
    }
    
    int cntAns = 0;
    int ans = 0;
    for (int i = 0; i < m; ++i) {
        if (pairToRight[i].first != -1) {
            ans += Right[i];
            ans += Left[pairToRight[i].first].first;
            cntAns++;
        }
    }
    
    cout << ans << "\n" << cntAns << "\n";
    
    for (int i = 0; i < m; ++i) {
        if (pairToRight[i].first != -1) {
            cout << pairToRight[i].second << " ";
        }
    }
    cout << "\n";
    
    return 0;
}
