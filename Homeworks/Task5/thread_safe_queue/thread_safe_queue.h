#include <iostream>
#include <deque>
#include <mutex>
#include <atomic>
#include <condition_variable>

template<class Value, class Container = std::deque<Value>>
class thread_safe_queue {
public:
	explicit thread_safe_queue(size_t capacity) : capacity_(capacity), is_shutdown_(false) {}

	void enqueue(Value v) {
		std::unique_lock<std::mutex> lock(mutex_);
		while (is_shutdown_ || container_.size() == capacity_) {
			if (is_shutdown_) {
				throw std::logic_error("Call enqueue after shutdown!");
			}
			cv_something_pop_.wait(lock);
		}
		container_.push_back(std::move(v));
		cv_something_push_.notify_one();
	}

	void pop(Value &v) {
		std::unique_lock<std::mutex> lock(mutex_);
		while (container_.size() == 0) {
			if (is_shutdown_) {
				throw std::logic_error("Call pop on empty queue!");
			}
			cv_something_push_.wait(lock);
		}
		v = std::move(container_.front());
		container_.pop_front();
		cv_something_pop_.notify_one();
	}

	void shutdown() {
		is_shutdown_ = true;
		cv_something_push_.notify_all();
		cv_something_pop_.notify_all();
	}

private:
	size_t capacity_;
	Container container_;
	std::atomic_bool is_shutdown_;
	std::mutex mutex_;
	std::condition_variable cv_something_push_;
	std::condition_variable cv_something_pop_;
};