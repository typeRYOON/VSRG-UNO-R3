/* osumem.cpp */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: osumem.cpp :: VSRG-UNO-R3: Osu Memory Checker Object
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> This file's class object handles memory checks via gosumemory.
 >> Sends out emissions when there is a new beatmap / difficulty.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#include "osumem.h"
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>
#include <QDir>

// Constructor :: - -                                                                  - -
OsuMem::OsuMem() {}

// On beatmap button click, open link :: - -                                           - -
void OsuMem::openBeatmapLink()
{
    QDesktopServices::openUrl(
        QUrl( QString( "https://osu.ppy.sh/beatmapsets/%1" ).arg( osuInfo->at(8) ) )
    );
}

// OsuMem runner function :: - -                                                       - -
void OsuMem::runner()
{
    osuInfo   = new QStringList( 10, QString() );
    webSocket = new QWebSocket();
    connect(
        webSocket, &QWebSocket::connected,
        this,      &OsuMem    ::socketConnect );
    connect(
        webSocket, &QWebSocket::errorOccurred, this, [&](){
            emit memosu_err( QString( "[0xC : %1]" ).arg( webSocket->errorString() ) );
    } );
    webSocket->open( QUrl( "ws://localhost:24010/ws" ) );
}

// Set up signal connections on initial socket connection :: - -                       - -
void OsuMem::socketConnect()
{
    static quint8 iter = 0;
    emit onConnect_ConsoleBox_SIG( "[0x0 : All is well]" );
    connect( webSocket, &QWebSocket::textMessageReceived, this, [&]( const QString& msg )
    {
        QJsonDocument jsonResponse = QJsonDocument::fromJson( msg.toUtf8() );
        QJsonObject   jsonObject   = jsonResponse.object();
        QJsonObject   menu         = jsonObject.value( "menu" ).toObject();

        // First run :: - -                                                            - -
        if ( osuInfo->at(0).isEmpty() )
        {
            if ( !jsonObject.contains( "menu" ) ) {
                emit memosu_err( "[0xD : Websocket sent weird JSON]" );
                return;
            }
            QJsonObject bm      = menu.value( "bm" ).toObject();
            QJsonObject set     = jsonObject.value( "settings" ).toObject();
            QJsonObject stats   = bm.value( "stats" ).toObject();
            QJsonObject folders = set.value( "folders" ).toObject();
            osuInfo->replace( 0, folders.value( "game" ).toString() );
            readOsu( osuInfo->at(0), stats.value( "memoryCS" ).toInt() - 4 );
        }
        // Don't update during gameplay :: - -                                         - -
        if ( menu.value( "state" ).toInt() == 2 ) {
            if ( !inPlay ) {
                emit playChange( true );
                inPlay = true;
            }
            return;
        }
        else {
            if ( inPlay ) {
                emit playChange( false );
                inPlay = false;
            }
        }
        // Beatmap link :: - -                                                         - -
        QJsonObject bm = menu.value( "bm" ).toObject();
        if ( bm.contains( "set" ) && bm.contains( "id" ) ) {
            osuInfo->replace(
                8,
                QString( "%1#mania/%2" )
                    .arg( bm.value( "set" ).toInt() )
                    .arg( bm.value( "id" ).toInt() )
            );
        }
        // Skip the first packet ( reading star rating memory is slow ) :: - -         - -
        // Only update when we have a new map / difficulty :: - -                      - -
        if ( prev != osuInfo->at(8) )
        {   // 2: New beatmap, second packet, -> actual update emission:
            if ( iter == 1 ) {
                prev = osuInfo->at(8);
                iter = 0;
            }
            // 1: Skip first packet on new beatmap:
            else { iter++; return; }
        }
        // 3: Same beatmap reset:
        else { return; }

        // Update osu info string list :: - -                                          - -
        QJsonObject stats = bm.value( "stats" ).toObject();
        qreal       SR    = stats.value( "SR" ).toDouble();

        osuInfo->replace( 1, QString::number( SR, 'f', ( SR > 9 ? 1 : 2 ) ) );
        osuInfo->replace( 4, QString::number( stats.value( "memoryCS" ).toInt() ) );

        QJsonObject meta = bm.value( "metadata" ).toObject();
        osuInfo->replace( 2, meta.value( "titleOriginal" ).toString() );
        osuInfo->replace( 3, meta.value( "difficulty" ).toString() );
        osuInfo->replace( 9, meta.value( "mapper" ).toString() );

        QJsonObject paths = bm.value( "path" ).toObject();
        osuInfo->replace( 5, paths.value( "folder" ).toString() );
        osuInfo->replace( 6, paths.value( "file" ).toString() );
        osuInfo->replace( 7, paths.value( "bg" ).toString() );

        // Construct background image path :: - -                                      - -
        QString bgPath;
        if ( !osuInfo->at(7).isEmpty() ) {
            bgPath = QString( "%1\\Songs\\%2\\%3" )
                     .arg( osuInfo->at(0), osuInfo->at(5), osuInfo->at(7) );
        } else {
            bgPath = "res\\noBG.png";
        }

        emit imgbox_SIG(
            bgPath,
            QString( "%1。" ).arg( osuInfo->at(2)) ,
            QString( "「%1」" ).arg( osuInfo->at(3) ),
            QString( "%1K" ).arg( osuInfo->at(4) )
        );
        emit keyChange(
            osuInfo->at(4).toUInt(),
            QString( "%1\\%2" ).arg( osuInfo->at(5), osuInfo->at(6) ),
            bgPath,
            osuInfo->at(2),
            osuInfo->at(3)
        );
        emit consolebox_SIG(
            bgPath,
            osuInfo->at(2),
            osuInfo->at(3),
            QString( "%1 Stars" ).arg( osuInfo->at(1) ),
            osuInfo->at(9)
        );
        emit resetPaging( 0 );
    });
}

// Read Osu! config file :: - -                                                        - -
void OsuMem::readOsu( const QString& osuPath, const qint8& curK )
{
    QDir osuDir( osuPath );
    osuDir.setNameFilters( QStringList( "osu!*cfg" ) );
    osuDir.setFilter( QDir::Files );

    QVector<QString> vk_s( 4 );
    QString line;

    for ( const QString& file : osuDir.entryList() )
    {
        QFile configFile( osuDir.absoluteFilePath( file ) );
        if ( !configFile.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
            emit memosu_err( "[0xE: Unable to open CFG file]" );
        }

        QTextStream in( &configFile );
        while ( !in.atEnd() )
        {
            line = in.readLine();
            if      ( line.startsWith( "ManiaLayouts4K" ) ) {
                vk_s[0] = line.mid( 17 );
            }
            else if ( line.startsWith( "ManiaLayouts5K" ) ) {
                vk_s[1] = line.mid( 17 );
            }
            else if ( line.startsWith( "ManiaLayouts6K" ) ) {
                vk_s[2] = line.mid( 17 );
            }
            else if ( line.startsWith( "ManiaLayouts7K" ) ) {
                vk_s[3] = line.mid( 17 );
            }
        }
    }
    // Check if key strings acquired :: - -                                            - -
    for ( quint8 i = 0; i < 4; ++i )
    {
        if ( vk_s.at(i).isEmpty() ) {
            emit memosu_err( QString( "[0xF: Set your %1K keys]" ).arg( i + 4 ) );
            return;
        }
    }
    const QVector<QVector<quint8>> vk = makeKeyVector( vk_s );
    if ( !vk.empty() ) { emit sendOsuConfigs( osuPath, vk, curK ); }
}

// Create vector of virtual keys :: - -                                                - -
const QVector<QVector<quint8>> OsuMem::makeKeyVector( const QVector<QString>& vk )
{
    // Set up virtual key mappings :: - -                                              - -
    QHash<QString, quint8> vkMap;
    for (char c = 'A'; c <= 'Z'; ++c) {
        QString character(c);
        vkMap[character] = static_cast<quint8>( c );
    }
    quint8 val = 0x48; // 0
    for ( char c = '0'; c <= '9'; ++c ) {
        QString character( c );
        vkMap[character] = val++;
    }
    vkMap["OemCloseBrackets"] = 0xDD;
    vkMap["OemOpenBrackets"]  = 0xDB;
    vkMap["OemSemicolon"]     = 0xBA;
    vkMap["OemQuestion"]      = 0xBF;
    vkMap["OemQuotes"]        = 0xDE;
    vkMap["OemPeriod"]        = 0xBE;
    vkMap["OemComma"]         = 0xBC;
    vkMap["OemMinus"]         = 0X6D;
    vkMap["OemPipe"]          = 0xDC;
    vkMap["OemPlus"]          = 0x6B;
    vkMap["Space"]            = 0x20;

    // Push back :: - -                                                                - -
    QVector<QVector<quint8>> ret( 4 );
    for ( quint8 i = 0; i < 4; ++i )
    {
        foreach ( const QString& vk_s, vk.at(i).split( ' ', Qt::SkipEmptyParts ) ) {
            ret[i] << vkMap.value( vk_s );
        }
        if ( ret.at(i).size() != i + 4 ) {
            emit memosu_err( QString( "[0xF: Set your %1K keys]" ).arg( i ) );
            return {};
        }
    }
    return ret;
}   // :: - -                                                                          - -
