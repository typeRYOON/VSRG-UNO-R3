#ifndef ARDUINOHANDLER_H
#define ARDUINOHANDLER_H
#include <QObject>
#include <QTimer>

class ArduinoHandler : public QObject
{
    Q_OBJECT

signals:
    void sendKStoMain(const uint16_t);
    void resetKeyStrokes();
    void stopThread();
    void stopC_KS();
    void stopC_AP();
    void stopA_AP();
    void running();
    void msg(const QString&);

public slots:
    void keyStroke(const uint16_t);
    void receiveVirtualKeys(
        const QVector<uint8_t>&,
        const QVector<uint8_t>&,
        const QVector<uint8_t>&,
        const QVector<uint8_t>&);
    void receiveOsuFile(const QString&);
    void receiveTermination();
    void receiveMsg(const QString&);

public:
    explicit ArduinoHandler(QObject* parent = nullptr);
    void checkForH();
    void checkForJ();
    void run(uint8_t, bool, bool, bool, bool, bool, bool, QString, QString);

private:
    QVector<uint8_t> virtual4k;
    QVector<uint8_t> virtual5k;
    QVector<uint8_t> virtual6k;
    QVector<uint8_t> virtual7k;
    QString osuFile;
    bool cont = true;
};

#endif // ARDUINOHANDLER_H
