#include <stdlib.h>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

string replaceAll(string source, string find, string replace) {
	for (string::size_type i = 0; (i = source.find(find, i)) != string::npos;) {
		source.replace(i, find.length(), replace);
		i += replace.length();
	}
	return source;
}

const int N = 10010;
int z[N * 2];
int16_t number[N * N / 2];

void zfunc2(const char *s0, int i0, int n) {
	const char *s = s0 + i0;
	z[0] = n;
	for (int i = 1, l = 0, r = 0; i < n; ++i) {
		int zi = i <= r ? min(r - i + 1, z[i - l]) : 0;
		while (i + zi < n && s[zi] == s[i + zi])
			++zi;
		if (i + zi - 1 > r)
			l = i, r = i + zi - 1;
		z[i] = zi;
	}
}

int find(int left, int right, int len) {
	for (int i = right; i >= left; --i) {
		if (z[i] >= len) {
			return i;
		}
	}
	return -1;
}

int main() {
	string s;
	cin >> s;
	int n = (int) s.length();
	string ss0 = s + "#" + s;
	const char *ss = ss0.c_str();

	int minsum = INT_MAX;
	pair<int, int> ans = {-1, -1};

	for (int i = 0; i < n; ++i) {
		int k0 = ((n + 1) + (n - i + 2)) * i / 2;
		zfunc2(ss, i, n * 2 + 1 - i);
		int i0 = (n - i) + 1;
		int last = find(i0, i0 + i - 1, 1);
		for (int len = 1; len <= n - i; ++len) {
			int j = last - i0;
			number[k0 + len] = last == -1 ? 1 : (number[(n * 2 + 3 - j) * j / 2 + len] + 1);
			int k = number[k0 + len];
			int csum = len + (n - (len - 1) * k);
			if (minsum > csum) {
				minsum = csum;
				ans = {i, len};
			}
			last = last == -1 ? -1 : find(i0, min(last, i0 + i - len - 1), len + 1);
		}
	}

	int i = ans.first;
	int len = ans.second;
	string t = s.substr(i, len);
	if (t.length() == 1) {
		cout << endl;
		cout << s << endl;
	} else {
		cout << t << endl;
		cout << replaceAll(s, t, "A") << endl;
	}
	return 0;
}
