//
//  main.cpp
//  MrRobot
//
//  Created by Дмитрий Ткаченко on 30/09/2016.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//


#include <mutex>
#include <iostream>
#include <thread>
#include <vector>

int steps_count = 10;

class semaphore{
public:
    
    semaphore() {}
    semaphore (int x){}
    void wait() {
        return;
    }
    
    void post() {
        return;
    }
};

void stepLeft(semaphore &sem1, semaphore &sem2) {
    for (int i = 0; i < steps_count; i++) {
        sem2.wait();
        //Some function that required in left step.
        sem1.post();
    }
}

void stepRight(semaphore &sem1, semaphore &sem2) {
    for (int i = 0; i < steps_count; i++) {
        sem1.wait();
        //Some function that required in right step.
        sem2.post();
    }
}

void stepRight(std::atomic<bool> &dir, std::mutex &mutex, std::condition_variable &cv) {
    std::unique_lock<std::mutex> lock(mutex);
    for (int i = 0; i < steps_count; i++) {
        cv.wait(lock, [&] {return (dir.load()); }); // waiting for our direction
        //Some function that required in left step.
        dir.store(false); //change the direction
        cv.notify_one(); //signal to next thread
    }
}

int main(int argc, const char * argv[]) {
    semaphore sem1, sem2(1);
    std::thread th1(stepLeft, std::ref(sem1), std::ref(sem2));
    std::thread th2(stepRight, std::ref(sem1), std::ref(sem2));
    th1.join(), th2.join();
    return 0;
}
