#ifndef ARDUINOAP_H
#define ARDUINOAP_H
#include <QObject>

class ArduinoAP : public QObject
{
    Q_OBJECT
signals:
    void errorMsg(const QString&);

public slots:
    void stop();
    void run();

public:
    explicit ArduinoAP(const QVector<uint8_t>&, const QString&, QObject* parent = nullptr);

private:
    bool cont = true;
    QVector<uint8_t> vk;
    QString portAU;
};

#endif // ARDUINOAP_H
