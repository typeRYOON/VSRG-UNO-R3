/* keyreader.h */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: keyreader.h :: VSRG-UNO-R3: Key Reading Object
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> Header file, check implementation file for detailed class explanation.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#ifndef KEYREADER_H
#define KEYREADER_H
#include <QObject>

class KeyReader : public QObject {
    Q_OBJECT

signals:
    void keys( const quint8&, const quint8& );
    void keyreader_err( const QString& );

public:
    explicit KeyReader(
        const QVector<quint8>&,
        const quint8 &,
        const quint8 &,
        const QString&,
        QObject* = nullptr
    );

public slots:
    void stop_SIG();
    void runner();

private:
    quint8          buttonBMap, curK;
    bool            cont = true;
    QString         port;
    QVector<quint8> vk;
};

#endif // KEYREADER_H :: - -                                                           - -
