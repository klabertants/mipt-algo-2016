#include <iostream>
#include <map>
#include <memory>
#include <vector>

using namespace std;



char str[150005];

int main() {
    
    size_t n, m, c;
    cin >> n >> m;
    for (size_t i = 0; i < n; ++i) {
        cin >> c;
        str[i] = (char) (c + 'a');
    }
    SuffArray sa(string(str, n));
    SuffTree st(sa);
    
    auto ans = st.get_refref();
    cout << get<0>(ans) << "\n" << get<2>(ans) << "\n";
    for (int i = 0; i < get<2>(ans); ++i) {
        cout << str[get<1>(ans) + i] - 'a' << " ";
    }
    
    return 0;
}
