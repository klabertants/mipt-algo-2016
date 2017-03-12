#include <iostream>
#include <vector>
#include <algorithm>

std::vector<size_t> prefixFunc (std::string str)
{
    size_t n = str.size();
    std::vector<size_t> pi(n);
    for (int i = 1;i < n;++i){
        size_t j = pi[i - 1];
        while (j > 0 && str[i] != str[j]){
            j = pi[j - 1];
        }
        if (str[i] == str[j]){
            ++j;
        }
        pi[i] = j;
    }
    return pi;
}


/*std::vector<size_t> zFunc(std::string str)
{
    std::vector<size_t> zArr (str.size());
    size_t left = 0;
    size_t right = 0;
    zArr[0] = str.size();
    for (size_t i = 1;i < str.size();++i) {
        zArr[i] = std::max(static_cast<size_t>(0), std::min(right - i, zArr[i - left]));
        while (i + zArr[i] < str.size() && str[zArr[i]] == str[i + zArr[i]]) {
            ++zArr[i];
        }
        if (i + zArr[i] >= right) {
            left = i;
            right = i + zArr[i];
        }
    }
    return zArr;
}*/


int main ()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::string str;
    std::string subStr;
    std::vector<size_t> piArr;
    size_t countOfDifferentStrings = 0;
    std::cin >> str;
    for (size_t i = 1;i <= str.size();++i){//length of substr
        subStr = str.substr(0, i);
        std::reverse(subStr.begin(), subStr.end());
        piArr = prefixFunc(subStr);
        size_t maxPi = 0;
        for (size_t j = 0;j < piArr.size();++j){
            maxPi = std::max(maxPi, piArr[j]);
        }
        countOfDifferentStrings += i - maxPi;
    }
    size_t countOfUniqueStrings = 0;
    std::vector<char> used;
    std::vector<size_t> lengths(str.size(), 0);
    for (size_t i = 1;i <= str.size();++i){
        used.assign(str.size(), 0);
        used[0] = 1;
        subStr = str.substr(0, i);
        std::reverse(subStr.begin(), subStr.end());
        piArr = prefixFunc(subStr);
        for (size_t j = i - 1;j > 0;--j){
            size_t index = j;
            while (used[index] == 0) {
                lengths[i + piArr[index] - j - 2] = std::max(lengths[i + piArr[index] - j - 2],
                                                             std::min(piArr[index], j + 1 - piArr[index]));
                used[index] = 1;
                index = piArr[index];
                if (index <= 1){
                    break;
                }
                --index;
            }
            lengths[i - 1] = std::max(piArr[j], lengths[i - 1]);
        }
    }
    countOfUniqueStrings = str.size() * (str.size() + 1);
    countOfUniqueStrings /= 2;
    for (size_t i = 0;i < str.size();++i){
        countOfUniqueStrings -= lengths[i];
    }
    std::cout << countOfDifferentStrings - countOfUniqueStrings << std::endl;
    return 0;
}