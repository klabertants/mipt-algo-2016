//
//  main.cpp
//  lock_free_stack
//
//  Created by Дмитрий Ткаченко on 03/12/2016.
//  Copyright © 2016 BillionariesClub. All rights reserved.
//

#include <iostream>
#include <thread>


#include <atomic>

template<typename T>
class lock_free_stack {
private:
    struct node;
    
    struct counted_node_ptr {
        int external_count;
        node* ptr;
    };
    
    struct node{
        std::shared_ptr<T> data;
        std::atomic<int> internal_count;
        counted_node_ptr next;
        node(T const& data_) :
            data(std::make_shared<T>(data_)),
            internal_count(0)
        {}
    };
    
    std::atomic<counted_node_ptr> head;
    
    void increase_head_count(counted_node_ptr& old_counter) {
        counted_node_ptr new_counter;
        
        do {
            new_counter = old_counter;
            ++new_counter.external_count;
        } while (!head.compare_exchange_strong(old_counter, new_counter, std::memory_order_acquire, std::memory_order_relaxed));
        
        old_counter.external_count = new_counter.external_count;
    }
    
public:
    ~lock_free_stack() {
        while(pop());
    }
    
    void push(T const& data) {
        counted_node_ptr new_node;
        new_node.ptr = new node(data);
        new_node.external_count = 1;
        new_node.ptr -> next = head.load(std::memory_order_relaxed);
        while (!head.compare_exchange_weak(new_node.ptr -> next, new_node, std::memory_order_release, std::memory_order_relaxed));
    }
    
    std::shared_ptr<T> pop() {
        counted_node_ptr old_head = head.load(std::memory_order_relaxed);
        for (;;) {
            increase_head_count(old_head);
            node* const ptr = old_head.ptr;
            if (!ptr) {
                return std::shared_ptr<T>();
            }
            if (head.compare_exchange_strong(old_head, ptr -> next, std::memory_order_relaxed)) {
                std::shared_ptr<T> res;
                res.swap(ptr->data);
                int const count_increase = old_head.external_count - 2;
                if (ptr -> internal_count.fetch_add(count_increase, std::memory_order_release) == -count_increase) {
                    delete ptr;
                }
                return res;
            } else {
                if (ptr -> internal_count.fetch_add(-1, std::memory_order_relaxed) == 1) {
                    ptr -> internal_count.load(std::memory_order_acquire);
                    delete ptr;
                }
            }
        }
    }
};

lock_free_stack<int> lfs;

void case1() {
    int p = 47;
    lfs.push(p);
    lfs.push(p);
    lfs.push(p);
}

void case2() {
    lfs.pop();
    lfs.pop();
}
int main() {
    std::thread th1(case1), th2(case2);
    
    th1.join(), th2.join();
    
    std::cout << *lfs.pop() << *lfs.pop();
    return 0;
}
