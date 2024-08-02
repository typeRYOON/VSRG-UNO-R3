/* osuhandler.cpp */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: osuhandler.cpp :: VSRG-UNO-R3: Configuration Handling Object
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> This file's class object is to read configBox signals and change the configuration
 >> for the next time the user presses 'H'. Instantiates needed objects and threads.
 >> When the user presses 'H' it starts the configuration, when 'J' is pressed, 
 >> terminate. Handles error signals emitted by child objects and routes them to 
 >> the configBox object.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#include "autoplayer.h"
#include "osuhandler.h"
#include "keyreader.h"
#include <windows.h>
#include <QThread>

// OsuHandler constructor, mostly set up dummy graph for pixmap creation :: - -        - -
OsuHandler::OsuHandler( Utils* u, QObject* p ) : u( u )
{
    plot = new QCustomPlot();
    plot->setFixedSize( QSize( 250, 46 ) );

    // Axis removal :: - -                                                             - -
    plot->xAxis->setLabelPadding( 0 );
    plot->axisRect()->setMinimumMargins( QMargins( 0, 0, 0, 0 ) );
    plot->axisRect()->setMargins( QMargins( 0, 0, 0, 0 ) );
    plot->axisRect()->setBackground( Qt::transparent );
    plot->xAxis->grid()->setPen( QPen(Qt::NoPen ) );
    plot->yAxis->grid()->setPen( QPen(Qt::NoPen ) );
    plot->xAxis->setTickLabels( false );
    plot->yAxis->setTickLabels( false );
    plot->xAxis->setVisible( false );
    plot->yAxis->setVisible( false );
    plot->xAxis->setTicks( false );
    plot->yAxis->setTicks( false );

    // Set transparency :: - -                                                         - -
    plot->setAttribute( Qt::WA_NoSystemBackground, false );
    plot->setAttribute( Qt::WA_TransparentForMouseEvents );
    plot->setAttribute( Qt::WA_TranslucentBackground );
    plot->setBackground( Qt::transparent );
    plot->setInteractions( QCP::iNone );

    // Initial graph setup :: - -                                                      - -
    plot->xAxis->setRange( 0, 249 );
    graph = plot->addGraph();
    for ( quint8 i = 0; i < 250; ++i ) { xData << i; }

    // Pen line gradient :: - -                                                        - -
    QPen pen;
    QLinearGradient lineGradient( 0, 0, 0, plot->height() );
    lineGradient.setColorAt( 0, QColor( 184, 235, 255 ) );
    lineGradient.setColorAt( 1, QColor( 255, 255, 255 ) );
    pen.setBrush( lineGradient );
    pen.setWidth( 2 );
    graph->setPen(pen);

    // Fill gradient :: - -                                                            - -
    QLinearGradient fillGradient( 0, 0, 0, plot->height() );
    fillGradient.setColorAt( 0, QColor( 77, 116, 136, 150 ) );
    fillGradient.setColorAt( 1, QColor( 77, 116, 136, 0 ) );
    graph->setBrush( fillGradient );
}

// Initial Osu Configuration Settings Load :: - -                                      - -
void OsuHandler::setOsuConfig(
    const QString& osuPath, const QVector<QVector<quint8>>& vk, const quint8& curK )
{
    this->osuPath = osuPath;
    this->curK    = curK;
    this->vk      = vk;
}

// Configuration change via config buttons :: - -                                      - -
void OsuHandler::getButtonState( const quint8& config ) { buttonBMap = config; }


// Arduino port change :: - -                                                          - -
void OsuHandler::portChange( const quint8& i, const QS& portName ) {
    qDebug() << i << portName;
    if ( i == 0 ) { this->port1 = portName; }
    else          { this->port2 = portName; }
}

// New beatmap file path / key count of said map :: - -                                - -
void OsuHandler::keyChange(
    const quint8 & curK,
    const QString& beatmapDir,
    const QString& bgPath,
    const QString& title,
    const QString& diff)
{
    this->beatmapDir = beatmapDir;
    this->curK       = curK - 4;
    this->bgPath     = bgPath;
    this->title      = title;
    this->diff       = diff;
}

// Playfield change :: - -                                                             - -
void OsuHandler::playChange( const bool& inPlay )
{
    this->inPlay = inPlay;

    // Exit out of menu/enter results while in config = turn off config :: - -         - -
    if ( !inPlay && inConfig ) { inConfig = false; }

    // In config but hasn't been sent yet (H outside of playfield) :: - -              - -
    if ( inConfig && !sent ) {
        emit playChange_SIG( inPlay );
        sent = true;
    }
}

// Runner function for OsuHandler :: - -                                               - -
void OsuHandler::runner()
{
    QThread   * keyreader_t  = nullptr;
    KeyReader * keyreader    = nullptr;

    QThread   * autoplayer_t = nullptr;
    AutoPlayer* autoplayer   = nullptr;

    // Osu data must load in before running any configurations :: - -                  - -
    while ( vk.empty() || title == "-+_" ) { QThread::sleep( 1 ); }

    // Run until program termination :: - -                                            - -
    forever
    {   // H Key Hang Loop :: - -                                                      - -
        while ( !( GetAsyncKeyState( 'H' ) & 0x8000 ) ) { QThread::msleep( 40 ); }

        // Empty Configuration, do not run :: - -                                      - -
        if ( buttonBMap == 0 ) {
            emit osuhandler_err( "[0x9 : Empty config]" );
            QThread::sleep( 2 ); continue;
        }
        // Arduino Port Check :: - -                                                   - -
        if ( ( buttonBMap & 0x18 ) && port1 == "-1" ) {
            emit osuhandler_err( "[0xA : Bad KPS Arduino port]" );
            QThread::sleep( 2 ); continue;
        }
        if ( ( buttonBMap & 0x20 ) && port2 == "-1" ) {
            emit osuhandler_err( "[0xB : Bad KS Arduino port]" );
            QThread::sleep( 2 ); continue;
        }
        if ( curK > 3 ) { // 0-4: 4-7K, 0-3K: 253->255, 8k+: 5+
            emit osuhandler_err( "[0x2 : Invalid key count]" );
            QThread::sleep( 2 ); continue;
        }

        emit consoleBox_SIG( true );
        emit osuhandler_run();
        inConfig = true;

        // Computer/Arduino Key-Strokes / KPS (0001 1011 = 0x1B) :: - -                - -
        if ( buttonBMap & 0x1B )
        {   // KPS graph on, user in playfield, graph visible SIG not sent :: - -      - -
            if ( buttonBMap & 0x02 && inPlay && !sent ) {
                emit playChange_SIG( inPlay );   sent = true;
            }
            keyreader_t = new QThread();
            keyreader   = new KeyReader(
                vk.at( curK ), buttonBMap, curK, port1
            );
            keyreader->moveToThread( keyreader_t );
            connect(
                keyreader_t, &QThread   ::started,
                keyreader,   &KeyReader ::runner );
            connect(
                this,        &OsuHandler::stop_SIG,
                keyreader,   &KeyReader ::stop_SIG,      Qt::DirectConnection );
            connect(
                keyreader,   &KeyReader ::keys,
                this,        &OsuHandler::logKPS,        Qt::DirectConnection );
            connect(
                keyreader,   &KeyReader ::keyreader_err,
                this,        &OsuHandler::errorRouter,   Qt::DirectConnection );
            keyreader_t->start();
        }

        // Computer/Arduino Autoplayer (0010 0100 = 0x24) :: - -                       - -
        if ( buttonBMap & 0x24 )
        {
            autoplayer_t = new QThread();
            autoplayer   = new AutoPlayer(
                vk.at( curK ),
                QString( "%1\\Songs\\%2" ).arg( osuPath, beatmapDir ),
                port2
            );
            autoplayer->moveToThread( autoplayer_t );
            if ( buttonBMap & 0x04 ) {
                connect(
                    autoplayer_t, &QThread   ::started,
                    autoplayer,   &AutoPlayer::computerAuto );
            } else {
                connect(
                    autoplayer_t, &QThread   ::started,
                    autoplayer,   &AutoPlayer::arduinoAuto );
            }
            connect(
                this,             &OsuHandler::stop_SIG,
                autoplayer,       &AutoPlayer::stop_SIG,       Qt::DirectConnection );
            connect(
                autoplayer,       &AutoPlayer::sendObjectData,
                this,             &OsuHandler::sendObjectData, Qt::DirectConnection );
            connect(
                autoplayer,       &AutoPlayer::autoplayer_err,
                this,             &OsuHandler::errorRouter,    Qt::DirectConnection );
            connect(
                autoplayer,       &AutoPlayer::delay,
                this,             &OsuHandler::delay,          Qt::DirectConnection );
            autoplayer_t->start();
        }

        // J Key Hang Loop :: - -                                                      - -
        while ( !( GetAsyncKeyState( 'J' ) & 0x8000 ) && inConfig ) {
            QThread::msleep( 100 );
        }
        emit consoleBox_SIG( false );
        emit stop_SIG();
        inConfig = false;

        if ( keyreader_t )
        {   // Make KPS graph invisible :: - -                                         - -
            if ( sent ) {
                emit playChange_SIG( false );
                sent = false;
            }
            keyreader_t->quit(); keyreader_t->wait();
            delete keyreader; delete keyreader_t;
            keyreader_t = nullptr;

            addHistoryNode();
        }
        if ( autoplayer_t )
        {
            autoplayer_t->quit(); autoplayer_t->wait();
            delete autoplayer; delete autoplayer_t;
            autoplayer_t = nullptr;
        }
    }
}

// Send off error string to configBox and terminate configaration early :: - -         - -
void OsuHandler::errorRouter( const QString& error_s )
{
    emit osuhandler_err( error_s );
    inConfig = false;
}

// Store in internal KPS vector and send to imageBox :: - -                            - -
void OsuHandler::logKPS( const quint8& KPS, const quint8& KS )
{
    emit keys( KPS, KS );
    KPS_lst << KPS + 2 ;
}

// Create graph pixmap and send it to historyBox :: - -                                - -
void OsuHandler::addHistoryNode()
{   // Reduce data points :: - -                                                       - -
    downSample( KPS_lst, 250 );

    // Get max KPS value in graph and set range :: - -                                 - -
    plot->yAxis->setRange(
        0, *std::max_element( KPS_lst.begin(), KPS_lst.end() )
    );
    graph->setData( xData, KPS_lst );
    plot->replot();

    emit sendHistoryNode(
        plot->toPixmap( 250, 46 ),
        u->roundScaleImage( bgPath, 24, 92, 92, 1, 0 ),
        qMakePair( QString( "%1。" ).arg( title ), QString( " : : %1" ).arg( diff ) ),
        curK + 4
    );
    KPS_lst.clear();
}

// Reduce graph nodes to a suitable size :: - -                                        - -
void OsuHandler::downSample( QVector<double>& data, const quint16& target )
{
    quint32 n = data.size();
    if ( target >= n )
    {   // Pad the rest to reach target length :: - -                                  - -
        n = target - n;
        for ( quint8 i = 0; i < n; ++i ) { data << 2; }
        return;
    }
    // Downsample the QVector to fit target length :: - -                              - -
    QVector<double> ret;
    const double factor = static_cast<double>( n ) / target;
    for ( quint32 i = 0; i < target; ++i ) {
        ret << data.at( static_cast<quint32>( i * factor ) );
    }
    data = ret;
}   // :: - -                                                                          - -
