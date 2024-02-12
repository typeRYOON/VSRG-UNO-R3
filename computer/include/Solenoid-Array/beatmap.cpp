#include "beatmap.h"

void listenForKeyH(std::condition_variable& cv)
{
    while (!(GetAsyncKeyState('H') & 0x8000)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    cv.notify_all();
}

void listenForTermination()
{
    while (true) {
        if (GetAsyncKeyState('J') & 0x8000) {
            std::cout << "\n Premature termination...\n";
            exit(0);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void releaseAll(std::vector<int>& virtualKeys, bool exitFlag)
{
    for (auto& key : virtualKeys) {
        keybd_event(static_cast<BYTE>(key), 0, KEYEVENTF_KEYUP, 0);
    }
    if (exitFlag) {
        std::cout << "\n Program has fully terminated." << std::endl;
        exit(0);
    }
}

void BeatMap::play()
{
    short endedThread = 0;
    std::condition_variable cv;
    std::thread threads[keyCount];

    std::cout << "\n Beatmap   : " << title;
    std::cout << "\n Artist    : " << artist;
    std::cout << "\n Difficulty: " << version;
    std::cout << "\n Key Count : " << keyCount;
    std::cout << "\n\n Hold H to start / J to terminate.\n";

    releaseAll(virtualKeys, false);

    for (int i = 0; i < keyCount; ++i) {
        threads[i] = std::thread([this, i, &cv, &endedThread]()
        {
            std::mutex mtx;
            int virtualKey = virtualKeys[i];
            std::unique_lock<std::mutex> lock(mtx);
            unsigned int startDelay = hitChart[hitChart.size() - 1][i].first;
            cv.wait(lock);

            auto startTime = std::chrono::high_resolution_clock::now();
            auto expected = startTime + std::chrono::milliseconds(startDelay);
            if (startDelay) std::this_thread::sleep_until(expected);

            startTime = expected;
            for (const auto& pair : hitChart[i]) {
                // std::cout << i << '\n';
                keybd_event(static_cast<BYTE>(virtualKey), 0, 0, 0);
                std::this_thread::sleep_for(std::chrono::milliseconds(pair.first));
                keybd_event(static_cast<BYTE>(virtualKey), 0, KEYEVENTF_KEYUP, 0);

                expected = startTime + std::chrono::milliseconds(pair.second + pair.first);
                std::this_thread::sleep_until(expected);
                startTime = expected;
            }
            if (endedThread++ == (keyCount - 1)) releaseAll(virtualKeys, true);
        });
    }
    std::thread startThread(listenForKeyH, std::ref(cv));
    std::thread terminationThread(listenForTermination);

    for (int i = 0; i < keyCount; ++i) {
        threads[i].join();
    }
    startThread.join();
    terminationThread.join();
}

BeatMap::BeatMap(std::vector<std::string>& metadata, int keys, HC hitChart)
{
    title = metadata[0];
    artist = metadata[2];
    creator = metadata[3];
    version = metadata[1];
    keyCount = keys;
    this->hitChart = hitChart;
}