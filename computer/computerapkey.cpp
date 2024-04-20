#include "computerapkey.h"
#include <windows.h>
#include <QDebug>
#include <chrono>
#include <thread>


ComputerAPKey::ComputerAPKey(
    QVector<QPair<uint32_t, uint32_t>>& pHitVectora,
    uint32_t& pStartDelay,
    uint8_t& pVirtualKey,
    QObject *parent
    ) : QObject{parent}, hitVector{pHitVectora}, startDelay{pStartDelay}, virtualKey{pVirtualKey} {}

void ComputerAPKey::stop() { cont = false; }

void ComputerAPKey::processKey()
{
    auto startTime = std::chrono::high_resolution_clock::now();
    auto expected = startTime + std::chrono::milliseconds(startDelay);
    if (startDelay) { std::this_thread::sleep_until(expected); }

    startTime = expected;
    for (const auto& pair : hitVector)
    {
        if (!cont) { break; }
        keybd_event(static_cast<BYTE>(virtualKey), 0, 0, 0);

        std::this_thread::sleep_for(std::chrono::milliseconds(pair.first));
        keybd_event(static_cast<BYTE>(virtualKey), 0, KEYEVENTF_KEYUP, 0);

        expected = startTime + std::chrono::milliseconds(pair.second + pair.first);
        std::this_thread::sleep_until(expected);
        startTime = expected;
    }
}
