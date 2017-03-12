//
//  main.cpp
//  Hungary
//
//  Created by Дмитрий Ткаченко on 05/10/2016.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//

#include <iostream>
#include <vector>

const int INF = 1e6 + 1;
int n;
int a[310][310];

using namespace std;

int main() {
    cin >> n;
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> a[i][j];
        }
    }

    vector<int> rowPot(n + 1), colPot(n + 1), matching(n + 1), way(n +1 );
    
    for (int i = 1; i <= n; i++) {
        int freeColumn = 0;
        matching[0] = i;
        vector<int> minv(n + 1, INF);
        vector<bool> used(n + 1, false);
        
        do {
            used[freeColumn] = true;
            int i0 = matching[freeColumn],  delta = INF,  reachableMin;
            for (int j = 1; j <= n; j++)
                if (!used[j]) {
                    int cur = a[i0][j]-rowPot[i0]-colPot[j];
                    if (cur < minv[j])
                        minv[j] = cur,  way[j] = freeColumn;
                    if (minv[j] < delta)
                        delta = minv[j],  reachableMin = j;
                }
            for (int j = 0; j <= n; j++)
                if (used[j])
                    rowPot[matching[j]] += delta,  colPot[j] -= delta;
                else
                    minv[j] -= delta;
            freeColumn = reachableMin;
        } while (matching[freeColumn] != 0);
        
        do {
            int reachableMin = way[freeColumn];
            matching[freeColumn] = matching[reachableMin];
            freeColumn = reachableMin;
        } while (freeColumn);
        
    }
    
    vector<int> ans (n + 1);
    for (int j = 1; j <= n; ++j)
        ans[matching[j]] = j;
    
    cout << -colPot[0] << endl;
    for (int i = 1; i <= n; i++) {
        cout << i << " " << ans[i] << endl;
    }
    return 0;
}
