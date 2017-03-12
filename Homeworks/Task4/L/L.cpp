#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <set>


class Algorithm
{
public:
    Algorithm();
    int getLeftSize();
    int getRightSize();
    void getInput(std::istream& in);
    void clear();
    std::pair<std::set<int>, std::set<int> > findControlSet();
private:
    bool matchVisit(int v);
    void controlVisit(int v);
    void transitiveClosure();

    std::vector<char> right;
    std::vector<char> used;
    std::vector<int> match;
    std::vector<std::vector<char> > graph;
    std::size_t leftSize, rightSize;
};


int main ()
{
    Algorithm algorithm;
    algorithm.getInput(std::cin);
    std::pair<std::set<int>, std::set<int> > tmp = algorithm.findControlSet();
    std::vector<int> ans;
    int n = algorithm.getLeftSize();
    for (int i = 0;i < n;++i){
        if (tmp.first.count(i) == 0 && tmp.second.count(i) == 0){
            ans.push_back(i);
        }
    }
    std::cout << ans.size() << std::endl;
    for (int i = 0;i < ans.size();++i){
        std::cout << ans[i] + 1 << " ";
    }
    std::cout << std::endl;
    algorithm.clear();
    return 0;
}

Algorithm::Algorithm() {}

int Algorithm::getLeftSize()
{
    return leftSize;
}

int Algorithm::getRightSize()
{
    return rightSize;
}

void Algorithm::getInput(std::istream& in)
{
    in >> leftSize;
    rightSize = leftSize;
    graph.resize(leftSize);
    for (int i = 0; i < leftSize; ++i) {
        graph[i].resize(leftSize);
    }
    for (int i = 0; i < leftSize; ++i) {
        for (int j = 0; j < rightSize; ++j) {
            in >> graph[i][j];
        }
    }
}

void Algorithm::clear()
{
    leftSize = 0;
    rightSize = 0;
    right.clear();
    used.clear();
    match.clear();
    for (int i = 0;i < graph.size();i++){
        graph[i].clear();
    }
    graph.clear();
}

std::pair<std::set<int>, std::set<int> >
Algorithm::findControlSet ()
{
    transitiveClosure();

    match.assign(rightSize, -1);
    used.assign(leftSize, 0);

    for (int i = 0;i < leftSize;i++){
        used.assign(leftSize, 0);
        matchVisit(i);
    }
    std::vector<char> free;
    free.assign(leftSize, 1);
    for (int i = 0;i < rightSize;i++){
        if (match[i] != -1){
            free[match[i]] = false;
        }
    }
    right.assign(rightSize, 0);
    used.assign(leftSize, 0);
    for (int i = 0;i < leftSize;i++) {
        if (free[i]){
            controlVisit(i);
        }
    }
    std::pair<std::set<int>, std::set<int> > res;
    for (int i = 0;i < rightSize;i++){
        if (match[i] != -1){
            if (right[i]){
                res.second.insert(i);
            }
            else{
                res.first.insert(match[i]);
            }
        }
    }
    return res;
}



bool Algorithm::matchVisit(int v)
{
    used[v] = 1;
    for (int i = 0;i < leftSize;i++){
        if (graph[v][i] == '1') {
            if (match[i] == -1 ||
                (!used[match[i]] && matchVisit(match[i]))) {
                match[i] = v;
                return true;
            }
        }
    }
    return false;
}

void Algorithm::controlVisit(int v)
{
    used[v] = 1;
    for (int i = 0;i < graph[v].size();i++){
        if (graph[v][i] == '1') {
            if (!right[i] && !used[match[i]]) {
                controlVisit(match[i]);
                right[i] = true;
            }
        }
    }
}

void Algorithm::transitiveClosure()
{
    for (int k = 0;k < leftSize;++k){
        for (int i = 0;i < leftSize;++i){
            for (int j = 0; j < leftSize; ++j) {
                if (graph[i][j] == '0'){
                    if (graph[i][k] == graph[k][j] && graph[i][k] == '1'){
                        graph[i][j] = '1';
                    }
                }
            }
        }
    }
}