#include "arduinoap.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <Windows.h>
#include <QDebug>

ArduinoAP::ArduinoAP(const QVector<uint8_t>& vk, const QString& port, QObject *parent) : QObject{parent}
{
    this->vk = vk;
    this->portAU = port;
}

void ArduinoAP::stop() { cont = false; }

void ArduinoAP::run()
{
    if (portAU == "-1") {
        emit errorMsg(" : : Bad Arduino Port.");
        return;
    }

    QSerialPort serial;
    serial.setPortName(portAU);
    serial.setBaudRate(QSerialPort::Baud9600);

    if (!serial.open(QIODevice::ReadOnly)) {
        emit errorMsg(" : : Bad Port Open");
        return;
    }

    QMap<uint8_t, uint8_t> m_vk;
    uint8_t shifted = qPow(2, vk.size() - 1);
    for (int8_t i = vk.size() - 1; i >= 0; --i)
    {
        keybd_event(static_cast<BYTE>(vk[i]), 0, KEYEVENTF_KEYUP, 0);
        m_vk[shifted] = vk[i];
        shifted >>= 1;
    }
    shifted = qPow(2, vk.size() - 1);

    int value;
    while (cont)
    {
        value = 0;
        while (serial.waitForReadyRead(-1))
        {
            serial.waitForReadyRead(5);
            QByteArray data = serial.readAll();
            value = data.toInt();
            break;
        }
        for (uint8_t i = 1; i <= shifted; i <<= 1)
        {
            // Key Press ::
            if (value & i) {
                keybd_event(static_cast<BYTE>(m_vk[i]), 0, 0, 0);
            }
            // Release ::
            else {
                keybd_event(static_cast<BYTE>(m_vk[i]), 0, KEYEVENTF_KEYUP, 0);
            }
        }
    }
    serial.close();
}
