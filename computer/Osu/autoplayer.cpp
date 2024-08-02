/* autoplayer.cpp */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: autoplayer.cpp :: VSRG-UNO-R3: Column Presser Handler
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> This file's class object is to either run the Arduino or Computer auto player
 >> feature. For the Arduino side, it will read in a bit mapping of the columns, which
 >> will then be used to press the corresponding keys. For the Computer side, the program
 >> reads a osu beatmap file (.osu), does error checking, transforms file hit objects
 >> to hit object vector. This program then presses the keys by instantiating n key
 >> threads and pushes each column timing vector to each thread.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#include "autoplayerkey.h"
#include "autoplayer.h"
#include <QSerialPort>
#include <QTextStream>
#include <windows.h>
#include <QThread>
#include <QMap>

// Initial data set :: - -                                                             - -
AutoPlayer::AutoPlayer(
    const QVector<quint8>& vk,
    const QString& beatmapDir,
    const QString& port,
    QObject* p
) : vk(vk), beatmapDir( beatmapDir ), port( port ), QObject{ p } {
    this->nKey = vk.size();
}

// Terminate this thread gracefully :: - -                                             - -
void AutoPlayer::stop_SIG() { cont = false; }

// Arduino Autoplayer :: - -                                                           - -
void AutoPlayer::arduinoAuto()
{
    QSerialPort arduinoSerial( port, this );
    if ( !arduinoSerial.open( QIODevice::ReadOnly ) ) {
        emit autoplayer_err( "[0x3 : Failed to open port: " + port + ']' ); return;
    }

    QMap<quint8, quint8> vk_mp;
    uint8_t shifted = qPow( 2, nKey - 1 );
    for ( quint8 i = nKey - 1; i >= 0; --i )
    {
        keybd_event( vk.at(i), 0, KEYEVENTF_KEYUP, 0 );
        vk_mp[shifted] = vk.at(i);
        shifted      >>= 1;
    }
    shifted = qPow( 2, nKey - 1 );

    quint16 value;
    while ( cont )
    {
        value = 0;
        // Wait for Arduino bit mapping value :: - -                                   - -
        while ( arduinoSerial.waitForReadyRead( -1 ) )
        {
            arduinoSerial.waitForReadyRead( 5 );
            QByteArray data = arduinoSerial.readAll();
            value = data.toUShort();
            break;
        }
        for ( quint8 i = 1; i <= shifted; i <<= 1 )
        {
            // Key Press ::
            if ( value & i ) {
                keybd_event( vk_mp[i], 0, 0, 0 );
            }
            // Release ::
            else {
                keybd_event( vk_mp[i], 0, KEYEVENTF_KEYUP, 0 );
            }
        }
    }
    arduinoSerial.close();
}

// Computer Autoplayer :: - -                                                          - -
void AutoPlayer::computerAuto()
{
    QFile file( beatmapDir );
    bool fileError = readBeatmap( &file );
    file.close();

    if ( fileError ) { return; }

    QVector<AutoPlayerKey*> obj    ( nKey, nullptr );
    QVector<QThread*>       threads( nKey, nullptr );

    for ( quint8 i = 0; i < nKey; ++i )
    {
        threads[i] = new QThread;
        obj[i] = new AutoPlayerKey(
            hitChart.at(i), hitChart.at(nKey).at(i).first, vk.at(i)
        );
        obj.at(i)->moveToThread( threads.at(i) );

        // Connections :: - -                                                          - -
        connect(
            threads.at(i), &QThread      ::started,
            obj.at(i),     &AutoPlayerKey::runner,      Qt::DirectConnection);
        connect(
            threads.at(i), &QThread      ::finished,
            obj.at(i),     &AutoPlayerKey::deleteLater, Qt::DirectConnection);
        connect(
            threads.at(i), &QThread      ::finished,
            threads.at(i), &QThread      ::deleteLater, Qt::DirectConnection);
        connect(
            this,          &AutoPlayer   ::delay,
            obj.at(i),     &AutoPlayerKey::delay,       Qt::DirectConnection);
        connect(
            this,          &AutoPlayer   ::stop_SIG_push,
            obj.at(i),     &AutoPlayerKey::stop_SIG,    Qt::DirectConnection);
    }
    for ( quint8 i = 0; i < nKey; ++i ) { threads.at(i)->start(); }
    while ( cont )
    {
        if      ( GetAsyncKeyState( '1' ) & 0x8000 ) { emit delay( -3 ); }
        else if ( GetAsyncKeyState( '2' ) & 0x8000 ) { emit delay( +3 ); }
        QThread::sleep( 1 );
    }

    emit stop_SIG_push();
    for ( quint8 i = 0; i < nKey; ++i ) {
        threads.at(i)->quit();
        threads.at(i)->wait();
        delete threads.at(i);
    }
}


// Read beatmap file timings into timing container :: - -                              - -
// ret bool, true = err, false = good :: - -                                           - -
bool AutoPlayer::readBeatmap( QFile* file )
{
    if ( !file->open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        emit autoplayer_err( "[0x4 : Osu file open error]" ); return true;
    }

    QTextStream in( file );
    bool inDiff = false, inGen = false;
    QStringList lineParts;
    QString     line;

    // Check if beatmap is 4-7K, mania, and get textstream next to hit objs :: - -     - -
    while ( !in.atEnd() )
    {
        line = in.readLine().trimmed();

        // Check for section beginnings :: - -                                         - -
        if ( line.startsWith( "[General]" ) ) {
            inDiff = false, inGen = true;  continue;
        } else if ( line.startsWith( "[Difficulty]" ) ) {
            inDiff = true,  inGen = false; continue;
        }
        else if ( line.startsWith( "[HitObjects]" ) ) { break; }

        // Check if valid file :: - -                                                  - -
        if ( inGen ) {
            if ( line.startsWith( "Mode:" )
                 && line.mid( 5 ).trimmed() != "3" )
            {
                emit autoplayer_err( "[0x5 : Mania maps only]" ); return true;
            }
        } else if ( inDiff ) {
            if ( line.startsWith( "CircleSize:" )
                 && vk.size() != line.mid( 11 ).trimmed().toInt() )
            {
                emit autoplayer_err( "[0x6 : 4-7K maps only]" ); return true;
            }
        }
    }
    // First nKey elements are the actual key hit timings, last = start delays :: - -  - -
    hitChart.resize( nKey + 1 );
    quint32 column, startTime, endTime;
    quint32 holdTime, delayTime, n;

    // https://osu.ppy.sh/wiki/en/Client/File_formats/osu_%28file_format%29#osu!mania  - -
    while ( !in.atEnd() )
    {   // Parse beatmap file hit objects into parts :: - -                            - -
        line              = in.readLine().trimmed();
        lineParts         = line.split( ',' );

        // Column:
        column            = lineParts.at(0).toUInt() * nKey / 0x200;

        // StartTime:
        startTime         = lineParts.at(2).toUInt();

        // EndTime:
        endTime           = lineParts.at(5).split( ":" ).first().toUInt();
        hitChart[column] << qMakePair( startTime, endTime );
    }
    qint32 earliestDelay = setStartDelay();
    if ( earliestDelay == -1 ) {
        emit autoplayer_err( "[0x7 : A column is empty]" ); return true;
    }
    quint32 totalObjects = 0, totalCircles = 0;
    QVector<quint32> totalTime_lst( nKey, 0 );

    // Set up delay and hold times :: - -                                              - -
    for ( quint32 i = 0; i < nKey; ++i )
    {
        n = hitChart.at(i).size() - 1;
        for ( quint32 j = 0; j < n; ++j )
        {
            if ( hitChart.at(i).at(j).second == 0 )
            {   // Hit circle :: - -                                                   - -
                holdTime = 5; totalCircles++;
            } else 
            {   // Slider :: - -                                                       - -
                holdTime = hitChart.at(i).at(j).second - hitChart.at(i).at(j).first;
            }
            delayTime = hitChart.at(i).at(j + 1).first - hitChart.at(i).at(j).first - holdTime;
            totalTime_lst[i] += holdTime + delayTime;

            // Overlapping notes in a column :: - -                                    - -
            if ( hitChart.at(i).at(j + 1).first < ( hitChart.at(i).at(j).first + holdTime ) )
            {
                emit autoplayer_err(
                    QString( "[0x8 : %1 %2]" ).arg( i, hitChart.at(i).at(j + 1).first )
                );
                return true;
            }
            hitChart[i][j].first  = holdTime;
            hitChart[i][j].second = delayTime;
        }
        // Last note in column :: - -                                                  - -
        if ( hitChart.at(i).at(n).second == 0 )
        {   // Hit circle :: - -                                                       - -
            holdTime = 5; totalCircles++;
        } else
        {   // Slider :: - -                                                           - -
            holdTime = hitChart.at(i).at(n).second - hitChart.at(i).at(n).first;
        }
        hitChart[i][n].first  = holdTime;
        hitChart[i][n].second = 0;
        totalObjects += hitChart.at(i).size();
    }

    const qreal minutes_real =
        ( *std::max_element(
                totalTime_lst.begin(), totalTime_lst.end()
           ) + earliestDelay
        ) / 60000.0;

    quint16 minutes = static_cast<quint16> ( minutes_real );
    quint8  seconds = static_cast<quint8>( ( minutes_real - minutes ) * 60 );
    if ( seconds == 60 ) { minutes++; seconds = 0; }

    emit sendObjectData(
        QString( "%1 Objects" ).arg( totalObjects ),
        QString( "%1 Circles" ).arg( totalCircles ),
        QString( "%1 Sliders" ).arg( totalObjects - totalCircles ),
        QString( "%1:%2" )
            .arg( minutes, ( minutes > 9 ? 2 : 1 ), 10, QChar( '0' ) )
            .arg( seconds, 2, 10, QChar( '0' ) )
    );
    return false;
}

// Find earliest column note in hitChart and set delay in other columns :: - -         - -
// ret bool, true = err, false = good :: - -                                           - -
qint32 AutoPlayer::setStartDelay()
{
    quint32 earliest = UINT32_MAX;

    // Find the earliest time value among the first notes in each column :: - -        - -
    for ( quint8 i = 0; i < nKey; ++i ) {
        if ( hitChart.at(i).isEmpty() ) { return -1; }
        if ( hitChart.at(i).at(0).first < earliest ) {
            earliest = hitChart.at(i).at(0).first;
        }
    }
    // Adjust the time values of the first notes in each column :: - -                 - -
    for ( quint8 i = 0; i < nKey; ++i ) {
        hitChart[nKey] << qMakePair( hitChart.at(i).at(0).first - earliest, 0 );
    }
    return earliest;
}   // :: - -                                                                          - -
