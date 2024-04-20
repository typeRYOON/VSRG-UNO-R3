#include "computerks.h"
#include "serial.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <chrono>
#include <thread>

typedef std::chrono::steady_clock::time_point TimePoint;

ComputerKS::ComputerKS(
    QVector<uint8_t>& vk, QString& port, bool serialRun, QObject* parent) : QObject{parent}
{
    virtualKeys = vk;
    this->port = port;
    this->serialRun = serialRun;
}

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
    short initial, countDelta = 0;
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

void ComputerKS::run()
{
    SimpleSerial serial(port.toStdString(), CBR_9600);
    if (!serial.IsConnected() && serialRun) {
        emit errorMsg(" : : Bad Arduino Port.");
        return;
    }

    std::unordered_map<int, bool> lastPress;
    std::map<int, int> offset;

    for (int i = 0; i < virtualKeys.size(); ++i)
    {
        offset[virtualKeys[i]] = i+1;
        lastPress[virtualKeys[i]] = false;
    }

    uint16_t KPS = 0, val;
    std::unordered_map<int, std::vector<TimePoint>> timeHits;
    auto startTime = std::chrono::steady_clock::now();
    auto endTime = startTime, sendStart = startTime + std::chrono::milliseconds(1000);

    while (cont)
    {
        val = 0;
        for (int i = 0; i < 2; ++i)
        {
            endTime = startTime + std::chrono::milliseconds(10);
            std::this_thread::sleep_until(endTime);
            for (auto& [key, v] : offset) {
                if (GetAsyncKeyState(key) & 0x8000) {
                    KPS += incrementKPS(timeHits, lastPress, key);
                    lastPress[key] = true;
                    val |= 1 << v;
                } else {
                    lastPress[key] = false;
                }
            }
            startTime = endTime;
        }
        KPS -= decrementKPS(timeHits);
        emit keyStroke(val | (KPS << 10)); // 000000KKKKKKKKKKKKKKKK0006543210
        if (serialRun && startTime > sendStart) {
            serial.WriteSerialPort(QString("#%1#").arg(val | (KPS << 10)).toStdString());
        }
    }
    serial.CloseSerialPort();
}

void ComputerKS::stop()
{
    cont = false;
}
