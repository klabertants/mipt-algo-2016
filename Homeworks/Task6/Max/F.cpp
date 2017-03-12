#include <vector>
#include <iostream>
 
const int MAXN = 200015;
const int ALPHABET_SIZE = 26;

class PalindromicTree
{
    std::vector<std::vector<int> > next;
    std::vector<int> cnt, fail, len, str, num;
    int tot, last, n;
public:
    int newnode(int length)
    {
        for (int i = 0;i < ALPHABET_SIZE;++i) {
            next[tot][i] = 0;
        }
        len[tot] = length;
        cnt[tot] = num[tot] = 0;
        return tot++;
    }
    int get_fail(int x)
    {
        while(str[n - len[x] - 1] != str[n]) {
            x = fail[x];
        }
        return x;
    }
    void init()
    {
        next.resize(MAXN);
        for (int i = 0;i < MAXN;++i){
            next[i].resize(ALPHABET_SIZE);
        }
        cnt.resize(MAXN);
        fail.resize(MAXN);
        len.resize(MAXN);
        str.resize(MAXN);
        num.resize(MAXN);
        last = 0;
        tot = 0;
        n = 0;
        newnode(0);
        newnode(-1);
        fail[0] = 1;
        str[n] = -1;
    }
    void count()
    {
        for(int i = tot - 1; i >= 0; --i) {
            cnt[fail[i]] += cnt[i];
        }
    }
    void add(int c)
    {
        c -= 'a';
        str[++n] = c;
        int cur = get_fail(last);
        if(!next[cur][c]) {
            int now = newnode(len[cur] + 2);
            fail[now] = next[get_fail(fail[cur])][c];
            next[cur][c] = now;
            num[now] = num[fail[now]] + 1;
        }
        last = next[cur][c];
        cnt[last]++;
    }
    int getNext (int i, int j)
    {
        return next[i][j];
    }
    int getCnt(int i)
    {
        return cnt[i];
    }
};

PalindromicTree a, b;

long long result;
void dfs(int u,int v)
{
    for(int i = 0; i < ALPHABET_SIZE; ++i) {
        int x = a.getNext(u, i);
        int y = b.getNext(v, i);
        if(x && y) {
            result += (long long)a.getCnt(x) * b.getCnt(y);
            dfs(x,y);
        }
    }
}

int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    int t;
    std::string str1, str2;
    std::cin >> t;
    for (int i = 0;i < t;++i) {
        a.init();
        b.init();
        std::cin >> str1;
        for(int j = 0; j < str1.size(); ++j) {
            a.add(str1[j]);
        }
        std::cin >> str2;
        for(int j = 0;j < str2.size(); ++j) {
            b.add(str2[j]);
        }
        a.count();
        b.count();
        result = 0;
        dfs(0,0);
        dfs(1,1);
        std::cout << "Case #" << i + 1 << ": " << result << "\n";
    }
    return 0;
}