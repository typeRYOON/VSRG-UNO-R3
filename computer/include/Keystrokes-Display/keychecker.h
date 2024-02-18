#ifndef KEYCHECKER_H
#define KEYCHECKER_H
#include "osuenv.h"
#include <condition_variable>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <unordered_map>
#include <algorithm>
typedef std::chrono::steady_clock::time_point TimePoint;


class KeyChecker {
private:
    OsuEnv& env;
    void    trackJKey();
    void    trackActiveKeys(int);

public:
    KeyChecker(OsuEnv& ref) : env(ref) {}
    void trackKeys(int);
};
#endif