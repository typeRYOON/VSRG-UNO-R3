#ifndef COMPUTERKS_H
#define COMPUTERKS_H
#include <QObject>

class ComputerKS : public QObject
{
    Q_OBJECT
public:
    explicit ComputerKS(QVector<uint8_t>&, QString&, bool, QObject *parent = nullptr);

signals:
    void keyStroke(const uint16_t val);
    void errorMsg(const QString&);

public slots:
    void run();
    void stop();

private:
    bool cont = true;
    bool serialRun = false;
    QVector<uint8_t> virtualKeys;
    QString port;
};

#endif // COMPUTERKS_H
