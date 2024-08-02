/* autoplayerkey.cpp */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: autoplayerkey.cpp :: VSRG-UNO-R3: Column Presser Object
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> This file's class object is to be instantiated by the AutoPlayer object. The
 >> AutoPlayer object will send a QVector of pair hit timings via the constructor, where
 >> .first is hold time and .second is delay time (time in ms to delay "this" thread).
 >> This object will read the hit timing vector and press the constructor parameter
 >> supplied virtual key code.
 >>
 >> The AutoPlayer object may also send a signal to this object to shift the timings of
 >> this column object +- the shifted amount. The default shift amount is 3ms.
 - -                                                                                   - -
 >> https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#include "autoplayerkey.h"
#include <windows.h>
#include <chrono>
#include <thread>

// Initial data set :: - -                                                             - -
AutoPlayerKey::AutoPlayerKey(
    const QVector<QPair<quint32, quint32>>& hitVector,
    const quint32& startDelay,
    const quint8 & key,
    QObject* p
) : hitVector( hitVector ), startDelay( startDelay ), key( key ), QObject{ p } {}

// Terminate this thread gracefully :: - -                                             - -
void AutoPlayerKey::stop_SIG() { cont = false; }

// Shift timings +- shift :: - -                                                       - -
void AutoPlayerKey::delay( const qint16& shift ) { this->shift += shift; }

// Key object runner function :: - -                                                   - -
void AutoPlayerKey::runner()
{
    keybd_event( key, 0, KEYEVENTF_KEYUP, 0 );
    auto startTime = std::chrono::high_resolution_clock::now();
    auto expected  = startTime + std::chrono::milliseconds( startDelay );
    if ( startDelay ) { std::this_thread::sleep_until( expected ); }

    startTime = expected;
    for ( const auto& pair : hitVector )
    {
        if ( !cont ) { break; }
        keybd_event( key, 0, 0, 0 );

        std::this_thread::sleep_for( std::chrono::milliseconds( pair.first ) );
        keybd_event( key, 0, KEYEVENTF_KEYUP, 0 );

        expected = startTime + std::chrono::milliseconds( pair.second + pair.first + shift );
        startTime = expected, shift = 0;
        std::this_thread::sleep_until( expected );
    }
}   // :: - -                                                                          - -
