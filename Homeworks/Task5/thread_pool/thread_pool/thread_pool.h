//
//  parallel_sort.h
//  parallel_sort
//
//  Created by Dmitry Tkachenko on 04/11/2016.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//	

#ifndef parallel_sort_h
#define parallel_sort_h

#include <iostream>
 
class parallel_sort {

private:
    size_t workers;

public:
    
    parallel_sort(size_t workers_num) : workers(workers_num) {}

    template<typename RandomAccessIterator, typename Compare>
    void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        workers /= 2;
        std::sort(first, last, comp);
    }

};


#endif /* parallel_sort_h */
