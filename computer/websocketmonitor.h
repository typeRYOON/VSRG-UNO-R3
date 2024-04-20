#ifndef WEBSOCKETMONITOR_H
#define WEBSOCKETMONITOR_H

#include <QObject>
#include <QWebSocket>

class WebSocketMonitor : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketMonitor(QObject *parent = nullptr);

signals:
    void jsonDataReceived(QStringList& osuInfo);
    void websocketError(const QString& errorMessage);
    void playChange(const bool);
    void readOsu(const QString& osuDir);
    void term();

public slots:
    void startMonitoring();

private slots:
    void onWebSocketConnected();
    void onWebSocketError(QAbstractSocket::SocketError error);

private:
    QWebSocket webSocket;
    QStringList osuInfo;
    QString prev = "";
    int count = 0;
    int countP = 0;
    bool inPlay = false;
};

#endif // WEBSOCKETMONITOR_H
