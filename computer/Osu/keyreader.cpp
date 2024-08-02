/* keyreader.cpp */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: keyreader.cpp :: VSRG-UNO-R3: Key Reading Object
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> This file's class object handles KPS and KS value creation and emission. 
 >> KS values represent 7 key columns starting on bit 2 of the number, 0b1111 1110
 >> represents all seven keys pressed. KPS values increment on press (not hold), and
 >> decrement after 1 second, the press time is unique between each key press.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#include "keyreader.h"
#include <QSerialPort>
#include <windows.h>
#include <thread>
#include <QMap>
using TimePoint = std::chrono::steady_clock::time_point;

// Constructor set :: - -                                                              - -
KeyReader::KeyReader(
    const QVector<quint8>& vk,
    const quint8& buttonBMap,
    const quint8& curK,
    const QString& port,
    QObject* p
) : vk( vk ), buttonBMap( buttonBMap ), port( port ), curK( curK ), QObject{ p } {}

// Terminate this thread gracefully :: - -                                             - -
void KeyReader::stop_SIG() { cont = false; }

// Increment KPS on new press, not hold :: - -                                         - -
qint16 incrementKPS(
    QMap<quint8, QVector<TimePoint>>& timeHits,
    QMap<quint8, bool>& lastPress,
    const quint8& vkey )
{
    if ( lastPress[vkey] ) { return 0; }
    timeHits[vkey] << std::chrono::steady_clock::now();
    return 1;
}

// Decrement KPS by removing >1s key press times  in timeHits container :: - -         - -
qint16 decrementKPS( QMap<quint8, QVector<TimePoint>>& timeHits )
{
    qint16 initial, countDelta = 0;
    auto now = std::chrono::steady_clock::now();
    QMap<quint8, QVector<TimePoint>>::iterator i;

    for ( i = timeHits.begin(); i != timeHits.end(); ++i )
    {
        initial = i.value().size();
        i.value().erase(
            std::remove_if(
                i.value().begin(),
                i.value().end(),
                [now]( const TimePoint& timePoint )
                {
                    return std::chrono::duration_cast<std::chrono::milliseconds>
                        ( now - timePoint ).count() >= 1000;
                }
            ),
            i.value().end()
        );
        countDelta += initial - i.value().size();
    }
    return countDelta;
}

// Runner function for KeyReader :: - -                                                - -
void KeyReader::runner()
{
    QMap<quint8,  QVector<TimePoint>> timeHits;
    QMap<quint32, quint32>::const_iterator i;
    QMap<quint32, quint32> offset;
    QMap<quint8, bool> lastPress;
    quint8 col = 0, KPS = 0, KS;

    auto startTime = std::chrono::steady_clock::now();
    auto endTime   = startTime,
         sendStart = startTime + std::chrono::milliseconds( 1000 );

    const bool   serialRun = buttonBMap & 0x18;
    const quint8 ksRun_c   = buttonBMap & 0x01 ? 0xFF : 0x0;
    const quint8 kpsRun_c  = buttonBMap & 0x02 ? 0xFF : 0x0;
    const quint8 ksRun_a   = buttonBMap & 0x08 ? 0xFF : 0x0;
    const quint8 kpsRun_a  = buttonBMap & 0x10 ? 0xFF : 0x0;

    // Set up optional Arduino serial connection :: - -                                - -
    QSerialPort arduinoSerial( port, this );
    if ( serialRun && !arduinoSerial.open( QIODevice::WriteOnly ) ) {
        emit keyreader_err( "[0x3 : Failed to open port: " + port + ']' ); return;
    }

    // Default container values :: - -                                                 - -
    foreach ( const quint8& vkey, vk ) {
        lastPress[vkey] = false;
        offset   [vkey] = col++;
    }
    while ( cont )
    {
        KS = 0x0;
        // For each 20ms, check key states every 10ms, then send out :: - -            - -
        for ( quint8 iter = 0; iter < 2; ++iter )
        {
            endTime = startTime + std::chrono::milliseconds( 10 );
            std::this_thread::sleep_until( endTime );

            // For each key: check press state and add/substract accordingly :: - -    - -
            for ( i = offset.constBegin(); i != offset.constEnd(); ++i )
            {
                if ( GetAsyncKeyState( i.key() ) & 0x8000 ) {
                    KS  |= 1 << i.value();
                    KPS += incrementKPS( timeHits, lastPress, i.key() );
                    lastPress[i.key()]    = true;
                }
                else { lastPress[i.key()] = false; }
            }
            startTime = endTime;
        }
        KPS -= decrementKPS( timeHits );

        // Send out KPS and KS values :: - -                                           - -
        emit keys( KPS & kpsRun_c, KS & ksRun_c );
        if ( serialRun && startTime > sendStart )
        {
            arduinoSerial.write(
                QString( "#%1#" ).arg(
                    KS & ksRun_a  | ( ( KPS << 10 ) & kpsRun_a )
                ).toLocal8Bit().constData()
            );
        }
    }
    arduinoSerial.close();
}   // :: - -                                                                          - -
