#include <iostream>
#include <vector>

int main ()
{
    int n;
    std::cin >> n;
    std::string str;
    std::cin >> str;
    str = str + "#";
    std::vector<int> sufArr (n);
    std::vector<int> reverseSufArr (n);
    std::vector<int> lcp (n);
    for (int i = 0;i < n;++i){
        std::cin >> sufArr[i];
        --sufArr[i];
        reverseSufArr[sufArr[i]] = i;
    }
    int tmp = 0;
    for (int i = 0;i < n;++i){
        if (tmp > 0){
            --tmp;
        }
        if (reverseSufArr[i] == n - 1){
            lcp[n - 1] = -1;
            tmp = 0;
        }
        else{
            int j = sufArr[reverseSufArr[i] + 1];
            while (i + tmp < n && j + tmp < n &&
                    str[i + tmp] == str[j + tmp]){
                ++tmp;
            }
            lcp[reverseSufArr[i]] = tmp;
        }
    }
    for (int i = 0;i < n - 1;++i){
        std::cout << lcp[i] << " ";
    }
    std::cout << std::endl;
}