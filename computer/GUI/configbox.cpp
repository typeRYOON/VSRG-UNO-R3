/* configbox.cpp */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: configbox.cpp :: VSRG-UNO-R3: ConfigBox QWidget
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> This file's class object is a QWidget that handles the user setting some
 >> configurations. There is a paging system that is slotted from a signal from the
 >> ImgBox object. One page one, the user clicks boxes to set the OsuHandler
 >> run configuration. On page two, the user can set the Arduino ports if they want to
 >> run an Arduino OsuHandler configuration. On page three, the authors and versioning.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#include "configbox.h"
#include <QSerialPortInfo>

// Constructor :: - -                                                                  - -
ConfigBox::ConfigBox( Utils* u, QObject* p ) : u( u )
{
    setFixedSize( QSize( 390, 200 ) );

    // Put main layout together :: - -                                                 - -
    stack = new QSL( this );
    stack->addWidget( makeConfigLayer()  );
    stack->addWidget( makeArduinoLayer() );
    stack->addWidget( makeCreditsLayer() );
}

// Change stack to ith widget :: - -                                                   - -
void ConfigBox::changeStack( const quint8& i ) { stack->setCurrentIndex( i ); }

// Config layer QFrame :: - -                                                          - -
QF* ConfigBox::makeConfigLayer()
{
    QF* ret = new QF( this );
    ret->setStyleSheet(
        "QFrame { background-color: transparent; border: 0px solid transparent; }"
    );
    QHL* icon_hl = new QHL();
    icon_hl->setAlignment( Qt::AlignLeft );
    icon_hl->setSpacing( 16 );
    icon_hl->setContentsMargins( 4, 0, 0, 0 );

    QStringList icons_lst = { "res/arduino.png", "res/computer.png" };
    foreach ( const QS& path, icons_lst ) {
        QL* icon = new QL();
        icon->setStyleSheet( "background-color: transparent; border: 0px" );
        icon->setFixedSize( 30, 30 );
        icon->setPixmap( QPixmap( path ) );
        icon->setGraphicsEffect(
            u->makeDropShadow( QColor( 255, 255, 255, 150 ), icon )
        );
        icon_hl->addWidget( icon );
    }
    // Create push buttons :: - -                                                      - -
    QVector<QPB*> buttons;
    QVL* col2_vl = new QVL();
    col2_vl->setAlignment( Qt::AlignTop );
    for ( quint8 i = 0; i < 3; ++i ) {
        buttons << makeConfigButton();
        col2_vl->addWidget( buttons.at(i) );
    }
    QVL* col1_vl = new QVL();
    col1_vl->setAlignment( Qt::AlignTop );
    for ( quint8 i = 0; i < 3; ++i ) {
        buttons << makeConfigButton();
        col1_vl->addWidget( buttons.at(i + 3) );
    }
    makeButtonConnections( buttons );

    // Text to the right of the buttons :: - -                                         - -
    QVL* text_vl = new QVL();
    text_vl->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    text_vl->setContentsMargins( 0, 7, 0, 0 );
    text_vl->setSpacing( 22 );

    QFont config( u->getFont( 13 ) );
    QStringList text_lst = { "Keystrokes。", "Keys per Second。", "Autoplayer。" };
    foreach ( const QS& str, text_lst ) {
        QL* label = new QL( str );
        label->setStyleSheet( "background-color: transparent; border: 0px" );
        label->setFont( config );
        label->setGraphicsEffect(
            u->makeDropShadow( QColor( 255, 255, 255, 150 ), label )
        );
        text_vl->addWidget( label );
    }
    // Put layouts together :: - -                                                     - -
    QVL* main_vl = new QVL( ret );
    main_vl->setAlignment( Qt::AlignBottom | Qt::AlignLeft );
    main_vl->setContentsMargins( 20, 0, 0, 20 );
    main_vl->addLayout( icon_hl );

    QHL* main_hl = new QHL();
    main_hl->setAlignment( Qt::AlignLeft );
    main_hl->addLayout( col1_vl );
    main_hl->addLayout( col2_vl );
    main_hl->addSpacing( 4 );
    main_hl->addWidget(
        u->makeVLine( QSize( 2, 124 ), QColor( 255, 255, 255, 150 ), this )
    );
    main_hl->addLayout( text_vl );
    main_vl->addLayout( main_hl );
    return ret;
}

// Make the six configuration buttons :: - -                                           - -
QPB* ConfigBox::makeConfigButton()
{
    QPB* ret = new QPB();
    ret->setStyleSheet(
        "QPushButton { border: 2px solid white;"
        "border-radius: 10px; background-color: rgba( 0, 0, 0, 0 ); }"
        "QPushButton:hover { background-color: rgba( 0, 0, 0, 0.2 ); }"
        "QPushButton:pressed { background-color: rgba( 0, 0, 0, 0.4 ); }"
    );
    ret->setCursor( Qt::PointingHandCursor );
    ret->setFixedSize( 38, 38 );
    ret->setIconSize( QSize( 24, 24 ) );
    ret->setGraphicsEffect(
        u->makeDropShadow( QColor( 255, 255, 255, 150 ), ret )
    );
    return ret;
}

// Make signal/slot connections for unique inputs :: - -                               - -
void ConfigBox::makeButtonConnections( const QVector<QPB*>& b )
{   // Computer KS :: - -                                                              - -
    connect( b.at(0), &QPB::clicked, this, [=](){
        handleClick( b.at(0), 0x01 );
    } );

    // Computer KPS :: - -                                                             - -
    connect( b.at(1), &QPB::clicked, this, [=](){
        handleClick( b.at(1), 0x02 );
    } );

    // Computer AP :: - -                                                              - -
    connect( b.at(2), &QPB::clicked, this, [=](){
        handleClick( b.at(2), 0x04, b.at(5) );
    } );

    // Arduino KS :: - -                                                               - -
    connect( b.at(3), &QPB::clicked, this, [=](){
        handleClick( b.at(3), 0x08 );
    } );

    // Arduino KPS :: - -                                                              - -
    connect( b.at(4), &QPB::clicked, this, [=](){
        handleClick( b.at(4), 0x10 );
    } );

    // Arduino AP :: - -                                                               - -
    connect( b.at(5), &QPB::clicked, this, [=](){
        handleClick( b.at(5), 0x20, b.at(2) );
    } );
}

// Change internal config state and emit it off :: - -                                 - -
void ConfigBox::handleClick( QPB* clicked, const quint8& sent, QPB* related )
{
    if ( config & sent ) { config -= sent; clicked->setIcon( QIcon() ); }
    else                 { config += sent; clicked->setIcon( QIcon( "res/check.png" ) ); }
    if ( sent == 0x04 && config & 0x20 ) {
        config -= 0x20; related->setIcon( QIcon() );
    }
    if ( sent == 0x20 && config & 0x04 ) {
        config -= 0x04; related->setIcon( QIcon() );
    }
    emit newConfig( config );
}

// Arduino layer QFrame :: - -                                                         - -
QF* ConfigBox::makeArduinoLayer()
{
    QF* ret = new QF( this );
    ret->setStyleSheet(
        "QFrame { background-color: transparent; border: 0px solid transparent; }"
    );
    QList<QL*> labels;
    QStringList text = { "Arduino", "KPS : : -1", "KS   : : -1" };

    // Set up text QLabels :: - -                                                      - -
    foreach ( const QS& str, text ) {
        labels << new QL( str );
        labels.last()->setGraphicsEffect(
            u->makeDropShadow( QColor( 255, 255, 255 ), this )
        );
        labels.last()->setStyleSheet(
            "background-color: transparent; border: 0px; color: white;"
        );
        labels.last()->setFixedHeight( 20 );
    }
    kps_ql = labels.at(1);
    ks_ql  = labels.at(2);
    const QFont arduinoSml( u->getFont( 13 ) );
    labels.at(0)->setFixedHeight( 26 );
    labels.at(0)->setFont( u->getFont( 18 ) );
    kps_ql->setFont( arduinoSml );
    ks_ql ->setFont( arduinoSml );

    // Lambda widget generators :: - -                                                 - -
    auto makeLineEdit = [&]()
    {
        QLineEdit* ret = new QLineEdit( "-" );
        ret->setFixedHeight( 24 );
        ret->setFont( arduinoSml );
        ret->setFrame( false );
        ret->setGraphicsEffect(
            u->makeDropShadow( QColor(255, 255, 255, 150), ret )
        );
        ret->setStyleSheet(
            "QLineEdit { background-color: transparent;"
            "color: rgba( 255, 255, 255, 150 );"
            "border: 0px solid transparent; }"
        );
        return ret;
    };
    auto makeButton = [&]( const quint8& i, const QLineEdit* qle )
    {
        QPB* ret = new QPB();
        ret->setFixedSize( 50, 50 );
        ret->setIconSize( QSize( 32, 32 ) );
        ret->setCursor( Qt::PointingHandCursor );
        ret->setIcon( QIcon( "res/refresh.png" ) );
        ret->setStyleSheet(
            "QPushButton { background-color: #161616;"
            "border: 0px solid transparent;"
            "border-radius: 12px; }"
        );
        connect( ret, &QPB::clicked, this, [=](){
            evalLineEdit( i, qle->text() );
        } );
        return ret;
    };

    // Put Layouts together :: - -                                                     - -
    QLineEdit* r1 = makeLineEdit();
    QLineEdit* r2 = makeLineEdit();

    QHL* arduinoR1_hl = new QHL();
    arduinoR1_hl->setAlignment( Qt::AlignLeft );
    arduinoR1_hl->addWidget( makeButton( 0, r1 ) );
    arduinoR1_hl->addWidget(
        u->makeVLine( QSize( 2, 44 ), QColor( 255, 255, 255 ), this )
    );
    QVL* arduinoR1_vl = new QVL();
    arduinoR1_vl->setSpacing( 2 );
    arduinoR1_vl->addWidget( kps_ql );
    arduinoR1_vl->addWidget( r1 );
    arduinoR1_hl->addLayout( arduinoR1_vl );

    QHL* arduinoR2_hl = new QHL();
    arduinoR2_hl->setAlignment( Qt::AlignLeft );
    arduinoR2_hl->addWidget( makeButton( 1, r2 ) );
    arduinoR2_hl->addWidget(
        u->makeVLine( QSize( 2, 44 ), QColor( 255, 255, 255 ), this )
    );
    QVL* arduinoR2_vl = new QVL();
    arduinoR2_vl->addWidget( ks_ql );
    arduinoR2_vl->addWidget( r2 );
    arduinoR2_hl->addLayout( arduinoR2_vl );

    QVL* main_vl = new QVL( ret );
    main_vl->setSpacing( 6 );
    main_vl->setAlignment( Qt::AlignTop );
    main_vl->setContentsMargins( 20, 20, 20, 30 );

    main_vl->addWidget( labels.at(0) );
    main_vl->addLayout( arduinoR1_hl );
    main_vl->addLayout( arduinoR2_hl );
    return ret;
}

// Evaluate user's entered port name and check against active Arduino marked ports ::  - -
void ConfigBox::evalLineEdit( const quint8& i, const QS& portCheck )
{   // An empty string would be "contained" within all potential port names :: - -     - -
    if ( portCheck.isEmpty() ) { return; }
    QS foundPort = "-1";

    foreach ( const QSerialPortInfo& pInfo, QSerialPortInfo::availablePorts() )
    {
        if ( !pInfo.description().contains( "Arduino" )) { continue; }
        if ( pInfo.portName().contains( portCheck, Qt::CaseInsensitive ) ) {
            foundPort = pInfo.portName();
            break;
        }
    }
    // Change msg and QLabel based on i parameter :: - -                               - -
    QS pf; QL* ql;
    if ( i == 0 ) { pf = "KPS : : " ; ql = kps_ql; }
    else          { pf = "KS   : : "; ql = ks_ql ; }
    // No port found, return msg :: - -                                                - -
    if ( foundPort == "-1" ) {
        ql->setText( QString( "%1Not found。" ).arg( pf ) );
        return;
    }
    // Port found, emit to OsuHandler to change its internal port name :: - -          - -
    ql->setText( QString( "%1%2。" ).arg( pf, foundPort ) );
    emit portChange( i, foundPort );
}

// Credits layer QFrame :: - -                                                         - -
QF* ConfigBox::makeCreditsLayer()
{
    QF* ret = new QF( this );
    ret->setStyleSheet(
        "QFrame { background-color: transparent; border: 0px solid transparent; }"
    );

    // LEFT :: - -                                                                     - -
    QList<QL*> labels;
    QStringList text = {
        "Authors", "Versions",                           // Large  font, white [0-1]
        "Ryan Magdaleno", "John See",                    // Normal font, white [2-3]
        "Ryoon", "ChessZra", "v2.00", "v6.72", "v1.3.9", // Normal font, grey  [4-8]
        "", "", ""                                       // Image labels       [9-11]
    };
    // Creation :: - -                                                                 - -
    foreach ( const QS& str, text ) { labels << new QL( str ); }
    const QFont creditsLrg( u->getFont( 18 ) ), creditsSml( u->getFont( 13 ) );

    // Large font, white :: - -                                                        - -
    for (  quint8 i = 0; i < 2; ++i )
    {
        labels.at(i)->setFont( creditsLrg );
        labels.at(i)->setStyleSheet(
            "background-color: transparent; border: 0px; color: white;"
        );
        labels.at(i)->setFixedHeight( 28 );
        labels.at(i)->setGraphicsEffect(
            u->makeDropShadow( QColor( 255, 255, 255 ), labels.at(i) )
        );
    }
    // Normal font, white :: - -                                                       - -
    for ( quint8 i = 2; i < 4; ++i )
    {
        labels.at(i)->setFont( creditsSml );
        labels.at(i)->setStyleSheet(
            "background-color: transparent; border: 0px; color: white;"
        );
        labels.at(i)->setGraphicsEffect(
            u->makeDropShadow( QColor( 255, 255, 255 ), labels.at(i) )
        );
    }
    // Normal font, grey :: - -                                                        - -
    for ( quint8 i = 4; i < 9; ++i )
    {
        labels.at(i)->setFont( creditsSml );
        labels.at(i)->setStyleSheet(
            "background-color: transparent; border: 0px;"
            "color: rgba( 255, 255, 255, 150 );"
        );
        labels.at(i)->setGraphicsEffect(
            u->makeDropShadow( QColor( 255, 255, 255, 150 ), labels.at(i) )
        );
    }
    // Manual tweaks :: - -                                                            - -
    labels.at(1)->setAlignment( Qt::AlignRight );
    labels.at(6)->setAlignment( Qt::AlignRight );
    labels.at(7)->setAlignment( Qt::AlignRight );
    labels.at(8)->setAlignment( Qt::AlignRight );

    // RIGHT :: - -                                                                    - -
    QStringList iconPath = {
        "res/version-vsrg.png", "res/version-qt.png", "res/version-gosu.png"
    };
    for ( quint8 i = 9; i < 12; ++i ) {
        labels.at(i)->setFixedSize( 26, 26 );
        labels.at(i)->setPixmap( QPixmap( iconPath.at(i - 9) ) );
        labels.at(i)->setStyleSheet(
            "background-color: transparent; border: 0px;"
        );
    }

    // Put layouts together :: - -                                                     - -
    // Top row :: - -                                                                  - -
    QHL* titleText_hl = new QHL();
    titleText_hl->addWidget( labels.at(0) );
    titleText_hl->addWidget( labels.at(1) );

    // Left side :: - -                                                                - -
    // Row 1 :: - -                                                                    - -
    QHL* authorsR1_hl = new QHL();
    authorsR1_hl->setSpacing( 6 );
    authorsR1_hl->addWidget(
        u->makeVLine( QSize( 2, 44 ), QColor( 255, 255, 255 ), this )
    );
    QVL* authorsR1_vl = new QVL();
    authorsR1_vl->setSpacing( 0 );
    authorsR1_vl->addWidget( labels.at(2) );
    authorsR1_vl->addWidget( labels.at(4) );
    authorsR1_hl->addLayout( authorsR1_vl );

    // Row 2 :: - -                                                                    - -
    QHL* authorsR2_hl = new QHL();
    authorsR2_hl->setSpacing( 6 );
    authorsR2_hl->addWidget(
        u->makeVLine( QSize( 2, 44 ), QColor( 255, 255, 255 ), this )
    );
    QVL* authorsR2_vl = new QVL();
    authorsR2_vl->setSpacing( 0 );
    authorsR2_vl->addWidget( labels.at(3) );
    authorsR2_vl->addWidget( labels.at(5) );
    authorsR2_hl->addLayout( authorsR2_vl );

    QVL* left_vl = new QVL();
    left_vl->addLayout( authorsR1_hl );
    left_vl->addSpacing( 4 );
    left_vl->addLayout( authorsR2_hl );

    // Right side :: - -                                                               - -
    // Row 1 :: - -                                                                    - -
    QHL* versionsR1_hl = new QHL();
    versionsR1_hl->setAlignment( Qt::AlignBottom );
    versionsR1_hl->addWidget( labels.at(6) );
    versionsR1_hl->addSpacing( 3 );
    versionsR1_hl->addWidget( labels.at(9) );

    QHL* versionsR2_hl = new QHL();
    versionsR2_hl->setAlignment( Qt::AlignBottom );
    versionsR2_hl->addWidget( labels.at(7) );
    versionsR2_hl->addSpacing( 3 );
    versionsR2_hl->addWidget( labels.at(10) );

    QHL* versionsR3_hl = new QHL();
    versionsR3_hl->setAlignment( Qt::AlignBottom );
    versionsR3_hl->addWidget( labels.at(8) );
    versionsR3_hl->addSpacing( 3 );
    versionsR3_hl->addWidget( labels.at(11) );

    QVL* versions_vl = new QVL();
    versions_vl->setAlignment( Qt::AlignRight );
    versions_vl->setSpacing( 2 );
    versions_vl->addLayout( versionsR1_hl );
    versions_vl->addLayout( versionsR2_hl );
    versions_vl->addLayout( versionsR3_hl );

    QHL* right_hl = new QHL();
    right_hl->addLayout( versions_vl );
    right_hl->addWidget(
        u->makeVLine( QSize( 2, 92 ), QColor( 255, 255, 255 ), this )
    );

    QHL* leftright_hl = new QHL();
    leftright_hl->addLayout( left_vl );
    leftright_hl->addLayout( right_hl );

    QVL* main_vl = new QVL( ret );
    main_vl->setAlignment( Qt::AlignTop );
    main_vl->setContentsMargins( 20, 20, 20, 30 );
    main_vl->addLayout( titleText_hl );
    main_vl->addLayout( leftright_hl );
    main_vl->setSpacing( 6 );
    return ret;
}   // :: - -                                                                          - -
