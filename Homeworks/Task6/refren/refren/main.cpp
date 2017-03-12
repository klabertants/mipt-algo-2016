//
//  main.cpp
//  refren
//
//  Created by Дмитрий Ткаченко on 28/11/2016.
//  Copyright © 2016 BillionariesClub. All rights reserved.
//

//Includes block.
#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

typedef vector<int> vi;

#define forn(i, x) for (int i = 0; i < x; i++)

//Our great structures.
class SuffixArray {
public:
    int ALPHABET_SIZE;
    string s;
    vector<int> suff;
    vector<int> lcp;
    
    SuffixArray(string const& str) {
        ALPHABET_SIZE = 256;
        s = str + "$";
        int n = (int) s.length();
        
        vector<int> count(max(n, ALPHABET_SIZE), 0);
        vector<int> cl(s.length(), 0);
        
        for (int i = 0; i < n; ++i) {
            ++count[s[i]];
        }
        
        for (int i = 1; i < ALPHABET_SIZE; ++i) {
            count[i] += count[i - 1];
        }
        
        suff = vector<int>(s.length(), 0);
        for (int i = n - 1; i >= 0; --i) {
            suff[--count[s[i]]] = i;
        }
        
        cl[suff[0]] = 0;
        int classes = 1;
        for (int i = 1; i < n; ++i) {
            if (s[suff[i]] != s[suff[i - 1]]) {
                ++classes;
            }
            cl[suff[i]] = classes - 1;
        }
        
        vector<int> new_classes(s.length(), 0);
        vector<int> second(s.length(), 0);
        for (int cur_len = 1; cur_len < n; cur_len *= 2) {
            for (int i = 0; i < n; ++i) {
                second[i] = (suff[i] - cur_len + n) % n;
            }
            fill(count.begin(), count.end(), 0);
            for (int i = 0; i < n; ++i) {
                ++count[cl[second[i]]];
            }
            for (int i = 1; i < classes; ++i) {
                count[i] += count[i - 1];
            }
            for (int i = n - 1; i >= 0; --i) {
                suff[--count[cl[second[i]]]] = second[i];
            }
            
            new_classes[suff[0]] = 0;
            classes = 1;
            for (int i = 1; i < n; ++i) {
                int cur_mid = (suff[i] + cur_len) % n;
                int prev_mid = (suff[i - 1] + cur_len) % n;
                if ((cl[suff[i]] != cl[suff[i - 1]]) || (cl[cur_mid] != cl[prev_mid])) {
                    classes++;
                }
                new_classes[suff[i]] = classes - 1;
            }
            copy(new_classes.begin(), new_classes.end(), cl.begin());
        }
        make_lcp();
    }
    
    void make_lcp() {
        lcp = vector<int>(s.length());
        vector<int> rev_suff(s.length());
        int n = (int) s.length();
        
        for (int i = 0; i < n; ++i) {
            rev_suff[suff[i]] = i;
        }
        int k = 0;
        for (int i = 0; i < n; ++i) {
            if (k > 0) {
                --k;
            }
            if (rev_suff[i] == n - 1) {
                k = 0;
            } else {
                int j = suff[rev_suff[i] + 1];
                while (max(i + k, j + k) < n && s[i + k] == s[j + k]) {
                    ++k;
                }
                lcp[rev_suff[i]] = k;
            }
        }
        for (int i = n - 1; i > 0; --i) {
            lcp[i] = lcp[i - 1];
        }
        lcp[0] = 0;
    }
};

class SuffixTree {
public:
    struct Node;
    
    shared_ptr<Node> root;
    int size;
    string s;
    
    struct Edge {
        int l, r;
        shared_ptr<Node> child;
        
        Edge(int l, int r, shared_ptr<Node> child) : l(l), r(r), child(child) { }
    };
    
    struct Node {
        int index;
        int depth;
        weak_ptr<Node> parent;
        vector<Edge> children;
        
        Node(int index, int depth) : Node(index, nullptr, depth) { }
        
        Node(int index, shared_ptr<Node> parent, int depth) :
        index(index), depth(depth), parent(parent), children(vector<Edge>()) { }
        
        size_t count_leafs(int end, long long& refrain, int& pos, int& len) {
            size_t cnt = 0;
            
            if (children.size() == 0) {
                cnt = 1;
                --depth;
            }
            for (Edge& edge: children) {
                if (index != 1 || edge.l != end)
                    cnt += edge.child->count_leafs(end, refrain, pos, len);
            }
            
            long long cur = (long long) depth * (long long) cnt;
            if (refrain < cur) {
                refrain = cur;
                auto par = parent.lock();
                for (int i = 0; i < par->children.size(); ++i) {
                    if (par->children[i].child.get() == this) {
                        if (children.size() == 0) {
                            pos = par->children[i].r - depth;
                        } else {
                            pos = par->children[i].r - depth + 1;
                        }
                    }
                }
                len = depth;
            }
            if (children.size() == 0) {
                ++depth;
            }
            return cnt;
        }
    };
    
    shared_ptr<Node> pushSuffix(shared_ptr<Node> prev, int pos, int lcp) {
        int n = (int) s.length();
        if (prev->depth == 0 || prev->depth == lcp) {
            shared_ptr<Node> new_node(new Node(++size, prev, n - pos));
            prev->children.push_back(Edge(pos + lcp, n - 1, new_node));
            return shared_ptr<Node>(new_node);
        } else {
            auto parent = prev->parent.lock();
            if (parent->depth < lcp) {
                shared_ptr<Node> new_node(new Node(++size, parent, lcp));
                
                auto edge = parent->children[parent->children.size() - 1];
                parent->children.pop_back();
                parent->children.push_back(Edge(edge.l, edge.l + (lcp - parent->depth - 1), new_node));
                
                new_node->children.push_back(Edge(edge.l + (lcp - parent->depth), edge.r, edge.child));
                prev->parent = weak_ptr<Node>(new_node);
            }
            return pushSuffix(prev->parent.lock(), pos, lcp);
        }
    }
    
    SuffixTree(SuffixArray const& sa) : size(1), s(sa.s) {
        root = make_shared<Node>(1, 0);
        shared_ptr<Node> prev = root;
        for (size_t i = 0; i < sa.lcp.size(); ++i) {
            prev = pushSuffix(prev, sa.suff[i], sa.lcp[i]);
        }
    }
    
    tuple<long long, int, int> getRefren() {
        long long refrain = 0;
        int pos = 0;
        int len = 0;
        root->count_leafs((int) s.length() - 1, refrain, pos, len);
        tuple<long long, int, int> ans(refrain, pos, len);
        return ans;
    }
    
};

//Vars block.
char str[150005];
size_t n, m, c;

int main(int argc, const char * argv[]) {
    
    //Philipp's magic cin boost.
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    //Code.
    cin >> n >> m;
    for (size_t i = 0; i < n; i++) {
        cin >> c;
        str[i] = (char) (c + 'a');
    }
    SuffixArray sa(string(str, n));
    SuffixTree st(sa);
    
    auto ans = st.getRefren();
    cout << get<0>(ans) << "\n" << get<2>(ans) << "\n";
    for (int i = 0; i < get<2>(ans); ++i) {
        cout << str[get<1>(ans) + i] - 'a' << " ";
    }
    
    return 0;
}

