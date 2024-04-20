#ifndef COMPUTERAPKEY_H
#define COMPUTERAPKEY_H

#include <QObject>

class ComputerAPKey : public QObject
{
    Q_OBJECT
public:
    explicit ComputerAPKey(
        QVector<QPair<uint32_t, uint32_t>>&,
        uint32_t&,
        uint8_t&,
        QObject *parent = nullptr);

private:
    bool cont = true;
    QVector<QPair<uint32_t, uint32_t>>& hitVector;
    uint32_t& startDelay;
    uint8_t& virtualKey;

public slots:
    void processKey();
    void stop();

signals:
};

#endif // COMPUTERAPKEY_H
