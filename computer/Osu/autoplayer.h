/* autoplayer.h */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: autoplayer.h :: VSRG-UNO-R3: Column Presser Handler
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> Header file, check implementation file for detailed class explanation.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#ifndef AUTOPLAYER_H
#define AUTOPLAYER_H
#include <QObject>
#include <QFile>

class AutoPlayer : public QObject {
    Q_OBJECT

signals:
    void autoplayer_err( const QString& );
    void delay( const qint16& );
    void stop_SIG_push();
    void sendObjectData(
        const QString&,
        const QString&,
        const QString&,
        const QString&
    );

public:
    explicit AutoPlayer(
        const QVector<quint8>&,
        const QString&,
        const QString&,
        QObject* = nullptr
    );

public slots:
    void stop_SIG();
    void arduinoAuto();
    void computerAuto();

private:
    bool   readBeatmap( QFile* );
    qint32 setStartDelay();
    QVector<QVector<QPair<quint32, quint32>>>
                    hitChart;
    QVector<quint8> vk;
    QString         beatmapDir, port;
    bool            cont = true;
    quint8          nKey = 0;
};
#endif // AUTOPLAYER_H :: - -                                                          - -
