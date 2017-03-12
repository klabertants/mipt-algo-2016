#pragma once

#include <cstdio>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <future>
#include <queue>
#include <algorithm>
#include <chrono>
using namespace std;


template<class Value>
class thread_pool {
    bool isShutdowned = false;
    mutex queue_mutex;
    vector<thread> workers;
    queue<packaged_task<Value()> > task_queue;
    condition_variable task_queue_not_empty;
    
    void TryExecuteTask() {
        packaged_task<Value()> task;
        {
            unique_lock<mutex> lock(queue_mutex);
            if (task_queue.empty())
                return;
            task = move(task_queue.front());
            task_queue.pop();
        }
        task();
    }
    
    void workerFunction() {
        for (;;) {
            packaged_task<Value()> task;
            {
                unique_lock<mutex> lock(queue_mutex);
                task_queue_not_empty.wait(lock, [&]() { return !task_queue.empty() || isShutdowned; });
                if (task_queue.empty() && isShutdowned)
                    return;
                task = move(task_queue.front());
                task_queue.pop();
            }
            task();
        }
    }
    
public:
    explicit thread_pool(size_t num_workers) {
        for (size_t i = 0; i < num_workers; i++) {
            workers.emplace_back([&]() { workerFunction(); });
        }
    }
    
    future<Value> submit(function<Value()> task) {
        unique_lock<mutex> lock(queue_mutex);
        packaged_task<Value()> pckg_task(task);
        future<Value> result = pckg_task.get_future();
        
        if (isShutdowned) {
            throw exception();
        }
        task_queue.push(std::move(pckg_task));
        task_queue_not_empty.notify_one();
        return result;
    }
    
    void wait(future<Value> &&future) {
        while (future.wait_for(chrono::milliseconds(0)) != future_status::ready) {
            TryExecuteTask();
        }
    }
    
    void shutdown() {
        {
            unique_lock<mutex> lock(queue_mutex);
            if (isShutdowned)
                return;
            isShutdowned = true;
        }
        task_queue_not_empty.notify_all();
        for (auto &worker : workers)
            worker.join();
    }
};

template<typename RandomAccessIterator, typename Compare>
void buble_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    for (; first != last; first++){
        for (auto second = first + 1; second != last; second++) {
            if (comp(*second, *first))
                swap(*second, *first);
        }
    }
}

//#pragma comment(linker, "\STACK:64000000")

class parallel_sort {
    
    typedef thread_pool<void> sort_pool;
    
    sort_pool *pool;
    
    
    template<typename RandomAccessIterator, typename Compare>
    void subsort_task(RandomAccessIterator first, RandomAccessIterator last, Compare comp, sort_pool *pool) {
        size_t length = last - first;
        if (length < 42) { // amazing number
            buble_sort(first, last, comp);
            //std::sort(first, last, comp);
            return;
        }
        
        auto middle = first + (length / 2);
        auto left_part = [&]() -> void {
            subsort_task(first, middle, comp, pool);
        };
        auto right_part = [&]() -> void {
            subsort_task(middle, last, comp, pool);
        };
        auto future_left = pool->submit(left_part);
        
        pool->wait(std::move(future_left));
        right_part();
        
        inplace_merge(first, middle, last, comp);
    }
    
public:
    parallel_sort(size_t workers) {
        pool = new thread_pool<void>(workers);
    }
    
    template<typename RandomAccessIterator, typename Compare>
    void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        auto fut = pool->submit([&]() { subsort_task(first, last, comp, pool); });
        fut.get();
        pool->shutdown();
    }
};
