/* historybox.cpp */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: historybox.cpp :: VSRG-UNO-R3: HistoryBox QWidget
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> This file's class object is a QWidget that shows the user their past played
 >> beatmaps, played as in the user ran the OsuHandler configuration [H].
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#include "historybox.h"
#include <QFile>

// Constructor :: - -                                                                  - -
HistoryBox::HistoryBox( Utils* u, QObject* p ) : u( u )
{   // Load last session's history nodes :: - -                                        - -
    if ( loadHistoryNodes() )
    {   // Bad load, fill with empty elements :: - -                                   - -
        nodes.resize(
            5,
            HistoryNode(
                QPixmap(),
                QPixmap(),
                { "BAD", " : : LOAD" },
                0
            )
        );
    }
    title_qf = u->getFont( 15 );
    sub_qf   = u->getFont( 13 );
    setFixedSize( QSize( 390, 605 ) );

    // Create widgets/layout :: - -                                                    - -
    QVL* vl = new QVL( this );
    vl->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    vl->setContentsMargins( 0, 0, 0, 0 );

    history = new QVL();
    history->setAlignment( Qt::AlignTop );
    history->setContentsMargins( 15, 0, 0, 0 );
    history->setSpacing( 15 );

    // Set up the five nodes :: - -                                                    - -
    for ( qint8 i = 4; i >= 0; --i )
    {
        frames <<
        makeHistoryNode(
            std::get<0>( nodes.at(i) ),
            std::get<1>( nodes.at(i) ),
            std::get<2>( nodes.at(i) ),
            std::get<3>( nodes.at(i) )
        );
        history->addWidget( frames.last() );
    }

    setFrameStyle();
    vl->addLayout( history );
    afterLoad = true;
}

// Create QFrame widget :: - -                                                         - -
QF* HistoryBox::makeHistoryNode(
    const QPixmap&       graph,
    const QPixmap&       bg,
    const QPair<QS, QS>& beatmap_s,
    const quint8 &       keyN )
{   // Add to history vector :: - -                                                    - -
    if ( afterLoad ) {
        nodes.pop_front();
        nodes << std::make_tuple( graph, bg, beatmap_s, keyN );
    }
    // Arrange layouts :: - -                                                          - -
    QF* ret = new QF( this );
    ret->setObjectName( "historyFrame" );
    ret->setFixedSize( 360, 100 );

    QHL* ret_hl = new QHL( ret );
    ret_hl->setContentsMargins( 4, 0, 0, 0 );
    ret_hl->setAlignment( Qt::AlignLeft );
    ret_hl->setSpacing( 4 );

    QL* bgFrame = new QL();
    bgFrame->setAlignment( Qt::AlignCenter );
    bgFrame->setFixedSize( 92, 92 );
    bgFrame->setStyleSheet(
        "QLabel { background-color: transparent; border: 2px solid white; }"
    );
    bgFrame->setPixmap( bg );
    ret_hl->addWidget( bgFrame );
    ret_hl->addSpacing( 4 );

    QF* vline = new QF();
    vline->setFixedSize( 2, 92 );
    vline->setStyleSheet( "background-color: white;" );
    ret_hl->addWidget( vline );

    QVL* rightHalf = new QVL();
    rightHalf->setAlignment( Qt::AlignTop | Qt::AlignLeft );
    rightHalf->setContentsMargins( 0, 4, 0, 0 );
    rightHalf->setSpacing( 0 );

    QGL* text_gl = new QGL();
    text_gl->setAlignment( Qt::AlignTop | Qt::AlignLeft );
    text_gl->setContentsMargins( 0, 0, 0, 23 );

    QL* title = new QL( beatmap_s.first );
    title->setFixedSize ( 250, 23 );
    title->setFont( title_qf );
    title->setStyleSheet(
        "QLabel { background-color: transparent;"
        "border: 0px solid transparent;"
        "border-radius: 0px; }"
    );
    QL* sub = new QL( beatmap_s.second );
    sub->setFixedSize( 250, 23 );
    sub->setFont( sub_qf );
    sub->setStyleSheet(
        "QLabel { background-color: transparent;"
        "border: 0px solid transparent;"
        "border-radius: 0px;"
        "color: rgba(255, 255, 255, 150); }"
    );
    QHL* sub_vl = new QHL();
    sub_vl->setAlignment( Qt::AlignLeft );
    sub_vl->setContentsMargins( 0, 24, 0, 0 );

    QL* keyBox = new QL( QString( "%1K" ).arg( keyN ) );
    keyBox->setAlignment( Qt::AlignCenter );
    keyBox->setFixedSize( 30, 23 );
    keyBox->setStyleSheet( "background-color: black; border: 0px; border-radius: 6px" );
    keyBox->setFont( sub_qf );

    sub_vl->addWidget( keyBox );
    sub_vl->addWidget( sub );

    text_gl->addLayout( sub_vl, 0, 0, 0, 0 );
    text_gl->addWidget( title,  0, 0, 0, 0 );

    QL* kpsGraph = new QL();
    kpsGraph->setFixedSize( 250, 46 );
    kpsGraph->setStyleSheet(
        "QLabel { background-color: transparent;"
        "border: 0px solid transparent;"
        "border-radius: 0px; }"
        );
    kpsGraph->setPixmap( graph );
    rightHalf->addWidget( kpsGraph );
    ret_hl->addLayout( rightHalf );
    rightHalf->addLayout( text_gl );

    bgFrame->setGraphicsEffect(
        u->makeDropShadow( QColor( 255, 255, 255, 155 ), ret )
    );
    title->setGraphicsEffect(
        u->makeDropShadow( QColor( 255, 255, 255, 200 ), ret )
    );
    sub->setGraphicsEffect(
        u->makeDropShadow( QColor( 255, 255, 255, 150 ), ret )
    );
    vline->setGraphicsEffect(
        u->makeDropShadow( QColor( 255, 255, 255, 200 ), ret )
    );
    keyBox->setGraphicsEffect(
        u->makeDropShadow( QColor( 0, 0, 0, 150 ), ret )
    );
    return ret;
}

// Shift layout and insert new frame :: - -                                            - -
void HistoryBox::addHistoryNode(
    const QPixmap&       graph,
    const QPixmap&       bg,
    const QPair<QS, QS>& beatmap_s,
    const quint8 &       kps )
{
    QF* newFrame = makeHistoryNode( graph, bg, beatmap_s, kps );
    deleteFrame();
    frames.prepend( newFrame ) ;
    history->insertWidget( 0, newFrame );
    setFrameStyle();
}

// Set up alternating frame color :: - -                                               - -
void HistoryBox::setFrameStyle()
{
    for ( quint8 i = 0; i < 5; ++i )
    {
        if ( i & 0x1 ) {
            frames.at(i)->setStyleSheet(
                "#historyFrame { background-color: #1b1b1b;"
                "border: 0px solid transparent; }"
            );
        }
        else {
            frames.at(i)->setStyleSheet(
                "#historyFrame { background-color: transparent;"
                "border: 0px solid transparent; }"
            );
        }
    }
}

// Load history.dat into nodes QList :: - -                                            - -
bool HistoryBox::loadHistoryNodes()
{
    QFile file( "res/history.dat" );
    if ( !file.open( QIODevice::ReadOnly ) ) { return true; }

    HistoryNode node;
    QDataStream in( &file );
    for ( quint8 i = 0; i < 5; ++i )
    {
        in >> std::get<0>( node );  // graph    : QPixmap
        in >> std::get<1>( node );  // bg       : QPixmap
        in >> std::get<2>( node );  // beatmap_s: QPair<QString, QString>
        in >> std::get<3>( node );  // keyN     : quint8
        nodes << node;
    }
    return false;
}

// Save nodes QList into history.dat :: - -                                            - -
void HistoryBox::saveHistoryNodes()
{
    QFile file( "res/history.dat" );
    if ( !file.open( QIODevice::WriteOnly ) ) { return; }

    QDataStream out( &file );
    foreach ( const HistoryNode& node, nodes )
    {
        out << std::get<0>( node );  // graph    : QPixmap
        out << std::get<1>( node );  // bg       : QPixmap
        out << std::get<2>( node );  // beatmap_s: QPair<QString, QString>
        out << std::get<3>( node );  // keyN     : quint8
    }
}

// Clear history nodes and replace with 5 empty ones :: - -                            - -
void HistoryBox::clearHistory()
{
    quint8 n = frames.size();
    for ( quint8 i = 0; i < n; ++i ) {
        nodes.removeLast();
        deleteFrame();
    }
    afterLoad = false;
    for ( quint8 i = 0; i < 5; ++i ) {
        HistoryNode node =
        {
            QPixmap( "res/graph.png" ),
            u->roundScaleImage( "res/noBG.png", 26, 92, 92, 1, 0 ),
            { "Title", " : : Difficulty" },
            0
        };
        frames <<
        makeHistoryNode(
            std::get<0>( node ),
            std::get<1>( node ),
            std::get<2>( node ),
            std::get<3>( node )
        );
        history->addWidget( frames.last() );
        nodes << node;
    }
    afterLoad = true;
    setFrameStyle();
}

// Delete the oldest frame to make space for a new frame :: - -                        - -
void HistoryBox::deleteFrame()
{
    QF* remove = frames.last();
    history->removeWidget( remove );
    QLayout* childLayout = remove->layout();
    if ( childLayout )
    {
        QLayoutItem* item;
        while ( ( item = childLayout->takeAt(0) ) != nullptr )
        {
            delete item->widget();
            delete item;
        }
        delete childLayout;
    }
    delete remove;
    frames.removeLast();
}   // :: - -                                                                          - -
