#include "computerapkey.h"
#include "computerap.h"
#include <QThread>
#include <QDebug>
#include <QFile>

ComputerAP::ComputerAP(
    const QVector<uint8_t>& vk,
    const QString& osuFile,
    QObject* parent) : QObject{parent}
{
    this->osuFile = osuFile;
    this->vk = vk;
}

bool ComputerAP::setStartDelay()
{
    uint32_t earliest = UINT32_MAX;

    // Find the earliest time value among the first notes in each column:
    for (uint8_t i = 0; i < keyCount; ++i) {
        if (hitChart[i].isEmpty()) { return true; }
        if (hitChart[i][0].first < earliest) {
            earliest = hitChart[i][0].first;
        }
    }

    // Adjust the time values of the first notes in each column:
    for (uint8_t i = 0; i < keyCount; ++i) {
        hitChart[keyCount].push_back(
            qMakePair(hitChart[i][0].first - earliest, 0)
        );
    }
    return false;
}

bool ComputerAP::readOsuFile()
{
    QFile file(osuFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return true;
    }

    QTextStream in(&file);
    QString line, mode;
    bool inDiff = false, inGen = false;

    while (!in.atEnd())
    {
        line = in.readLine().trimmed();

        // Check for section beginnings
        if (line.startsWith("[General]")) {
            inDiff = false, inGen = true;
            continue;
        } else if (line.startsWith("[Difficulty]")) {
            inDiff = true,  inGen = false;
            continue;
        } else if (line.startsWith("[HitObjects]")) {
            break;
        }

        // Parse key-value pairs within sections
        if (inGen) {
            if (line.startsWith("Mode:")) {
                mode = line.mid(5).trimmed();
            }
        } else if (inDiff) {
            if (line.startsWith("CircleSize:")) {
                keyCount = line.mid(11).trimmed().toInt();
            }
        }
    }
    // Check Assertions :: - -
    if (mode != "3") {
        emit errorMsg(" : : Not Osu! Mania.");
        return true;
    }
    if (keyCount < 4 || keyCount > 7) {
        emit errorMsg(" : : Invalid Key Count.");
        return true;
    }

    hitChart.resize(keyCount + 1);
    uint32_t holdTime, delayTime, endTime;
    uint32_t startTime, column;

    while (!in.atEnd())
    {
        line = in.readLine().trimmed();
        QStringList lineParts = line.split(',');

        // X/Column calculation:
        column = lineParts[0].toUInt() * keyCount / 0x200;

        // Time:
        startTime = lineParts[2].toUInt();

        // Endtime:
        endTime = lineParts[5].split(":").first().toUInt();
        hitChart[column].push_back(qMakePair(startTime, endTime));
    }
    file.close();
    if (setStartDelay())
    {
        emit errorMsg(" : : A Column is empty.");
        return true;
    }

    uint32_t n;
    for (uint32_t i = 0; i < keyCount; ++i)
    {
        n = hitChart[i].size() - 1;

        for (uint32_t j = 0; j < n; ++j) {
            holdTime  = hitChart[i][j].second == 0 ? 5 : hitChart[i][j].second - hitChart[i][j].first;
            delayTime = hitChart[i][j + 1].first - hitChart[i][j].first - holdTime;
            if (hitChart[i][j + 1].first < (hitChart[i][j].first + holdTime)) {
                emit errorMsg(" : : Overlapping note!");
                emit errorMsg(QString(" : : Column: %1").arg(i));
                emit errorMsg(QString(" : : Time(ms): %1").arg(hitChart[i][j + 1].first)); //
                return true;
            }
            hitChart[i][j].first  = holdTime;
            hitChart[i][j].second = delayTime;
        }
        holdTime = hitChart[i][n].second == 0 ? 5 : hitChart[i][n].second - hitChart[i][n].first;
        hitChart[i][n].first = holdTime;
        hitChart[i][n].second = 0;
    }
    return false;
}


// read osu file, check if valid.
// read hit timings into vector of n key vectors
// create n seperate threads for each key.

void ComputerAP::run()
{
    if (readOsuFile()) { return; }

    QVector<ComputerAPKey*> obj(keyCount, nullptr);
    QVector<QThread*> threads(keyCount, nullptr);

    for (int i = 0; i < keyCount; ++i) {
        // Create an instance of CustomObject for each column vector
        obj[i] = new ComputerAPKey(hitChart[i], hitChart[keyCount][i].first, vk[i]);

        // Create a thread and move the custom object to the thread
        threads[i] = new QThread;
        obj[i]->moveToThread(threads[i]);

        // Connect signals for thread management
        connect(threads[i], &QThread::started, obj[i], &ComputerAPKey::processKey, Qt::DirectConnection);
        connect(threads[i], &QThread::finished, obj[i], &ComputerAPKey::deleteLater, Qt::DirectConnection);
        connect(threads[i], &QThread::finished, threads[i], &QThread::deleteLater, Qt::DirectConnection);
        connect(this, &ComputerAP::stopKeyThreads, obj[i], &ComputerAPKey::stop, Qt::DirectConnection);
    }

    for (int i = 0; i < keyCount; ++i) { threads[i]->start(); }
    while (cont) { QThread::msleep(1000); }

    emit stopKeyThreads();
    for (QThread* thread : threads) {
        thread->quit();
        thread->wait();
        delete thread;
    }
}


void ComputerAP::stop() { cont = false; }
