#ifndef KEYCHECKER_H
#define KEYCHECKER_H
#include "osuenv.h"
#include <condition_variable>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>


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