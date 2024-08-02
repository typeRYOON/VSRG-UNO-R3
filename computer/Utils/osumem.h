/* osumem.h */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: osumem.h :: VSRG-UNO-R3: Osu Memory Checker Object
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> Header file, check implementation file for detailed class explanation.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#ifndef OSUMEM_H
#define OSUMEM_H
#include <QWebSocket>
#include <QObject>

class OsuMem : public QObject {
    Q_OBJECT

signals:
    void imgbox_SIG(
        const QString&,
        const QString&,
        const QString&,
        const QString&
    );
    void sendOsuConfigs(
        const QString&,
        const QVector<QVector<quint8>>&,
        const quint8 &
    );
    void keyChange(
        const quint8 &,
        const QString&,
        const QString&,
        const QString&,
        const QString&
    );
    void consolebox_SIG(
        const QString&,
        const QString&,
        const QString&,
        const QString&,
        const QString&
    );
    void onConnect_ConsoleBox_SIG( const QString& );
    void resetPaging( const quint8& );
    void memosu_err( const QString& );
    void playChange( const bool& );

public:
    OsuMem();

public slots:
    void openBeatmapLink();
    void runner();

private:
    const QVector<QVector<quint8>> makeKeyVector( const QVector<QString>& );
    void readOsu( const QString&, const qint8& );

    quint8       curK = 0;
    QWebSocket * webSocket;
    QStringList* osuInfo;
    QString      prev;
    bool         inPlay;

private slots:
    void socketConnect();
};
#endif // OSUMEM_H :: - -                                                              - -
