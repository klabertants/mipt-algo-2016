////
////  SenichSolution.cpp
////  string
////
////  Created by Дмитрий Ткаченко on 04/12/2016.
////  Copyright © 2016 BillionariesClub. All rights reserved.
////
//
//#include <iostream>
//#include <vector>
//#include <cmath>
//
//using namespace std;
//
//int z[20001];
//
//void z_function (const string &s, int offset) {
//    int n = s.length() - offset;
//    z[0] = n;
//    for (int i = 1, l = 0, r = 0; i < n; ++i) {
//        z[i] = 0;
//        if (i <= r)
//            z[i] = min (r - i + 1, z[i - l]);
//        while (i + z[i] < n && s[offset + z[i]] == s[offset + i + z[i]])
//            ++z[i];
//        if (i + z[i] - 1 > r)
//            l = i, r = i + z[i] - 1;
//    }
//}
//
//vector<vector<short> > dp;
//string s;
//
//int main() {
//    std::ios_base::sync_with_stdio(0);
//    std::cin.tie(0);
//    
//    cin >> s;
//    int n = s.length();
//    s = s + "$" + s;
//    
//    dp.resize(n);
//    for (int i = 0; i < n; ++i) {
//        dp[i].resize(n - i + 1, 1);
//    }
//    int bestPos = 0;
//    int bestLen = 0;
//    int ans = n;
//    for (int i = 0; i < n; ++i) {
//        int pos = i - 1;
//        z_function(s, i);
//        
//        for (int len = 1; len <= min(i, n - i); ++len) {
//            if (pos < 0) {
//                break;
//            } else {
//                pos = min(pos, i - len);
//                while (pos >= 0) {
//                    if (z[n - i + pos + 1] >= len) {
//                        break;
//                    }
//                    pos--;
//                }
//                dp[i][len] = pos < 0 ? 1 : 1 + dp[pos][len];
//                int curr = n + len - (len - 1) * dp[i][len];
//                if (curr < ans) {
//                    ans = curr;
//                    bestPos = i;
//                    bestLen = len;
//                }
//            }
//        }
//    }
//    
//    for (int i = bestPos; i < bestLen + bestPos; ++i) {
//        cout << s[i];
//    }
//    cout << endl;
//    if (bestLen == 0) {
//        for (int i = 0; i < n; ++i) {
//            cout << s[i];
//        }
//    } else {
//        z_function(s, bestPos);
//        for (int i = n - bestPos + 1; i < 2 * n - bestPos + 1; ++i) {
//            if (z[i] >= bestLen) {
//                cout << "A";
//                i += bestLen - 1;
//            } else {
//                cout << s[i - (n - bestPos + 1)];
//            }
//        }
//    }
//    
//    cout << endl;
//    
//    return 0;
//}
