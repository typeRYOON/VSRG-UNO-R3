/* osuhandler.h */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: osuhandler.h :: VSRG-UNO-R3: Configuration Handling Object
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> Header file, check implementation file for detailed class explanation.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#ifndef OSUHANDLER_H
#define OSUHANDLER_H
#include "../Utils/qcustomplot.h"
#include "../Utils/utils.h"
#include <QObject>

class OsuHandler : public QObject {
    Q_OBJECT

signals:
    void keys( const quint8&, const quint8& );
    void osuhandler_err( const QString& );
    void consoleBox_SIG( const bool& );
    void playChange_SIG( const bool& );
    void delay( const qint16& );
    void osuhandler_run();
    void stop_SIG();
    void sendHistoryNode(
        const QPixmap&,
        const QPixmap&,
        const QPair<QString, QString>&,
        const quint8 &
    );
    void sendObjectData(
        const QString&,
        const QString&,
        const QString&,
        const QString&
    );

public:
    explicit OsuHandler( Utils*, QObject* = nullptr );

public slots:
    void keyChange(
        const quint8 &,
        const QString&,
        const QString&,
        const QString&,
        const QString&
    );
    void setOsuConfig(
        const QString&,
        const QVector<QVector<quint8>>&,
        const quint8 &
    );
    void portChange( const quint8&, const QString& );
    void logKPS( const quint8&, const quint8& );
    void getButtonState( const quint8& );
    void errorRouter( const QString& );
    void playChange( const bool& );
    void runner();

private:
    Utils* u;
    void addHistoryNode();
    void downSample( QVector<double>&, const quint16& );
    QVector<QVector<quint8>> vk;
    bool            inConfig = false, inPlay = false, sent = false;
    QString         beatmapDir = "", osuPath = "", bgPath = "", diff = "";
    QString         title = "-+_",   port1 = "-1", port2 = "-1";
    QVector<double> KPS_lst, xData;
    quint8          buttonBMap = 0, curK;
    QCustomPlot*    plot;
    QCPGraph*       graph;
};

#endif // OSUHANDLER_H
