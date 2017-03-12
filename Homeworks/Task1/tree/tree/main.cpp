//
//  main.cpp
//  tree
//
//  Created by Дмитрий Ткаченко on 06/09/16.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//

#include <iostream>
#include <atomic>
#include <array>
#include <vector>
#include <thread>

class peterson_mutex {
public:
    
    peterson_mutex() {
        want[0].store(false);
        want[1].store(false);
        victim.store(0);
    }
    
    void lock(int t) {
        want[t].store(true);
        victim.store(t);
        while (want[1 - t].load() && victim.load() == t) {
            std::this_thread::yield();
        }
    }
    
    void unlock(int t) {
        want[t].store(false);
    }
    
private:
    std::array<std::atomic<bool>, 2> want;
    std::atomic<int> victim;
};

class tree_mutex {
public:
    
    tree_mutex(std::size_t num_threads) {
        peterson_mutex pm;
        buf.assign(num_threads - 1, pm);
    }
    
    void lock(std::size_t thread_index) {
        //Сохраняем путь от потока до вершины дерева (а также каким сыном поток является).
        std::vector<std::pair<int, int>> path;
        std::size_t new_ind = thread_index + buf.size();
        
        while (new_ind > 0) {
            path.push_back(std::make_pair( (new_ind - 1) / 2, 1 - ((new_ind) % 2)));
            new_ind = (new_ind - 1) / 2;
        }
        
        //Захватываем все мьютексы на пути.
        for (std::size_t i = 0; i < path.size(); i++) {
            buf[path[i].first].lock(path[i].second);
        }
        
    }
    
    void unlock(std::size_t thread_index) {
        //Сохраняем путь от потока до вершины дерева (а также каким сыном поток является).
        std::vector<std::pair<int, int>> path;
        std::size_t new_ind = thread_index + buf.size();
        
        while (new_ind > 0) {
            path.push_back(std::make_pair( (new_ind - 1) / 2, 1 - ((new_ind) % 2)));
            new_ind = (new_ind - 1) / 2;
        }
        
        //Разблокировываем все мьютексы на пути.
        for (std::size_t i = path.size() - 1; i > 0; i++) {
            buf[path[i].first].unlock(path[i].second);
        }
    }
    
private:
    //Линейная развертка бинарного дерева мьютексов.
    std::vector<peterson_mutex> buf;
};

int main() {
    return 0;
}
