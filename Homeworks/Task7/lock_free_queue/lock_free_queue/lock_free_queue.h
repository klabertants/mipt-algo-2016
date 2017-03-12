//
//  lock_free_queue.h
//  lock_free_queue
//
//  Created by Дмитрий Ткаченко on 03/12/2016.
//  Copyright © 2016 BillionariesClub. All rights reserved.
//

#ifndef lock_free_queue_h
#define lock_free_queue_h

#include <atomic>

template<typename T>
class lock_free_queue {
private:
    
    struct Node
    {
        Node(const T& data_)
        : data(std::make_shared<T>(data_))
        {}
        
        T data;
        Node* next;
        
        Node() {
            next = nullptr;
        }
    };
    
    std::atomic<Node*> Head, Tail;
    
public:
    
    lock_free_queue() {
        Node* newNode = new Node;
        Head.store(newNode);
        Tail.store(newNode);
    }
    
    void enqueue(T item) {
        
        Node* tempNode = new Node;
        tempNode -> data = item;
        std::atomic<Node*> newNode;
        newNode.store(tempNode);
        std::atomic<Node*> currentTail;
        currentTail.store(Tail.load());
        std::atomic<Node*> currentTailNext;
        currentTailNext.store(Tail.load() -> next);
        
        while (true) {
            currentTail.store(Tail.load());
            currentTailNext.store(Tail.load() -> next);
            
            if (!currentTailNext.load()) {
                if (atomic_compare_exchange_strong(Tail, currentTailNext, *newNode)) {
                    break;
                } else {
                    Tail.compare_exchange_weak(currentTail.load(), currentTailNext.load());
                }
            }
        }
        
        atomic_compare_exchange_weak(currentTail.load(), currentTailNext.load());
    }
    
    bool dequeue(T& item) {
        std::atomic<Node*> currentHead;
        currentHead.store(Head.load());
        std::atomic<Node*> currentTail;
        currentTail.store(Tail.load());
        std::atomic<Node*> currentHeadNext;
        currentHeadNext.store(currentHead.load() -> next);
        if (currentHead.load() == currentTail.load()) {
            if (!currentHeadNext.load()) {
                return false;
            } else {
                Tail.compare_exchange_strong(currentHead.load(), currentHeadNext.load(), std::memory_order_relaxed, std::memory_order_acquire);
            }
        } else {
            if (Head.compare_exchange_weak(currentHead.load(), currentHeadNext.load())) {
                item = currentHeadNext.load() -> item_;
                return true;
            }
        }
    }
};

#endif /* lock_free_queue_h */
