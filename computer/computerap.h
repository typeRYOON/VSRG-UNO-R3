#ifndef COMPUTERAP_H
#define COMPUTERAP_H

#include <QObject>

class ComputerAP : public QObject
{
    Q_OBJECT

signals:
    void stopKeyThreads();
    void errorMsg(const QString&);

public slots:
    void stop();
    void run();

public:
    explicit ComputerAP(
        const QVector<uint8_t>&,
        const QString&,
        QObject *parent = nullptr);

private:
    bool cont = true;
    bool readOsuFile();
    bool setStartDelay();
    QVector<QVector<QPair<uint32_t, uint32_t>>> hitChart;
    QVector<uint8_t> vk;
    QString osuFile;
    int8_t keyCount = -1;
};

#endif // COMPUTERAP_H
