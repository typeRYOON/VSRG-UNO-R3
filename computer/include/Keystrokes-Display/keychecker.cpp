#include "keychecker.h"

std::atomic<bool> terminate(false);
std::condition_variable cv;
std::mutex mtx;

void KeyChecker::trackKeys(int keyCount) {
    std::thread activeKeysThread(&KeyChecker::trackActiveKeys, this, keyCount);
    std::thread jKeyThread(&KeyChecker::trackJKey, this);

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return terminate.load(); });
    }

    activeKeysThread.join();
    jKeyThread.join();
}

void KeyChecker::trackJKey() {
    auto startTime = std::chrono::steady_clock::now();
    while (true) {
        auto endTime = startTime + std::chrono::milliseconds(20);
        std::this_thread::sleep_until(endTime);
        startTime = endTime;

        if (GetAsyncKeyState('J') & 0x8000) {
            std::cout << " :: Terminating the key tracker!" << std::endl;
            {
                std::lock_guard<std::mutex> lock(mtx);
                terminate = true;
            }
            cv.notify_one();
            break;
        }
    }
}

void KeyChecker::trackActiveKeys(int keyCount) {
    std::vector<int> virtualKeys = env.getVirtualKeys(keyCount);
    auto startTime = std::chrono::steady_clock::now();
    std::stringstream ss;

    while (!terminate) {
        auto endTime = startTime + std::chrono::milliseconds(20);
        std::this_thread::sleep_until(endTime);
        startTime = endTime;

        for (const int& key : virtualKeys) {
            if (GetAsyncKeyState(key) & 0x8000) {
                ss << '+';
            } else {
                ss << ' ';
            }
        }
        std::cout << ss.str() << '\n';
        ss.str("");
    }
}
