#include <iostream>
#include <vector>

int main ()
{
    int n;
    std::cin >> n;
    std::vector<int> piArr;
    std::vector<int> zArr;
    piArr.resize(n);
    zArr.resize(n);
    for (int i = 0;i < n;++i){
        std::cin >> piArr[i];
    }
    for (int i = 1;i < n;++i){
        if (piArr[i] > 0){
            zArr[i + 1 - piArr[i]] = piArr[i];
        }
    }
    zArr[0] = n;
    int i = 1;
    while (i < n){
        int jump = i;
        if (zArr[i] > 0){
            for (int j = 1;j < zArr[i];++j){
                if (zArr[i + j] > zArr[j]){
                    break;
                }
                zArr[i + j] = std::min(zArr[j], zArr[i] - j);
                jump = i + j;
            }
        }
        i = jump + 1;
    }
    for (i = 0;i < n;++i){
        std::cout << zArr[i] << " ";
    }
    std::cout << std::endl;
}