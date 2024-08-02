/* autoplayerkey.h */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: autoplayerkey.h :: VSRG-UNO-R3: Column Presser Object
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> Header file, check implementation file for detailed class explanation.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#ifndef AUTOPLAYERKEY_H
#define AUTOPLAYERKEY_H
#include <QObject>

class AutoPlayerKey : public QObject {
    Q_OBJECT

public:
    explicit AutoPlayerKey(
        const QVector<QPair<quint32, quint32>>&,
        const quint32&,
        const quint8 &,
        QObject* = nullptr
    );

public slots:
    void delay( const qint16& );
    void stop_SIG();
    void runner();

private:
    const QVector<QPair<quint32, quint32>>&
                   hitVector;
    const quint32& startDelay;
    const quint8 & key;
    qint16         shift = 0;
    bool           cont = true;
};
#endif // AUTOPLAYERKEY_H :: - -                                                       - -
