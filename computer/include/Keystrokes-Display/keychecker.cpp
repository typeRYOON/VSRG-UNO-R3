#include "keychecker.h"
#include "SimpleSerial.h"


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


short incrementKPS(
    std::unordered_map<int, std::vector<TimePoint>>& timeHits,
    std::unordered_map<int, bool>& lastPress,
    const int& key)
{
    if (lastPress[key]) { return 0; }
    timeHits[key].push_back(std::chrono::steady_clock::now());
    // std::cout << key << ": " << timeHits[key].size() << '\n';
    return 1;
}


short decrementKPS(
    std::unordered_map<int, std::vector<TimePoint>>& timeHits)
{
    short initial;
    short countDelta = 0;
    auto now = std::chrono::steady_clock::now();

    for (auto& [key, hitVector] : timeHits)
    {
        initial = hitVector.size();
        hitVector.erase(
            std::remove_if(hitVector.begin(), hitVector.end(), 
                [now](const TimePoint& timePoint) {
                    return std::chrono::duration_cast<std::chrono::milliseconds>
                    (now - timePoint).count() >= 1000;
                }),
            hitVector.end());
        countDelta += initial - hitVector.size();
    }
    return countDelta;
}

void printBytes(const char* data, size_t size, int KPS) {
    std::cout << "KPS: " << KPS << '\n';
    for (size_t i = 0; i < size; ++i) {
        std::cout << "Byte " << i << ": " << static_cast<int>(data[i]) << '\n';
    }
}


void KeyChecker::trackActiveKeys(int keyCount) {
    std::vector<int> virtualKeys = env.getVirtualKeys(keyCount);
    auto startTime = std::chrono::steady_clock::now();
    auto endTime = startTime;
    char row[keyCount + 1];
    std::ostringstream oss;
    std::string strKPS;
    row[keyCount] = '\0';
    short j;


    char comPort[] = "\\\\.\\COM3";
    DWORD COM_BAUD_RATE = CBR_9600;
    SimpleSerial ArduinoOne(comPort, COM_BAUD_RATE);

    if (!ArduinoOne.IsConnected()) {
        std::cout <<
            "Error :: Arduino Serial Connection (" << comPort << ')' << '\n';
        terminate = true;
    } 

    int KPS = 1;
    std::unordered_map<int, bool> lastPress;
    std::unordered_map<int, std::vector<TimePoint>> timeHits;
    for (const int& key : virtualKeys) { lastPress[key] = false; }

    while (!terminate) {
        memset(row, ' ', keyCount);
        for (int i = 0; i < 2; ++i)
        {
            endTime = startTime + std::chrono::milliseconds(10);
            std::this_thread::sleep_until(endTime);
            j = 0;
            for (const int& key : virtualKeys) {
                if (GetAsyncKeyState(key) & 0x8000) {
                    KPS += incrementKPS(timeHits, lastPress, key);
                    lastPress[key] = true;
                    row[j] = '+';
                } else {
                    lastPress[key] = false;
                    row[j] = ' ';
                }
                j++;
            }
            startTime = endTime;
        }
        //printBytes((const char*)&KPS, sizeof((const char*)&KPS), KPS);
        ArduinoOne.WriteSerialPort((const char*)&KPS);
        KPS -= decrementKPS(timeHits);
        std::cout << row << ' ' << KPS-1 << '\n';
    }

    bool closed  = ArduinoOne.CloseSerialPort();
    if (closed) { std::cout << "CLOSED PORT" << std::endl; }
}
