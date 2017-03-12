//
//  parallel_sort.h
//  parallel_sort
//
//  Created by Дмитрий Ткаченко on 06/11/2016.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//  Right solution for now...

#ifndef parallel_sort_h
#define parallel_sort_h

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>

template <class Value>
class thread_pool {
public:
    
    explicit thread_pool(size_t num_workers) {
        
        auto worker = [this]() {
            while (true) {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this]() { return isShutdowned || promises.size();});
                if (promises.empty()) {
                    break;
                }
                std::promise<Value> promise = std::move(promises.front());
                auto func = functions.front();
                promises.pop();
                functions.pop();
                lock.unlock();
                try{
                    promise.set_value(func());
                }
                catch (...) {
                    promise.set_exception(std::current_exception());
                }
            }
        };
        
        isShutdowned = false;
        
        for (size_t i = 0; i < num_workers; i++) {
            workers.emplace_back(worker);
        }
    }
    
    std::future<Value> submit(std::function<Value()> func) {
        
        std::unique_lock<std::mutex> lock(mtx);
        
        if (isShutdowned) {
            throw std::runtime_error("Pool is shutdowned!");
        }
        
        std::promise<Value> promise;
        auto futureValue = promise.get_future();
        promises.emplace(std::move(promise));
        functions.emplace(func);
        cv.notify_one();
        return futureValue;
    }
    
    void shutdown() {
        std::unique_lock<std::mutex> lock(mtx);
        isShutdowned = true;
        lock.unlock();
        cv.notify_all();
        for (auto &w : workers) {
            w.join();
        }
    }
    
    void wait(std::future<Value> &&future) {
        while (future.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
            tryTask();
        }
    }
    
private:
    std::mutex mtx;
    std::condition_variable cv;
    std::vector<std::thread> workers;
    bool isShutdowned;
    std::queue<std::promise<Value>> promises;
    std::queue<std::function<Value()>> functions;
    
    void tryTask() {
        std::packaged_task<Value()> func;
        {
            std::unique_lock<std::mutex> lock(mtx);
            if (functions.empty()) return;
            func = std::move(functions.front());
            functions.pop();
        }
        func();
    }
};

class parallel_sort {
public:
    
    parallel_sort(size_t workers) {
        pool = new thread_pool<void>(workers);
    }
    
    template<typename RandomAccessIterator, typename Compare>
    void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        auto startCalculate = pool->submit([&]() { subTask(first, last, comp, pool); });
        startCalculate.get();
        pool->shutdown();
    }
    
private:
    
    template<typename RandomAccessIterator, typename Compare>
    void subTask(RandomAccessIterator first, RandomAccessIterator last, Compare comp, thread_pool<void> *pool) {
        size_t length = last - first;
        
        if (length < 3000) {
            std::sort(first, last, comp);
            return;
        }
        
        auto mid = first + (length / 2);
        auto leftPart = [&]() -> void { subTask(first, mid, comp, pool); };
        auto rightPart = [&]() -> void { subTask(mid, last, comp, pool); };
        
        auto futureValueLeft = pool->submit(leftPart);
        
        pool->wait(std::move(futureValueLeft));
        rightPart();
        
        inplace_merge(first, mid, last, comp);
    }
    
    thread_pool<void> *pool;
};
#endif /* parallel_sort_h */
