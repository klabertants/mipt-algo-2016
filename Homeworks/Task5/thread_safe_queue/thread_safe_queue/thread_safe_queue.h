//
//  thread_safe_queue.h
//  thread_safe_queue
//
//  Created by Дмитрий Ткаченко on 03/11/2016.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//

#ifndef thread_safe_queue_h
#define thread_safe_queue_h

#include <deque>
#include <thread>
#include <condition_variable>
#include <mutex>

template <class Value, class Container = std::deque<Value>>
class thread_safe_queue {
public:
    
    explicit thread_safe_queue(size_t capacity) {
        size = capacity;
        isShutdowned = false;
    }
    
    void enqueue(Value v) {
        std::unique_lock<std::mutex> lock(mtx);
        
        if (isShutdowned) {
            throw std::runtime_error("You are trying enqueue to shutdowned queue!");
        }
        
        while (isShutdowned || container.size() == size)
            cvPop.wait(lock);
        
        container.push_back(std::move(v));
        cvPush.notify_one();
    }
    
    void pop(Value& v) {
        
        std::unique_lock<std::mutex> lock(mtx);
        
        if (isShutdowned) {
            throw std::runtime_error("You are trying pop to empty queue!");
        }
        
        while (container.size() == 0)
            cvPush.wait(lock);
        
        v = std::move(container.front());
        container.pop_front();
        cvPop.notify_one();
    }
    
    void shutdown() {
        isShutdowned = true;
        cvPop.notify_all();
        cvPush.notify_all();
    }
    
private:
    size_t size;
    Container container;
    std::mutex mtx;
    std::condition_variable cvPush, cvPop;
    bool isShutdowned;
};

#endif /* thread_safe_queue_h */
