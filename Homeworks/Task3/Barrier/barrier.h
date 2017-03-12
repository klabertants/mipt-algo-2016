#include <mutex>
#include <atomic>
#include <condition_variable>

class barrier {
private:
    size_t _num_threads;
    std::atomic<size_t> waiting;
    std::mutex mutex;
    std::condition_variable cv;
public:
    explicit barrier(size_t num_threads) {
    	_num_threads = num_threads;
    	waiting.store(0);
    }

    void enter() {
    	std::unique_lock<std::mutex> lock(mutex);
    	if (waiting.load() >= _num_threads - 1) {
    		waiting.store(0);
    		cv.notify_all();
    	} 
    	else {
    		waiting.store(waiting.load() + 1);
    		cv.wait(lock);
    	}
    }
};

