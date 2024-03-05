#include "arduinohandler.hpp"
#include "omaniaparser.hpp"
#include "beatmap.hpp"
#include "serial.hpp"
#include <condition_variable>
#include <unordered_map>
#include <algorithm>
#include <Windows.h>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
typedef std::chrono::steady_clock::time_point TimePoint;


std::atomic<bool> isRunning(false);
std::atomic<bool> terminate(false);
std::condition_variable cv;
std::mutex mtx;


void removeComma(std::string& src)
{
    src.erase(
        std::remove_if(
            src.begin(),
            src.end(),
            [](char c) { return c == '"'; }),
        src.end()
    );
}


void startKey()
{
    while (!(GetAsyncKeyState('H') & 0x8000)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    isRunning = true;
    cv.notify_all();
}


void terminateKey()
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&]{ return isRunning.load(); });
    mtx.unlock();

    auto startTime = std::chrono::steady_clock::now();
    while (!terminate) {
        auto endTime = startTime + std::chrono::milliseconds(20);
        std::this_thread::sleep_until(endTime);
        startTime = endTime;

        if (GetAsyncKeyState('J') & 0x8000) {
            std::cout << " :: Premature Termination!" << std::endl;
            terminate = true;
            cv.notify_all();
        }
    }
}

// - - KEYS PER SECOND AND KEY STROKES FUNCTIONS - -                                   - -

short incrementKPS(
    std::unordered_map<int, std::vector<TimePoint>>& timeHits,
    std::unordered_map<int, bool>& lastPress,
    const int& key)
{
    if (lastPress[key]) { return 0; }
    timeHits[key].push_back(std::chrono::steady_clock::now());
    return 1;
}


short decrementKPS(std::unordered_map<int, std::vector<TimePoint>>& timeHits)
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


void enableANSI()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode))
        {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
}


void ArduinoHandler::trackActiveKeys(int keyCount)
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&]{ return isRunning.load(); });
    mtx.unlock();


    std::vector<int> virtualKeys = env.getVirtualKeys(keyCount);
    auto startTime = std::chrono::steady_clock::now();
    auto endTime = startTime;
    char row[keyCount + 1];
    std::ostringstream oss;
    std::string strKPS;
    row[keyCount] = '\0';
    short j;


    //char comPort[] = "\\\\.\\COM3";
    //DWORD COM_BAUD_RATE = CBR_9600;
    //SimpleSerial ArduinoOne(comPort, COM_BAUD_RATE);

    /**if (!ArduinoOne.IsConnected()) {
        std::cout <<
            "Error :: Arduino Serial Connection (" << comPort << ')' << '\n';
        terminate = true;
        return;
    }*/

    int KPS = 1;
    enableANSI();
    std::unordered_map<int, bool> lastPress;
    std::unordered_map<int, std::vector<TimePoint>> timeHits;
    for (const int& key : virtualKeys) { lastPress[key] = false; }
    std::cout << std::endl;

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
                    row[j] = '\xDB';
                } else {
                    lastPress[key] = false;
                }
                j++;
            }
            startTime = endTime;
        }
        //printBytes((const char*)&KPS, sizeof((const char*)&KPS), KPS);
        // ArduinoOne.WriteSerialPort((const char*)&KPS);
        KPS -= decrementKPS(timeHits);
        std::cout << row << ' ' << KPS-1 << '\n';
        std::cout << "\033[s\033[2A\033[8C  \033[u";
    }

    // bool closed  = ArduinoOne.CloseSerialPort();
    // if (closed) { std::cout << "CLOSED PORT" << std::endl; }
}


void releaseAll(std::vector<int>& virtualKeys)
{
    for (auto& key : virtualKeys) {
        keybd_event(static_cast<BYTE>(key), 0, KEYEVENTF_KEYUP, 0);
    }
}


void ArduinoHandler::runKey
(int i, std::vector<int>& virtualKeys, HC& hitChart, int& ended, int& keyCount)
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&]{ return isRunning.load(); });
    mtx.unlock();

    int virtualKey = virtualKeys[i];
    unsigned startDelay = hitChart[hitChart.size() - 1][i].first;
    auto startTime = std::chrono::high_resolution_clock::now();
    auto expected = startTime + std::chrono::milliseconds(startDelay);
    if (startDelay) { std::this_thread::sleep_until(expected); }

    startTime = expected;
    for (const auto& pair : hitChart[i]) {
        if (terminate) { break; }
        keybd_event(static_cast<BYTE>(virtualKey), 0, 0, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(pair.first));
        keybd_event(static_cast<BYTE>(virtualKey), 0, KEYEVENTF_KEYUP, 0);

        expected = startTime + std::chrono::milliseconds(pair.second + pair.first);
        std::this_thread::sleep_until(expected);
        startTime = expected;
    }
    if (++ended == keyCount) { terminate = true; }
}


void ArduinoHandler::programLoop()
{
    bool AU, SA, KS, KPS, programLoop = true;
    std::string line;

    std::system("cls");
    std::cout <<
        "\n ::     :: Which modules should be used?                 ::"
        "\n :: AU  :: Automatic Player (Solenoid-Array must be off) ::"
        "\n :: SA  :: Solenoid Array (Automatic Player must be off) ::"
        "\n :: KS  :: Key-Strokes Display                           ::"
        "\n :: KPS :: Keys per Second Display                       ::"
        "\n :: OLD :: Settings from previous session                ::"
        "\n\n  >> ";
    std::getline(std::cin, line);
    env.setModulesInUse(line);
    try   { env.getModulesInUse(AU, SA, KS, KPS); }
    catch ( const std::invalid_argument& e ) {
        std::cerr << " :: Error :: " << e.what() << std::endl;
        return;
    }

    std::system("cls");
    std::cout << "\n :: Current Settings ::";
    std::cout << "\n :: 4K :: " << env.full4K << "\n :: 7K :: " << env.full7K;
    std::cout << "\n :: ON :: " << env.onModules;
    std::cout << "\n :: Enter the key count to track\n\n >> ";
    std::getline(std::cin, line);
    int keyCount;
    try   {
        keyCount = stoi(line);
        if (keyCount != 4 && keyCount != 7) {
            throw std::invalid_argument("");
        }
    }
    catch ( const std::invalid_argument& e )
    {
        std::cerr << " :: Error :: Invalid Key Count. ";
        return;
    }

    std::vector<std::thread> threads;
    OManiaParser* parser = new OManiaParser(env);
    std::vector<int> vk;
    HC hitChart;
    int endedThreads;
    BeatMap* bp = nullptr;
    while (programLoop)
    {
        terminate.store(false);
        isRunning.store(false);
        threads.clear();
        endedThreads = 0;

        if (AU)
        {
            std::system("cls");
            std::cout << "\n :: Automatic Player            ::";
            std::cout << "\n :: Enter the beatmap directory ::\n\n >> ";
            std::getline(std::cin, line);
            removeComma(line);
            if (line == "e") { return; }

            try   { bp = parser->from_osu(line); }
            catch ( const std::invalid_argument& e ) {
                std::cerr << " :: Error :: " << e.what() << '\n';
                std::getline(std::cin, line);
                continue;
            }
            hitChart = bp->getHC();
            vk = bp->virtualKeys;
            releaseAll(vk);

            std::thread keyThreads[keyCount];
            for (int i = 0; i < keyCount; ++i) {
                threads.emplace_back(std::thread
                    ([i, this, &vk, &hitChart, &endedThreads, &keyCount]()
                    { this->runKey(i, vk, hitChart, endedThreads, keyCount); }));
            }
        }

        if (KS || KPS)
        {
            threads.emplace_back(std::thread(
                [this, keyCount, KS, KPS](){ this->trackActiveKeys(keyCount); }
            ));
        }

        std::thread startThread(startKey);
        std::thread terminationThread(terminateKey);

        std::cout << "Press H to begin / J to end." << std::endl;

        startThread.join();
        terminationThread.join();

        for (auto& thread : threads) {
            thread.join();
        }

        std::cout << "Exit? :: ";
        std::getline(std::cin, line);
        if (env.toLowercase(line) == "e") { programLoop = false; }
    }
    try   { env.writeConfigs(); }
    catch ( const std::invalid_argument& e )
    {
        std::cerr << " :: Error :: " << e.what() << std::endl;
    }
}