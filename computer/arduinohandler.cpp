#include "arduinohandler.h"
#include "computerks.h"
#include "computerap.h"
#include "arduinoap.h"
#include <Windows.h>
#include <QThread>
#include <Thread>
#include <chrono>
#include <QDebug>

ArduinoHandler::ArduinoHandler(QObject *parent) : QObject{parent} {}

void ArduinoHandler::keyStroke(const uint16_t val) { emit sendKStoMain(val); }

void ArduinoHandler::receiveVirtualKeys(
    const QVector<uint8_t>& k4,
    const QVector<uint8_t>& k5,
    const QVector<uint8_t>& k6,
    const QVector<uint8_t>& k7)
{
    virtual4k = k4;
    virtual5k = k5;
    virtual6k = k6;
    virtual7k = k7;
}

void ArduinoHandler::receiveOsuFile(const QString& osuFile) { this->osuFile = osuFile; }

void ArduinoHandler::receiveTermination() { cont = false; }

void ArduinoHandler::receiveMsg(const QString& message)
{
    cont = false;
    emit msg(message);
}

void ArduinoHandler::run
    (uint8_t keyC, bool bA_KS, bool bA_KPS, bool bA_AP, bool bC_KS, bool bC_KPS, bool bC_AP,
     QString KPS_KS, QString AU)
{
    QThread* C_KSThread = nullptr;
    QThread* C_APThread = nullptr;
    QThread* A_APThread = nullptr;

    // Arduino Port Check :: - -                                                       - -
    if ((bA_KS || bA_KPS) && KPS_KS == "-1") { receiveMsg(" : : Bad Arduino Port."); }
    if (bA_AP && AU == "-1")                 { receiveMsg(" : : Bad Arduino Port."); }

    // Pre-run H Key Check :: - -                                                      - -
    while (!(GetAsyncKeyState('H') & 0x8000) && cont) {
        if ((GetAsyncKeyState('J') & 0x8000)) { cont = false; }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Virtual Key Filtering :: - -                                                    - -
    QVector<uint8_t> vk;
    if      (keyC == 4) { vk = virtual4k; }
    else if (keyC == 5) { vk = virtual5k; }
    else if (keyC == 6) { vk = virtual6k; }
    else if (keyC == 7) { vk = virtual7k; }

    // Computer/Arduino Key-Strokes / KPS :: - -                                       - -
    if (cont && (bC_KS || bC_KPS || bA_KS || bA_KPS))
    {
        C_KSThread = new QThread();
        auto C_KS = new ComputerKS(vk, KPS_KS, bA_KS || bA_KPS);
        C_KS->moveToThread(C_KSThread);
        connect(C_KSThread, &QThread::started, C_KS, &ComputerKS::run);
        connect(C_KS, &ComputerKS::errorMsg, this, &ArduinoHandler::receiveMsg, Qt::DirectConnection);
        connect(C_KS, &ComputerKS::keyStroke, this, &ArduinoHandler::keyStroke, Qt::DirectConnection);
        connect(this, &ArduinoHandler::stopC_KS, C_KS, &ComputerKS::stop,       Qt::DirectConnection);
        C_KSThread->start();
    }

    // Computer Autoplayer :: - -                                                      - -
    if (cont && bC_AP)
    {
        C_APThread = new QThread();
        auto C_AP = new ComputerAP(vk, osuFile);
        C_AP->moveToThread(C_APThread);
        connect(C_APThread, &QThread::started, C_AP, &ComputerAP::run);
        connect(C_AP, &ComputerAP::errorMsg, this, &ArduinoHandler::receiveMsg, Qt::DirectConnection);
        connect(this, &ArduinoHandler::stopC_AP, C_AP, &ComputerAP::stop, Qt::DirectConnection);
        C_APThread->start();
    }

    // Arduino Autoplayer :: - -                                                       - -
    if (cont && bA_AP)
    {
        A_APThread = new QThread();
        auto A_AP = new ArduinoAP(vk, AU);
        A_AP->moveToThread(A_APThread);
        connect(A_APThread, &QThread::started, A_AP, &ArduinoAP::run);
        connect(A_AP, &ArduinoAP::errorMsg, this, &ArduinoHandler::receiveMsg, Qt::DirectConnection);
        connect(this, &ArduinoHandler::stopA_AP, A_AP, &ArduinoAP::stop, Qt::DirectConnection);
        A_APThread->start();
    }

    // Emit to MainWindow :: - -                                                       - -
    if (cont) { emit running(); }

    // Hang Loop :: - -                                                                - -
    while (!(GetAsyncKeyState('J') & 0x8000) && cont) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }   cont = true;

    // Terminate Custom Objects/Threads :: - -                                         - -
    if (C_KSThread) {
        emit stopC_KS();
        C_KSThread->quit();
        C_KSThread->wait();
        delete C_KSThread;
        emit resetKeyStrokes();
    }

    if (C_APThread) {
        emit stopC_AP();
        C_APThread->quit();
        C_APThread->wait();
        delete C_APThread;
    }

    if (A_APThread) {
        emit stopA_AP();
        A_APThread->quit();
        A_APThread->wait();
        delete A_APThread;
    }
    emit stopThread();
}
