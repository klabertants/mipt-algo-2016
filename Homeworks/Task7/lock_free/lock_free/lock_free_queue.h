//
//  lock_free_queue.h
//  lock_free
//
//  Created by Дмитрий Ткаченко on 08/12/2016.
//  Copyright © 2016 BillionariesClub. All rights reserved.
//

#ifndef lock_free_queue_h
#define lock_free_queue_h

#include <memory>
#include <atomic>

template<typename T>
class lock_free_queue
{
private:
    
    struct node;
    
    struct counted_node_ptr
    {
        int external_count;
        node* ptr;
    };
    
    std::atomic<counted_node_ptr> head;
    std::atomic<counted_node_ptr> tail;
    
    struct node_counter
    {
        unsigned internal_count:30;
        unsigned external_counters:2;
    };
    
    struct node
    {
        std::atomic<T*> data;
        std::atomic<node_counter> count;
        counted_node_ptr next;
        node()
        {
            node_counter new_count;
            new_count.internal_count=0;
            new_count.external_counters=2;
            count.store(new_count);
            next.ptr=nullptr;
            next.external_count=0;
        }
        
        void release_ref()
        {
            node_counter old_counter=
            count.load(std::memory_order_relaxed);
            node_counter new_counter;
            do
            {
                new_counter=old_counter;
                --new_counter.internal_count;
            }
            while(!count.compare_exchange_strong(
                                                 old_counter,new_counter,
                                                 std::memory_order_acquire,std::memory_order_relaxed));
            if(!new_counter.internal_count &&
               !new_counter.external_counters)
            {
                delete this;
            }
        }
    };
    
    static void increase_external_count(
                                        std::atomic<counted_node_ptr>& counter,
                                        counted_node_ptr& old_counter)
    {
        counted_node_ptr new_counter;
        do
        {
            new_counter=old_counter;
            ++new_counter.external_count;
        }
        while(!counter.compare_exchange_strong(
                                               old_counter,new_counter,
                                               std::memory_order_acquire,std::memory_order_relaxed));
        old_counter.external_count=new_counter.external_count;
    }
    
    static void free_external_counter(counted_node_ptr &old_node_ptr)
    {
        node* const ptr=old_node_ptr.ptr;
        int const count_increase=old_node_ptr.external_count-2;
        node_counter old_counter=
        ptr->count.load(std::memory_order_relaxed);
        node_counter new_counter;
        do
        {
            new_counter=old_counter;
            --new_counter.external_counters;
            new_counter.internal_count+=count_increase;
        }
        while(!ptr->count.compare_exchange_strong(
                                                  old_counter,new_counter,
                                                  std::memory_order_acquire,std::memory_order_relaxed));
        if(!new_counter.internal_count &&
           !new_counter.external_counters)
        {
            delete ptr;
        }
    }
public:
    
    lock_free_queue() {
        node* Head = new node;
        
        counted_node_ptr ptrH;
        ptrH.external_count = 0;
        ptrH.ptr = Head;
        
        head.store(ptrH);
        tail.store(head.load());
    }
    
    void enqueue(T new_value)
    {
        std::unique_ptr<T> new_data(new T(new_value));
        counted_node_ptr new_next;
        new_next.ptr=new node;
        new_next.external_count=1;
        counted_node_ptr old_tail=tail.load();
        for(;;)
        {
            increase_external_count(tail,old_tail);
            T* old_data=nullptr;
            if(old_tail.ptr->data.compare_exchange_strong(
                                                          old_data,new_data.get()))
            {
                old_tail.ptr->next=new_next;
                old_tail=tail.exchange(new_next);
                free_external_counter(old_tail);
                new_data.release();
                break;
            }
            old_tail.ptr->release_ref();
        }
    }
    
    bool dequeue(T& item)
    {
        counted_node_ptr old_head=head.load(std::memory_order_relaxed);
        for(;;)
        {
            increase_external_count(head,old_head);
            node* const ptr=old_head.ptr;
            if(ptr==tail.load().ptr)
            {
                ptr->release_ref();
                return false;
            }
            if(head.compare_exchange_strong(old_head,ptr->next))
            {
                T* const res=ptr->data.exchange(nullptr);
                free_external_counter(old_head);
                item = *res;
                return true;
            }
            ptr->release_ref();
        }
    }
};


#endif /* lock_free_queue_h */
