/* consolebox.cpp */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: consolebox.cpp :: VSRG-UNO-R3: ConsoleBox QWidget
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> This file's class object is a QWidget that displays stats about the current beatmap,
 >> if the AutoPlayer object is ran, extra details about the beatmap hit objects are
 >> sent to this object.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#include "consolebox.h"
#include <QDragEnterEvent>
#include <QMimeData>

// Constructor :: - -                                                                  - -
ConsoleBox::ConsoleBox( Utils* u, QObject* p ) : u( u )
{   // Set outline frame :: - -                                                        - -
    setFixedSize( QSize( 390, 393 ) );
    setAcceptDrops( true );

    QF* consoleFrame = new QF();
    consoleFrame->setFixedSize( 356, 360 );
    consoleFrame->setStyleSheet(
        "QFrame { border: 2px solid white;"
        "border-radius: 16px; }"
    );
    // Inner frame layout :: - -                                                       - -
    // Row one :: - -                                                                  - -
    bgBox = new QL();
    bgBox->setAlignment( Qt::AlignCenter );
    bgBox->setFixedSize( 64, 64 );
    bgBox->setStyleSheet(
        "QLabel { border: 2px solid white;"
        "border-radius: 16px; background-color: transparent; }"
    );
    bgBox->setGraphicsEffect(
        u->makeDropShadow( QColor( 255, 255, 255 ), this )
    );
    bgBox->setPixmap(
        u->roundScaleImage( "res/noBG.png", 20, 64, 64, 1, 0 ) 
    );

    levelTitle_ql = new QL( "VSRG-UNO-R3 v2.0" );
    levelTitle_ql->setFixedSize( 263, 26 );
    levelTitle_ql->setFont( u->getFont( 16 ) );
    levelTitle_ql->setGraphicsEffect(
        u->makeDropShadow( QColor( 255, 255, 255 ), this )
    );
    levelTitle_ql->setStyleSheet(
        "QLabel { border: 0px solid transparent;"
        "background-color: transparent; color: white; }"
    );

    levelSub_ql = new QL( "by Ryan and John" );
    levelSub_ql->setContentsMargins( 0, 0, 0, 8 );
    levelSub_ql->setFixedSize( 263, 30 );
    levelSub_ql->setFont( u->getFont( 13 ) );
    levelSub_ql->setGraphicsEffect(
        u->makeDropShadow( QColor( 255, 255, 255 ), this )
    );
    levelSub_ql->setStyleSheet(
        "QLabel { color: rgba( 255, 255, 255, 150 );"
        "border: 0px solid transparent;"
        "background-color: transparent;  }"
    );

    // Row two :: - -                                                                  - -
    const QFont text_qf = u->getFont( 12 );
    auto makeTextLabel = [&](
        const QS&            text_s,
        const QSize&         size,
        const Qt::Alignment& alignment )
    {
        QL* ret = new QL( text_s );
        ret->setAlignment( alignment | Qt::AlignVCenter );
        ret->setFixedSize( size );
        ret->setFont( text_qf );
        ret->setStyleSheet(
            "QLabel { border: 0px solid transparent;"
            "background-color: transparent; }"
        );
        ret->setGraphicsEffect(
            u->makeDropShadow( QColor( 255, 255, 255, 200 ), this )
        );
        return ret;
    };

    constexpr QSize row2_a_qs( 130, 20 ); // 158
    constexpr QSize row2_b_qs( 174, 20 );
    objects_ql = makeTextLabel( "0 Objects",  row2_a_qs, Qt::AlignRight );
    circles_ql = makeTextLabel( "0 Circles",  row2_a_qs, Qt::AlignRight );
    sliders_ql = makeTextLabel( "0 Sliders",  row2_a_qs, Qt::AlignRight );
    stars_ql   = makeTextLabel( "0.00 Stars", row2_b_qs, Qt::AlignLeft );
    creator_ql = makeTextLabel( "peppy",      row2_b_qs, Qt::AlignLeft );
    length_ql  = makeTextLabel( "0:00",       row2_b_qs, Qt::AlignLeft );

    // Row three :: - -                                                                - -
    constexpr QSize row3_qs( 320, 20 );
    constexpr QSize row4_qs( 330, 20 );
    QL* runConfig_ql = makeTextLabel(
        "[H]         : Run Configuration", row3_qs, Qt::AlignLeft );
    QL* termRun_ql   = makeTextLabel(
        "[J]          : Terminate Run",    row3_qs, Qt::AlignLeft );
    QL* onTop_ql     = makeTextLabel(
        "[Alt + T] : Always on Top",       row3_qs, Qt::AlignLeft );

    // Row four :: - -                                                                 - -
    errorCode_ql = makeTextLabel(
        "[0x1 : Osu! mem check]", row4_qs, Qt::AlignLeft );
    QL* inPlay_ql    = makeTextLabel(
        "[In-play]",             row4_qs, Qt::AlignLeft );
    inPlay_ql->setStyleSheet(
        "QLabel { border: 0px solid transparent;"
        "background-color: transparent; color: green; }"
    );
    QO_E* inPlay_oe = new QO_E( inPlay_ql );
    inPlay_oe->setOpacity( 0.2 );
    inPlay_ql->setGraphicsEffect( inPlay_oe );

    inPlay_a = new QPA( inPlay_oe, "opacity" );
    inPlay_a->setEasingCurve( QEasingCurve::InCubic );
    inPlay_a->setDuration( 250 );
    inPlay_a->setStartValue( 0.2 );

    QL* term_ql      = makeTextLabel(
        "[Terminated]", row4_qs,  Qt::AlignLeft );
    term_ql->setStyleSheet(
        "QLabel { border: 0px solid transparent;"
        "background-color: transparent; color: red; }"
    );
    QO_E* term_oe = new QO_E();
    term_oe->setOpacity( 0.2 );
    term_ql->setGraphicsEffect( term_oe );

    term_a = new QPA( term_oe, "opacity" );
    term_a->setEasingCurve( QEasingCurve::InCubic );
    term_a->setStartValue( 0.2 );
    term_a->setDuration( 250 );
    term_a->setEndValue( 1 );

    QPA* termIn_a = new QPA( term_oe, "opacity" );
    termIn_a->setEasingCurve( QEasingCurve::InCubic );
    termIn_a->setStartValue( 1 );
    termIn_a->setEndValue( 0.2 );
    termIn_a->setDuration( 500 );

    // On Termination signal delay fading out :: - -                                   - -
    termTimer = new QTimer( this );
    termTimer->setSingleShot( true );

    connect( termTimer, &QTimer::timeout, this, [=](){
        termIn_a->start();
    } );
    connect( term_a, &QPA::finished, this, [=](){
        termTimer->start( 2500 );
    } );

    // Bongo cat :: - -                                                                - -
    cat_ql = new QL();
    cat_ql->setFixedSize( 144, 70 );
    cat_ql->setPixmap( QPixmap( "res/cat-normal.png" ) );
    cat_ql->setStyleSheet(
        "QLabel { border: 0px transparent;"
        "background-color: transparent; }"
    );
    cat_ql->setGraphicsEffect(
        u->makeDropShadow( QColor( 255, 255, 255 ), this )
    );

    // Put widgets and layouts together :: - -                                         - -
    // Row one :: - -                                                                  - -
    QHL* row1_hl = new QHL();
    row1_hl->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
    row1_hl->setSpacing( 4 );
    row1_hl->addWidget( bgBox );
    row1_hl->addSpacing( 5 );
    row1_hl->addWidget(
        u->makeVLine( QSize( 2, 50 ), QColor( 255, 255, 255 ), this )
    );

    QVL* row1Text_vl = new QVL();
    row1Text_vl->setContentsMargins( 0, 4, 0, 0 );
    row1Text_vl->setSpacing( 0 );
    row1Text_vl->addWidget( levelTitle_ql );
    row1Text_vl->addWidget( levelSub_ql   );
    row1_hl->addLayout( row1Text_vl       );

    // Row two :: - -                                                                  - -
    QHL* row2_hl = new QHL();
    row2_hl->setContentsMargins( 7, 0, 7, 0 );
    row2_hl->addWidget(
        u->makeVLine( QSize( 2, 74 ), QColor( 255, 255, 255 ), this )
    );

    QVL* leftText_vl = new QVL();
    leftText_vl->setContentsMargins( 2, 0, 0, 0 );
    leftText_vl->addWidget( stars_ql   );
    leftText_vl->addWidget( creator_ql );
    leftText_vl->addWidget( length_ql  );
    row2_hl->addLayout( leftText_vl    );

    QVL* rightText_vl = new QVL();
    rightText_vl->setContentsMargins( 0, 0, 2, 0 );
    rightText_vl->addWidget( objects_ql );
    rightText_vl->addWidget( circles_ql );
    rightText_vl->addWidget( sliders_ql );
    row2_hl->addLayout( rightText_vl    );
    row2_hl->addWidget(
        u->makeVLine( QSize( 2, 74 ), QColor( 255, 255, 255 ), this )
    );

    // Row three :: - -                                                                - -
    QHL* row3_hl = new QHL();
    row3_hl->setContentsMargins( 7, 0, 7, 0 );
    row3_hl->addWidget(
        u->makeVLine( QSize( 2, 74 ), QColor( 255, 255, 255 ), this )
    );

    QVL* row3_vl = new QVL();
    row3_vl->setContentsMargins( 2, 0, 0, 0 );
    row3_vl->addWidget( runConfig_ql );
    row3_vl->addWidget( termRun_ql   );
    row3_vl->addWidget( onTop_ql     );
    row3_hl->addLayout( row3_vl      );

    // Row four :: - -                                                                 - -
    QHL* row4_hl = new QHL();
    row4_hl->setContentsMargins( 7, 0, 7, 0 );
    row4_hl->addWidget(
        u->makeVLine( QSize( 2, 68 ), QColor( 255, 255, 255 ), this )
    );

    QVL* row4_vl = new QVL();
    row4_vl->setContentsMargins( 2, 0, 0, 0 );
    row4_vl->setSpacing( 0 );
    row4_vl->addWidget( errorCode_ql );
    row4_vl->addWidget( inPlay_ql    );
    row4_vl->addWidget( term_ql      );
    row4_hl->addLayout( row4_vl      );

    QVL* consoleFrame_vl = new QVL( consoleFrame );
    consoleFrame_vl->setAlignment( Qt::AlignTop );
    consoleFrame_vl->setSpacing( 3 );
    consoleFrame_vl->addLayout( row1_hl );
    consoleFrame_vl->addSpacing( 10 );
    consoleFrame_vl->addLayout( row2_hl );
    consoleFrame_vl->addSpacing( 10 );
    consoleFrame_vl->addLayout( row3_hl );
    consoleFrame_vl->addSpacing( 10 );

    QGL* row4_cat_gl = new QGL();
    row4_cat_gl->addLayout( row4_hl, 0, 0, 0, 0 );

    QHL* cat_hl = new QHL();
    cat_hl->setContentsMargins( 200, 28, 0, 0 );
    cat_hl->addWidget( cat_ql );
    row4_cat_gl->addLayout( cat_hl,  0, 0, 0, 0, Qt::AlignLeft | Qt::AlignBottom );

    consoleFrame_vl->addLayout( row4_cat_gl );

    QVL* consoleBox_vfix = new QVL( this );
    consoleBox_vfix->setAlignment( Qt::AlignCenter );
    consoleBox_vfix->addWidget( consoleFrame );
}

// Change console box details on beatmap change :: - -                                 - -
void ConsoleBox::beatmapChange(
    const QS& beatmapBG,
    const QS& beatmapTitle,
    const QS& beatmapDiff,
    const QS& beatmapStars,
    const QS& beatmapCreator)
{
    this->bgBox->setPixmap( u->roundScaleImage( beatmapBG, 20, 64, 64, 1, 0 ) );
    this->levelTitle_ql->setText( beatmapTitle );
    this->levelSub_ql->setText( beatmapDiff    );
    this->stars_ql->setText( beatmapStars      );
    this->creator_ql->setText( beatmapCreator  );
}

// On autoplayer start, set detailed information on beatmap :: - -                     - -
void ConsoleBox::setObjectData(
    const QS& objects_s,
    const QS& circles_s,
    const QS& sliders_s,
    const QS& length_s)
{
    this->objects_ql->setText( objects_s );
    this->circles_ql->setText( circles_s );
    this->sliders_ql->setText( sliders_s );
    this->length_ql->setText( length_s );
}

// Error code received from somewhere :: - -                                           - -
void ConsoleBox::setErrCode( const QS& errCode_s ) {
    this->errorCode_ql->setText( errCode_s );
}

void ConsoleBox::delay( const qint16& delayed )
{
    if ( delayed == -3 ) {
        cat_ql->setPixmap( QPixmap( "res/cat-left.png" ) );
        errorCode_ql->setText( "[0x0 : All is well]            -3ms" );
    } else {
        errorCode_ql->setText( "[0x0 : All is well]                        +3ms" );
        cat_ql->setPixmap( QPixmap( "res/cat-right.png" ) );
    }
    QTimer::singleShot( 500, this, [&](){
        errorCode_ql->setText( "[0x0 : All is well]" );
        cat_ql->setPixmap( QPixmap( "res/cat-normal.png" ) );
    } );
}

// On OsuHandler configuration start, change play status in ConsoleBox :: - -          - -
void ConsoleBox::inPlayLabel( const bool& inPlayField )
{
    if ( inPlay_a->state() == QAbstractAnimation::Running ) {
        inPlay_a->stop();
    };
    if ( term_a->state() == QAbstractAnimation::Running ) {
        term_a->stop();
    }

    inPlay_a->setStartValue( 1 - 0.8 * inPlayField );
    inPlay_a->setEndValue( 1 - 0.8 * !inPlayField );
    inPlay_a->start();

    // Reset states :: - -                                                             - -
    if ( inPlayField ) { this->errorCode_ql->setText( "[0x0 : All is well]" ); }
    else {
        term_a->start();
        objects_ql->setText( "0 Objects" );
        circles_ql->setText( "0 Circles" );
        sliders_ql->setText( "0 Sliders" );
        length_ql ->setText( "0:00" );
    }
}   // :: - -                                                                          - -
