//
//  main.cpp
//  lock_free_queue
//
//  Created by Дмитрий Ткаченко on 29/11/2016.
//  Copyright © 2016 BillionariesClub. All rights reserved.
//

#include <iostream>
#include <thread>
#include "lock_free_queue.h"

lock_free_queue<int> Q;

int main() {
    std::atomic<int> q;
    atomic_compare_exchange_weak(<#atomic<_Tp> *__o#>, <#_Tp *__e#>, <#_Tp __d#>)
    Q.enqueue(3);
    int x;
    Q.enqueue(4);
    Q.dequeue(x);
    Q.dequeue(x);
    std:: cout << x << std::endl;
    return 0;
}
