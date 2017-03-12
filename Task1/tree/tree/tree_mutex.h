//
//  tree_mutex.h
//  tree
//
//  Created by Дмитрий Ткаченко on 06/09/16.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//

//#ifndef tree_mutex_h
//#define tree_mutex_h

//#include <atomic>
//#include <array>
//#include <vector>
//#include <algorithm>
//#include <thread>

//Мьютекс Петерсона (код с семинара).
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

//Турнирное дерево мьютексов.
class tree_mutex {
public:
    
    //Инициализация дерева.
    tree_mutex(std::size_t num_threads) :   buf(power(num_threads) - 1) {};

    
    void lock(std::size_t thread_index) {
        //Сохраняем путь от потока до вершины дерева (а также каким сыном поток является).
        std::size_t new_ind = buf.size() + thread_index;
        std::vector<std::pair<int, int>> path = findPath(new_ind);
        
        //Захватываем все мьютексы на пути.
        for (std::size_t i = 0; i < path.size(); i++) {
            buf[path[i].first].lock(path[i].second);
        }
        
    }
    
    void unlock(std::size_t thread_index) {
        //Сохраняем путь от потока до вершины дерева (а также каким сыном поток является).
        std::size_t new_ind = buf.size() + thread_index;
        std::vector<std::pair<int, int>> path = findPath(new_ind);
        
        //Разблокировываем все мьютексы на пути.
        for (int i = path.size() - 1; i >= 0; i--) {
            buf[path[i].first].unlock(path[i].second);
        }
    }
    
    
    
private:
    //Линейная развертка бинарного дерева мьютексов.
    std::vector<peterson_mutex> buf;
    //Служебные функции.
    int power(int x) {
        int i = 1;
        while (i < x) {
            i *= 2;
        }
        return i;
    }
    //Поиск пути
    std::vector<std::pair<int, int>> findPath(int x)  {
        std::vector<std::pair<int, int>> path;
        while (x > 0) {
            path.push_back(std::make_pair( (x - 1) / 2, 1 - ((x) % 2)));
            x = (x - 1) / 2;
        }
        return path;
    }
};


//#endif /* tree_mutex_h */
