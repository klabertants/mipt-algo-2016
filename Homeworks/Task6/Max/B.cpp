#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> prefixFunc (std::string str)
{
    int n = str.size();
    std::vector<int> pi(n);
    for (int i = 1;i < n;++i){
        int j = pi[i - 1];
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


std::vector<int> zFunc(std::string str)
{
    std::vector<int> zArr (str.size());
    int left = 0;
    int right = 0;
    zArr[0] = str.size();
    for (int i = 1;i < str.size();++i) {
        zArr[i] = std::max(0, std::min(right - i, zArr[i - left]));
        while (i + zArr[i] < str.size() && str[zArr[i]] == str[i + zArr[i]]) {
            ++zArr[i];
        }
        if (i + zArr[i] >= right) {
            left = i;
            right = i + zArr[i];
        }
    }
    return zArr;
}


int main ()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::string str;
    std::cin >> str;
    if (str[0] == '1'){
        std::cout << "1.(0)" << std::endl;
        return 0;
    }
    std::string reverseString = str.substr(2, str.size() - 2);
    std::reverse(reverseString.begin(), reverseString.end());
    std::vector<int> zArr = zFunc(reverseString);
    std::vector<int> prefArr = prefixFunc(reverseString);
    std::size_t begOfPeriod = 0;
    std::size_t minLength = reverseString.size();

    for (std::size_t i = 1; i < reverseString.size(); ++i) {
        std::size_t k = (i + 1) - prefArr[i];
        std::size_t j = i;
        if (i < reverseString.size() - 1) {
            j += zArr[i + 1];
        }
        if ((i + 1) % k == 0) {
            if (k + reverseString.size() - (j + 1) < minLength) {
                minLength = k + reverseString.size() - (j + 1);
                begOfPeriod = j;
            }
        }
        else {
            if ((i + 1) + reverseString.size() - (j + 1) < minLength) {
                minLength = (i + 1) + reverseString.size() - (j + 1);
                begOfPeriod = j;
            }
        }
    }
    std::cout << str[0] << str[1];
    begOfPeriod = str.size() - 1 - begOfPeriod;
    minLength += 2;
    for (int i = 2;i < begOfPeriod;++i){
        std::cout << str[i];
    }
    std::cout << '(';
    for (int i = begOfPeriod;i < minLength;++i){
        std::cout << str[i];
    }
    std::cout << ')';
    std::cout << std::endl;
    return 0;
}