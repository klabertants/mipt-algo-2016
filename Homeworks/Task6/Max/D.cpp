#include <iostream>
#include <string>
#include <vector>

int main ()
{
    std::string str;
    std::cin >> str;
    size_t n = str.size();
    std::vector<int> back(n);
    std::vector<int> rev(n);
    std::vector<int> ans(n);
    int index = 0;
    for (int i = 1;i < n;++i){
        while (index > 0 && str[i] != str[index]) {
            index = back[index - 1];
        }
        if (str[i] == str[index]){
            ++index;
        }
        back[i] = index;
        if (index != 0 && rev[ans[index - 1]] + ans[index - 1] + 1 >= i){
            ans[i] = ans[index - 1];
            rev[ans[i]] = i;
        }
        else {
            ans[i] = i;
            rev[i] = i;
        }
    }
    for (auto i : ans) {
        std::cout << i + 1 << " ";
    }
    std::cout << std::endl;
    return 0;
}