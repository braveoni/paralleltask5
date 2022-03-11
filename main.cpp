#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;

#define n 2

class Semaphore {
public:
    explicit Semaphore(int count) : count_(count) {}

    void acquire_semaphore() {
        unique_lock<mutex> lock(mut_);
        while (count_ == 0) {
            cout << "Wait" << endl;
            cv_.wait(lock);
        }

        cout << "Acquire" << endl;
        --count_;
    }

    void release_semaphore() {
        lock_guard<mutex> lock(mut_);
        cout << "Release" << endl;
        ++count_;
        cv_.notify_one();
    }

private:
    int count_;
    mutex mut_;
    condition_variable cv_;
};

Semaphore semaphore(n);

void f_thread() {
    for (;;) {
        semaphore.acquire_semaphore();
        this_thread::sleep_for(chrono::milliseconds(2000));
        semaphore.release_semaphore();
    }
}

void test() {
    thread t1(f_thread);
    thread t2(f_thread);
    thread t3(f_thread);
    thread t4(f_thread);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

int main() {
    cout << "Hardware Concurrency: " << thread::hardware_concurrency() << endl;
    test();
}
