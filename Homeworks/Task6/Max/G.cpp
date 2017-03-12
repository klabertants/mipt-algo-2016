#include <iostream>
#include <ext/pb_ds/assoc_container.hpp>

using namespace __gnu_pbds;


int main ()
{
    int n;
    std::cin >> n;
    tree<std::string, null_type, std::less<std::string>, rb_tree_tag, tree_order_statistics_node_update> bor;
    std::string input;
    for (int i = 0;i < n;++i){
        std::cin >> input;
        if (input[0] >= '0' && input[0] <= '9'){
            int ind = std::stoi(input);
            std::cout << *(bor.find_by_order(ind - 1)) << std::endl;
        }
        else{
            bor.insert(input);
        }
    }
    return 0;
}
