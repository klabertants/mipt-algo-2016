#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>

template <typename T>
class thread_pool {
 public:
  explicit thread_pool(size_t worker_count);

  std::future<T> submit(std::function<T()> task);

  void shutdown();

 private:
  bool shutdown_;
  std::mutex mutex_;
  std::condition_variable task_cond_;
  std::queue<std::pair<std::promise<T>, std::function<T()>>> tasks_;
  std::vector<std::thread> workers_;
};

template <typename T>
thread_pool<T>::thread_pool(size_t worker_count) : shutdown_(false) {
  auto worker = [this]() {
    for (;;) {
      std::unique_lock<std::mutex> lock(mutex_);
      task_cond_.wait(lock, [this]() { return shutdown_ || !tasks_.empty(); });

      if (tasks_.empty()) {
        break;
      }

      std::promise<T> promise = std::move(tasks_.front().first);
      auto task = tasks_.front().second;
      tasks_.pop();
      lock.unlock();

      try {
        promise.set_value(task());
      } catch (...) {
        promise.set_exception(std::current_exception());
      }
    }
  };

  for (size_t i = 0; i < worker_count; ++i) {
    workers_.emplace_back(worker);
  }
}

template <typename T>
std::future<T> thread_pool<T>::submit(std::function<T()> task) {
  std::unique_lock<std::mutex> lock(mutex_);
  if (shutdown_) {
    throw std::runtime_error("Submitted task to shut down pool");
  }

  std::promise<T> promise;
  auto future = promise.get_future();

  tasks_.emplace(std::move(promise), task);
  task_cond_.notify_one();

  return future;
}

template <typename T>
void thread_pool<T>::shutdown() {
  std::unique_lock<std::mutex> lock(mutex_);
  shutdown_ = true;
  lock.unlock();
  task_cond_.notify_all();

  for (auto &worker : workers_) {
    worker.join();
  }
}

#endif // THREAD_POOL_H_
