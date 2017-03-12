//#include <bits/stdc++.h>
//#include <stdlib.h>
//using namespace std;
//
//#ifdef LOCAL
//#include "/home/dima/C++/debug.h"
//#else
//void nop() {}
//#define dbg(...) nop();
//#define dbgt(...) nop();
//#endif
//
//vector<int> zfunc(string s) {
//	int n = s.length();
//	vector<int> z(n);
//	z[0] = n;
//
//	int maxi = -1;
//	for (int i = 1; i < n; ++i) {
//		int max_i_plus_zi = maxi + z[maxi];
//		z[i] = (maxi != -1 && i < max_i_plus_zi) ? min(z[i - maxi], max_i_plus_zi - i) : 0;
//		while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
//			++z[i];
//		}
//		if (maxi == -1 || maxi + z[maxi] < i + z[i]) {
//			maxi = i;
//		}
//	}
//	return z;
//}
//
//int main() {
//#ifdef LOCAL
//	freopen("input.txt", "r", stdin);
////	freopen("output.txt", "w", stdout);
//#endif
//	string s;
//	cin >> s;
//	int n = s.length();
//	dbg(s, n);
//
//	int ans = 0;
//	for (int i = 0; i < n; ++i) {
//		vector<int> z = zfunc(s.substr(i) + "#" + s);
//		int i0 = (n - i) + 1;
//		int maxRight = 0;
//		int maxLeft = 0;
//		for (int j = i0; j < i0 + i; ++j) {
//			maxLeft = max(maxLeft, z[j]);
//		}
//		for (int len = n - i; len > 0; --len) {
//			if (len < n - i) {
//				maxRight = max(maxRight, z[i0 + i + len]);
//			}
//			bool ok = maxLeft < len && maxRight >= len;
//			ans += ok;
//		}
//	}
//	cout << ans << endl;
//	return 0;
//}
