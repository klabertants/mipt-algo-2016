//
//  main.cpp
//  nenokku
//
//  Created by Дмитрий Ткаченко on 13/12/2016.
//  Copyright © 2016 BillionariesClub. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

//Consts block.
const int MAX_SIZE = 1e5;

class SuffixAutomaton {

public:
    
    SuffixAutomaton() {
        states.resize(2 * MAX_SIZE);
        size = last = states[0].length = 0;
        states[0].link = -1;
        size++;
    }
    
    ~SuffixAutomaton() {
        states.clear();
    }
    
    void addSymbol(char& symbol) {
        int current = size++;
        states[current].length = states[last].length + 1;
        int pointer;
        for (pointer = last; pointer != -1 &&
             !states[pointer].nextState.count(symbol); pointer = states[pointer].link) {
            states[pointer].nextState[symbol] = current;
        }
        if (pointer == -1) {
            states[current].link = 0;
        } else {
            int nextPointer = states[pointer].nextState[symbol];
            if (states[pointer].length + 1 == states[nextPointer].length) {
                states[current].link = nextPointer;
            } else {
                int clone = size++;
                states[clone].length = states[pointer].length + 1;
                states[clone].nextState = states[nextPointer].nextState;
                states[clone].link = states[nextPointer].link;
                for (; pointer != -1 && states[pointer].nextState[symbol] == nextPointer; pointer = states[pointer].link) {
                    states[pointer].nextState[symbol] = clone;
                }
                states[nextPointer].link = states[current].link = clone;
            }
        }
        last = current;
    }
    
    bool checkEntry(string& s) {
        int currentState = 0;
        for (auto symbol : s) {
            if (!states[currentState].nextState.count(symbol)) {
                return false;
            } else {
                currentState = states[currentState].nextState[symbol];
            }
        }
        return true;
    }
    
    void addString(string& s) {
        for (auto &symbol : s) {
            addSymbol(symbol);
        }
    }
    
private:
    
    struct state {
        int length;
        int link;
        map<char, int> nextState;
    };
    
    vector<state> states;
    int size;
    int last;
};

//Helpful function.
void toLowerCase(string& s) {
    for (auto &symbol : s) {
        symbol = (char)tolower(symbol);
    }
}

//Vars block.
char operation;
string keyword;
SuffixAutomaton suffixAutomaton;

int main() {
    
    //Philipp's magic cin boost.
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    while (cin >> operation) {
        cin >> keyword;
        toLowerCase(keyword);
        switch (operation) {
            case '?' : {
                if (suffixAutomaton.checkEntry(keyword)) {
                    printf("YES\n");
                } else {
                    printf("NO\n");
                }
                break;
            }
            case 'A' : {
                suffixAutomaton.addString(keyword);
                break;
            }
            default : break;
        }
    }
    return 0;
}
