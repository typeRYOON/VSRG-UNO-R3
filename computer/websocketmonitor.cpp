#include "websocketmonitor.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>


WebSocketMonitor::WebSocketMonitor(QObject *parent) : QObject{parent}
{
    osuInfo = QStringList(9, QString());
}


void WebSocketMonitor::startMonitoring()
{
    connect(&webSocket, &QWebSocket::connected, this, &WebSocketMonitor::onWebSocketConnected);
    connect(&webSocket, &QWebSocket::errorOccurred, this, &WebSocketMonitor::onWebSocketError);
    webSocket.open(QUrl("ws://localhost:24050/ws"));
}


void WebSocketMonitor::onWebSocketConnected()
{
    // WebSocket is active, start monitoring for JSON data
    connect(&webSocket, &QWebSocket::textMessageReceived, this, [&](const QString& message)
    {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(message.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        // Game path ::
        if (osuInfo.at(0).isEmpty()) {
            if (!jsonObject.contains("menu")) {
                emit websocketError("Invalid JSON.");
            }
            QJsonObject set = jsonObject.value("settings").toObject();
            QJsonObject folders = set.value("folders").toObject();
            osuInfo.replace(0, folders.value("game").toString());
            emit readOsu(osuInfo.at(0));
        }

        QJsonObject menu = jsonObject.value("menu").toObject();

        // Don't update during gameplay ::
        if (menu.value("state").toInt() == 2) {
            if (!inPlay) {
                inPlay = true;
                emit playChange(true);
            }
            return;
        }
        else {
            if (inPlay) {
                inPlay = false;
                emit playChange(false);
            }
        }
        QJsonObject bm = menu.value("bm").toObject();
        int x = -1, y = -1;
        if (bm.contains("set") && bm.contains("id")) {
            x = bm.value("set").toInt(), y = bm.value("id").toInt();
        }

        osuInfo.replace(8, QString("https://osu.ppy.sh/beatmapsets/%1#mania/%2").arg(x).arg(y));

        // Skip the first packet ::
        if (prev != osuInfo.at(8))
        {
            if (count == 0) {
                count++;
                return;
            }
            if (count == 1) {
                prev = osuInfo.at(8);
                count = 0;
            }
        }
        else {
            if (count == 0) {
                return;
            }
        }

        QJsonObject stats = bm.value("stats").toObject();
        double SR = stats.value("SR").toDouble();
        if (SR >= 10) {
            osuInfo.replace(1, QString::number(SR, 'f', 1));
        } else {
            osuInfo.replace(1, QString::number(SR, 'f', 2));
        }
        osuInfo.replace(4, QString::number(stats.value("memoryCS").toInt()));

        QJsonObject meta = bm.value("metadata").toObject();
        osuInfo.replace(2, meta.value("titleOriginal").toString());
        osuInfo.replace(3, meta.value("difficulty").toString());

        QJsonObject paths = bm.value("path").toObject();
        osuInfo.replace(5, paths.value("folder").toString());
        osuInfo.replace(6, paths.value("file").toString());
        osuInfo.replace(7, paths.value("bg").toString());

        prev = osuInfo.at(8);

        emit jsonDataReceived(osuInfo);
    });
}


void WebSocketMonitor::onWebSocketError(QAbstractSocket::SocketError error)
{
    emit websocketError(webSocket.errorString());
}
