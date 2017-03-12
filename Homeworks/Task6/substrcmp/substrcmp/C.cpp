#include <iostream>
#include <vector>

const long long MOD = static_cast<int> (1e9 + 7);
const long long MULTIPLIER = 31;
std::vector<long long> prefHashes;
std::vector<long long> multiplierPows;
std::string str;

void precalc ()
{
    multiplierPows.resize(str.size());
    multiplierPows[0] = 1;
    for (int i = 1;i < str.size();++i){
        multiplierPows[i] = multiplierPows[i - 1] * MULTIPLIER;
        multiplierPows[i] %= MOD;
    }
    prefHashes.resize(str.size());
    prefHashes[0] = str[0] - 'a' + 1;
    for (int i = 1;i < str.size();++i){
        prefHashes[i] = prefHashes[i - 1] + (str[i] - 'a' + 1) * multiplierPows[i];
        prefHashes[i] %= MOD;
    }
}

int main ()
{
    std::cin >> str;
    precalc ();
    int m;
    std::cin >> m;
    for (int i = 0;i < m;++i){
        int a, b, c, d;
        //std::cin >> a >> b >> c >> d;
        scanf ("%d%d%d%d", &a, &b, &c, &d);
        --a;
        --b;
        --c;
        --d;
        if (d - c != b - a){
            //std::cout << "No" << std::endl;
            printf ("No\n");
            continue;
        }
        long long res1, res2;
        if (a == 0){
            res1 = prefHashes[b];
        }
        else{
            res1 = prefHashes[b] - prefHashes[a - 1] + MOD;
        }
        if (c == 0){
            res2 = prefHashes[d];
        }
        else {
            res2 = prefHashes[d] - prefHashes[c - 1] + MOD;
        }
        res1 %= MOD;
        res2 %= MOD;
        res1 *= multiplierPows[c];
        res2 *= multiplierPows[a];
        if (res1 % MOD == res2 % MOD){
            //std::cout << "Yes" << std::endl;
            printf("Yes\n");
        }
        else{
            //std::cout << "No" << std::endl;
            printf("No\n");
        }
    }
}