/* imgbox.cpp */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: imgbox.cpp :: VSRG-UNO-R3: ImgBox QWidget
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> This file's class object is the top image box that shows data about the currently
 >> in view beatmap in the game osu. There are extra details during play like the KPS
 >> box and graph, a key strokes bitmap, paging system selector, map title/difficulty,
 >> source code link, and the minimize/exit buttons.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#include "imgbox.h"

// Constructor :: - -                                                                  - -
ImgBox::ImgBox( Utils* u, QWidget* parent ) : u( u ), QF{ parent }
{
    cipherTimer = new QTimer( this );
    connect( cipherTimer, &QTimer::timeout, this, &ImgBox::cipherText );
    setFixedSize( QSize( 831, 284 ) );
    setObjectName( "ImgBox" );
    setStyleSheet(
        "#ImgBox {"
        "background-color: transparent;"
        "border-bottom-left-radius: 0px;"
        "border-bottom-right-radius: 0px;"
        "border-width: 2px; }"
    );

    // Background image pixmaps :: - -                                                 - -
    beatmapBG_A = new QL();
    beatmapBG_A->setAlignment( Qt::AlignCenter );
    beatmapBG_A->setStyleSheet( "border: 0px; background-color: transparent;" );
    beatmapBG_A->setFixedSize( 825, 279 );
    beatmapBG_A->setScaledContents( true );
    beatmapBG_A->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    beatmapBG_A->setPixmap(
        u->roundScaleImage( "res/noBG.png", 16, 825, 273, 1, 1 )
    );

    beatmapBG_B = new QL();
    beatmapBG_B->setAlignment( Qt::AlignCenter );
    beatmapBG_B->setStyleSheet( "border: 0px; background-color: transparent;" );
    beatmapBG_B->setFixedSize( 825, 279 );
    beatmapBG_B->setScaledContents( true );
    beatmapBG_B->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    QO_E* e = new QO_E( beatmapBG_B );
    beatmapBG_B->setGraphicsEffect( e );
    e->setOpacity( 0 );

    out_a = new QPA( e, "opacity" );
    out_a->setDuration( 500 );
    out_a->setStartValue( 1 );
    out_a->setEndValue( 0 );
    out_a->setEasingCurve( QEasingCurve::InOutQuad );

    in_a  = new QPA( e, "opacity" );
    in_a->setDuration( 500 );
    in_a->setStartValue( 0 );
    in_a->setEndValue( 1 );
    in_a->setEasingCurve( QEasingCurve::InOutQuad );

    // Title button :: - -                                                             - -
    QPB* title_pb = new QPB();
    title_pb->setCursor( Qt::PointingHandCursor );
    title_pb->setFixedSize( QSize( 140, 34 ) );
    title_pb->setIcon( QIcon( "res/title.png" ) );
    title_pb->setIconSize( QSize( 140, 34 ) );
    title_pb->setStyleSheet(
        "background-color: transparent; border: 0px solid transparent;"
    );
    connect( title_pb, &QPB::clicked, this, [this]() {
        QDesktopServices::openUrl( QUrl( "https://github.com/typeRYOON/VSRG-UNO-R3" ) );
    } );
    QDS_E* drop_e = new QDS_E( title_pb );
    drop_e->setColor( QColor( 0, 0, 0, 170 ) );
    drop_e->setBlurRadius( 20 );
    drop_e->setOffset( 0, 0 );
    title_pb->setGraphicsEffect( drop_e );

    // Title bar buttons :: - -                                                        - -
    QPB* minus_pb = new QPB();
    minus_pb->setCursor( Qt::PointingHandCursor );
    minus_pb->setFixedSize( QSize( 26, 26 ) );
    minus_pb->setStyleSheet(
        "QPushButton { background-color: transparent; border: 0px; }"
    );
    minus_pb->setIcon( QIcon( "res/minus.png" ) );
    minus_pb->setIconSize( QSize( 16, 16 ) );
    connect( minus_pb, &QPB::clicked, this, [&](){ emit minus(); } );

    QPB* exit_pb = new QPB();
    exit_pb->setCursor( Qt::PointingHandCursor );
    exit_pb->setFixedSize( QSize( 16, 16 ) );
    exit_pb->setStyleSheet(
        "QPushButton { background-color: transparent;"
        "border: 0px; }"
    );
    exit_pb ->setIcon( QIcon("res/exit.png" ) );
    exit_pb->setIconSize( QSize( 16, 16 ) );
    connect( exit_pb, &QPB::clicked, this, [&](){ emit exit(); } );

    // Map Title, Diff, and Selection box :: - -                                       - -
    QGL* bottomLeft = new QGL();

    mapTitle = new QL("VSRG-UNO-R3。");
    subTitle = new QL("「Build v2.0」");
    keyFrame = new QL("0K");
    QL* dash = new QL("-");

    mapTitle->setFont( u->getFont( 22 ) );
    mapTitle->setContentsMargins( 14, 0, 0, 74 );
    mapTitle->setStyleSheet(
        "background-color: transparent;"
        "border: 0px;"
        "color: white"
    );
    dash->setFont( u->getFont( 16 ) );
    dash->setContentsMargins( 62, 0, 0, 54 );
    dash->setStyleSheet(
        "background-color: transparent;"
        "border: 0px;"
        "color: rgba( 255, 255, 255, 170 )"
    );
    subTitle->setFont( u->getFont( 16 ) );
    subTitle->setContentsMargins( 74, 0, 0, 54 );
    subTitle->setStyleSheet(
        "background-color: transparent;"
        "border: 0px;"
        "color: rgba( 255, 255, 255, 170 )"
    );

    keyFrame->setFont( u->getFont( 13 ) );
    keyFrame->setContentsMargins( 3, 2, 3, 2 );
    keyFrame->setStyleSheet(
        "background-color: rgba( 0, 0, 0, 150 );"
        "border: 0px;"
        "color: white;"
        "border-radius: 6px;"
    );

    mapTitle->setGraphicsEffect(
        u->makeDropShadow( QColor( 255, 255, 255, 255), this )
    );
    subTitle->setGraphicsEffect(
        u->makeDropShadow( QColor( 255, 255, 255, 170 ), this )
    );
    dash->setGraphicsEffect(
        u->makeDropShadow( QColor( 255, 255, 255, 170 ), this )
    );
    keyFrame->setGraphicsEffect(
        u->makeDropShadow( QColor( 0, 0, 0, 255 ), this )
    );

    QVL* keyFrame_vl = new QVL();
    keyFrame_vl->addWidget( keyFrame);
    keyFrame_vl->setContentsMargins( 22, 0, 0, 53 );

    // Select box :: - -                                                               - -
    QF* selectBox = new QF();
    selectBox->setFixedSize( QSize( 120, 40 ) );
    selectBox->setStyleSheet(
        "QFrame { background-color: rgba( 255, 255, 255, 100 );"
        "border: 0px;"
        "border-radius: 10px; }"
    );

    QVL* selectBox_vl = new QVL();
    selectBox_vl->addWidget( selectBox );
    selectBox_vl->setContentsMargins( 16, 0, 0, 9 );

    selected = new QF();
    selected->setFixedSize( 34, 34 );
    selected->setStyleSheet(
        "QFrame { background-color: rgba( 115, 115, 115, 100 );"
        "border: 2px solid #c8c8c8;"
        "border-radius: 8px; }"
    );
    selected->setGraphicsEffect(
        u->makeDropShadow( QColor( 255, 255, 255, 255 ), this )
    );

    QHL* selected_hl = new QHL();
    selected_hl->setContentsMargins( 19, 0, 0, 12 );
    selected_hl->addWidget( selected );

    scroll_a = new QPA( selected, "geometry" );
    scroll_a->setDuration( 300 );
    scroll_a->setEasingCurve( QEasingCurve::InOutQuad );

    // Button creation lambda :: - -                                                   - -
    auto makeSelectButton = [&]( const QS& path )
    {
        QPB* ret = new QPB();
        ret->setCursor( Qt::PointingHandCursor );
        ret->setStyleSheet(
            "background-color: transparent; border: 0px;"
        );
        ret->setIconSize( QSize( 22, 22 ) );
        ret->setFixedSize( QSize(26, 26) );
        ret->setIcon( QPixmap( path ) );
        ret->setGraphicsEffect(
            u->makeDropShadow( QColor( 255, 255, 255 ), this )
        );
        return ret;
    };
    selectMania        = makeSelectButton( "res/select-mania.png"   );
    QPB* selectArduino = makeSelectButton( "res/select-arduino.png" );
    QPB* selectCredits = makeSelectButton( "res/select-credits.png" );

    exit_pb->setGraphicsEffect(
        u->makeDropShadow( QColor( 0, 0, 0, 200 ), this, 20 )
    );
    minus_pb->setGraphicsEffect(
        u->makeDropShadow( QColor( 0, 0, 0, 200 ), this, 20 )
    );

    pressedButton = selectMania;
    buttonPos.insert( selectMania,  0 );
    buttonPos.insert( selectArduino, 1 );
    buttonPos.insert( selectCredits, 2 );

    connect( selectMania, &QPB::clicked, this, [=](){
        emit changeStack( 0 );
        buttonPress( selectMania );
    } );
    connect( selectArduino, &QPB::clicked, this, [=](){
        emit changeStack( 1 );
        buttonPress( selectArduino );
    } );
    connect( selectCredits, &QPB::clicked, this, [=](){
        emit changeStack( 2 );
        buttonPress( selectCredits );
    } );
    origin = QRect( 22, 236, 34, 34 );

    QHL* buttonLayout = new QHL();
    buttonLayout->setAlignment( Qt::AlignLeft );
    buttonLayout->setContentsMargins( 23, 0, 0, 16 );
    buttonLayout->addWidget( selectMania );
    buttonLayout->addSpacing( 7 );
    buttonLayout->addWidget( selectArduino );
    buttonLayout->addSpacing( 7 );
    buttonLayout->addWidget( selectCredits );

    // KPS and Keystrokes :: - -                                                       - -
    QVL* bottomRight = new QVL();
    bottomRight->setSpacing( 0 );
    bottomRight->setAlignment( Qt::AlignBottom );
    keystrokesBox = new QL();
    kpsBox        = new QL();

    keystrokesBox->setFixedSize( 28, 120 );
    keystrokesBox->setStyleSheet(
        "QLabel { background-color: transparent;"
        "border: 0px; }"
    );
    QHL* ks_hl = new QHL();
    ks_hl->addWidget( keystrokesBox );
    keystrokesBox->setGraphicsEffect(
        u->makeDropShadow( QColor( 255, 255, 255, 255 ), this )
    );

    kpsBox->setAlignment( Qt::AlignCenter );
    kpsBox->setFont( u->getFont( 10 ) );
    kpsBox->setFixedSize( 40, 40 );
    kpsBox->setGraphicsEffect(
        u->makeDropShadow( QColor( 0, 0, 0, 140 ), this )
    );
    kpsBox->setStyleSheet(
        "QLabel { background-color: rgba( 20, 22, 25, 100 );"
        "border: 2px solid white;"
        "border-radius: 10px; }"
    );

    bottomRight->addStretch();
    bottomRight->addLayout( ks_hl );
    bottomRight->addWidget( kpsBox );
    bottomRight->setContentsMargins( 0, 0, 16, 9 );

    QGL* selectBox_gl = new QGL();
    selectBox_gl->addLayout(selectBox_vl, 0, 0, 0, 0, Qt::AlignLeft | Qt::AlignBottom);
    selectBox_gl->addLayout(selected_hl,  0, 0, 0, 0, Qt::AlignLeft | Qt::AlignBottom);
    selectBox_gl->addLayout(buttonLayout, 0, 0, 0, 0, Qt::AlignLeft | Qt::AlignBottom);

    bottomLeft->addWidget(subTitle,       0, 0, 0, 0, Qt::AlignLeft | Qt::AlignBottom);
    bottomLeft->addWidget(dash,           0, 0, 0, 0, Qt::AlignLeft | Qt::AlignBottom);
    bottomLeft->addWidget(mapTitle,       0, 0, 0, 0, Qt::AlignLeft | Qt::AlignBottom);
    bottomLeft->addLayout(keyFrame_vl,    0, 0, 0, 0, Qt::AlignLeft | Qt::AlignBottom);
    bottomLeft->addLayout(selectBox_gl,   0, 0, 0, 0, Qt::AlignLeft | Qt::AlignBottom);

    // Central layout of ImgBox object :: - -                                          - -
    QGL* imgLayout = new QGL( this );
    imgLayout->setContentsMargins( 1, 1, 0, 0 );
    imgLayout->addWidget( beatmapBG_A, 0, 0, 0, 0, Qt::AlignTop );
    imgLayout->addWidget( beatmapBG_B, 0, 0, 0, 0, Qt::AlignTop );

    // Set up graph and place onto layout :: - -                                       - -
    makeGraph( imgLayout );

    QVL* title_vl = new QVL();
    title_vl->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    title_vl->setContentsMargins( 10, 12, 0, 0 );
    title_vl->addWidget( title_pb );
    imgLayout->addLayout( title_vl, 0, 0, 0, 0, Qt::AlignLeft  | Qt::AlignTop );

    QHL* titleButtons = new QHL();
    titleButtons->setContentsMargins(0, 14, 20, 0 );
    titleButtons->setAlignment( Qt::AlignRight );
    titleButtons->addWidget( minus_pb );
    titleButtons->addSpacing( 6 );
    titleButtons->addWidget( exit_pb );

    imgLayout->addLayout( titleButtons, 0, 0, 0, 0, Qt::AlignRight | Qt::AlignTop );
    imgLayout->addLayout( bottomLeft,   0, 0, 0, 0, Qt::AlignLeft  | Qt::AlignBottom );
    imgLayout->addLayout( bottomRight,  0, 0, 0, 0, Qt::AlignRight | Qt::AlignBottom );
}

// On button press, set pressedButton to selectMania :: - -                            - -
void ImgBox::resetPaging( const quint8& _ ) { pressedButton = selectMania; }

// On beatmap change, animate next pixmaps, cipher text :: - -                         - -
void ImgBox::changeData(
    const QS& imgPath,
    const QS& mapTarget_s,
    const QS& subTarget_s,
    const QS& key )
{
    if ( BisVisible )
    {
        beatmapBG_A->setPixmap( u->roundScaleImage( imgPath, 16, 825, 273, 1, 1 ) );
        out_a->start();
    } else
    {
        beatmapBG_B->setPixmap( u->roundScaleImage( imgPath, 16, 825, 273, 1, 1 ) );
        in_a->start();
    }
    BisVisible = !BisVisible;

    // Cipher Text :: - -                                                              - -
    mapCipherLen = mapTitle_s.length();
    subCipherLen = subTitle_s.length();
    mpDel_d      = mapTitle_s.length()  / 3;
    mpDel_u      = mapTarget_s.length() / 3;
    sbDel_d      = subTitle_s.length()  / 3;
    sbDel_u      = subTarget_s.length() / 3;
    step         = 0;

    if ( mapTitle_s != mapTarget_s )
    {
        cipherMap         = true;
        mapTitle_s        = mapTarget_s;
        mapTitle_target_s = mapTarget_s;
    }
    if ( subTitle_s != subTarget_s )
    {
        cipherSub         = true;
        subTitle_s        = subTarget_s;
        subTitle_target_s = subTarget_s;
    }
    this->key = key;
    cipherTimer->start( 40 );
}

// Recieve KPS/KS, update bitmap and graph :: - -                                      - -
void ImgBox::updateData( const quint8& KPS, const quint8& KS )
{
    static quint8 updateIter = 0;
    if ( ++updateIter == 8 )
    {   // Pop front :: - -                                                            - -
        yData.removeFirst();

        // Trail stop dummy data :: - -                                                - -
        if ( KS == 128 ) {
            yData << 2;
            kpsBox->setText( "" );
        } else {
            yData << KPS + 2;
            kpsBox->setText( QString::number( KPS ) );
        }
        graph->setData( xData, yData );
        plot->replot();
        updateIter = 0;
    }
    // Shift values up :: - -
    for ( uint8_t x = 0; x < 28; ++x ) {
        for ( uint8_t y = 0; y < 128; y += 2 ) {
            bitmap[x][y + 1] = bitmap[x][y + 2];
            bitmap[x][y]     = bitmap[x][y + 1];
        }
    }
    // Add new Keystroke values :: - -                                                 - -
    uint8_t x = 0, stopX;
    for ( uint16_t i = 1; i <= 64; i <<= 1 )
    {
        stopX = x + 4;
        for ( ; x < stopX; ++x )
        {
            bitmap[x][127] = KS & i;
            bitmap[x][126] = KS & i;
        }
    }
    // Create pixmap :: - -                                                            - -
    QImage image( 28, 128, QImage::Format_ARGB32 );
    for ( uint8_t x = 0; x < 28; ++x )
    {
        for ( uint32_t y = 0; y < 128; ++y )
        {   // Shift opacity by adding a growing value based on y value :: - -         - -
            image.setPixel(x , y, bitmap[x][y] ? 0x00FFFFFF + ( y << 25 ) : 0 );
        }
    }
    keystrokesBox->setPixmap( QPixmap::fromImage( image ) );
}

// Shift opacity of the graph :: - -                                                   - -
void ImgBox::opacityGraph( const bool& inPlay )
{
    if ( plotOpacity_a->state() == QAbstractAnimation::Running ) {
        plotOpacity_a->stop();
    }
    plotOpacity_a->setStartValue( !inPlay );
    plotOpacity_a->setEndValue( inPlay );
    plotOpacity_a->start();
}

// Set osuHandlerRun to true :: - -                                                    - -
void ImgBox::osuHandlerSet() { osuHandlerRun = true; }

// Generate random cipher text of cipher length :: - -                                 - -
void ImgBox::cipherText()
{   // Base case :: - -                                                                - -
    if ( step == 7 ) {
        mapTitle->setText( mapTitle_target_s );
        subTitle->setText( subTitle_target_s );
        cipherTimer->stop();
        cipherMap = false, cipherSub = false;
        return;
    }

    // Cipher only when different string :: - -                                        - -
    if ( cipherMap )
    {
        QS mapCipher;
        for ( quint8 i = 0; i < mapCipherLen; ++i ) {
            mapCipher += QChar( QRandomGenerator::global()->bounded( 33, 127 ) );
        }
        mapTitle->setText( mapCipher );
    }
    if ( cipherSub )
    {
        QS subCipher = "  ";
        for ( quint8 i = 0; i < subCipherLen; ++i ) {
            subCipher += QChar(QRandomGenerator::global()->bounded( 33, 127 ) );
        }
        subTitle->setText( subCipher );
    }

    // Decrease/Increase Cipher Length :: - -                                          - -
    if ( step < 3 )  { mapCipherLen -= mpDel_d; subCipherLen -= sbDel_d; }
    else             { mapCipherLen += mpDel_u; subCipherLen += sbDel_u; }
    if ( step++ == 3 ) { keyFrame->setText( key ); }
}

// On button press, move select box to correct spot :: - -                             - -
void ImgBox::buttonPress( QPB* pressed )
{
    if ( pressed == pressedButton ) { return; }
    pressedButton = pressed;

    QRect startRect = selected->geometry();
    QRect endRect   = origin.translated( 39 * buttonPos.value( pressed ), 0 );

    scroll_a->setStartValue( startRect );
    scroll_a->setEndValue( endRect );
    scroll_a->start();
}

// On termination, send dummy data during fade out :: - -                              - -
void ImgBox::trailStop()
{
    osuHandlerRun = false;
    QTimer* trailTimer = new QTimer();
    connect( trailTimer, &QTimer::timeout, this, [&, trailTimer]()
    {
        static quint8 iter = 0;
        updateData( 0, 128 );

        if ( osuHandlerRun ) {
            osuHandlerRun = false;
            trailTimer->stop();

        }
        if ( ++iter == 70 ) {
            osuHandlerRun = false;
            trailTimer->stop();
            resetData();
            iter = 0;
        }
    } );
    trailTimer->start( 20 );
}

// Reset internal bitmap and graph :: - -                                              - -
void ImgBox::resetData()
{
    for ( quint8 x = 0; x < 28; ++x ) {
        for ( quint8 y = 0; y < 128; ++y ) {
            bitmap[x][y] = 0;
        }
    }
    for ( quint8 i = 0; i < 30; ++i ) {
        yData[i] = 2;
    }
    graph->setData( xData, yData );
    plot->replot();
}

// Create the imgbox graph :: - -                                                      - -
void ImgBox::makeGraph( QGL* layout )
{
    plot = new QCustomPlot();
    plot->setFixedSize( QSize( 831, 280 ) );

    // Set styling :: - -                                                              - -
    plot->axisRect()->setMinimumMargins( QMargins( 0, 0, 0, 0 ) );
    plot->axisRect()->setMargins( QMargins( 0, 0, 0, 0 ) );
    plot->setAttribute( Qt::WA_NoSystemBackground, false );
    plot->setAttribute( Qt::WA_TransparentForMouseEvents );
    plot->setAttribute( Qt::WA_TranslucentBackground );
    plot->axisRect()->setBackground( Qt::transparent );
    plot->xAxis->grid()->setPen( QPen( Qt::NoPen ) );
    plot->yAxis->grid()->setPen( QPen( Qt::NoPen ) );
    plot->setBackground( Qt::transparent );
    plot->setInteractions( QCP::iNone );
    plot->xAxis->setTickLabels( false );
    plot->yAxis->setTickLabels( false );
    plot->xAxis->setLabelPadding( 0 );
    plot->xAxis->setVisible( false );
    plot->yAxis->setVisible( false );
    plot->xAxis->setTicks( false );
    plot->yAxis->setTicks( false );
    plot->xAxis->setRange( 0, 29 );
    plot->yAxis->setRange( 0, 80 );

    // Empty graph :: - -                                                              - -
    graph = plot->addGraph();
    for ( quint8 i = 0; i < 30; ++i ) {
        xData << i;
        yData << 2;
    }
    graph->setData( xData, yData );

    // Pen, line :: - -                                                                - -
    QPen pen;
    pen.setColor( QColor( 255, 255, 255 ) );
    pen.setWidth( 2 );
    graph->setPen( pen );

    // Fill gradient :: - -                                                            - -
    QLinearGradient fillGradient( 0, 0, 0, plot->height() );
    fillGradient.setColorAt( 0, QColor(255, 255, 255, 170 ) );
    fillGradient.setColorAt( 1, QColor(0,   255, 255, 0   ) );
    graph->setBrush( fillGradient );

    QHL* graph_hl = new QHL();
    graph_hl->addWidget( plot );
    graph_hl->addStretch();

    QO_E* graphOpacity_e = new QO_E( plot );
    plot->setGraphicsEffect( graphOpacity_e );

    plotOpacity_a = new QPA( graphOpacity_e, "opacity", this );
    plotOpacity_a->setEasingCurve( QEasingCurve::InOutQuad );
    plotOpacity_a->setDuration( 0 );
    plotOpacity_a->setEndValue( 0 );
    plotOpacity_a->start();
    plotOpacity_a->setDuration( 500 );

    resetData();
    layout->addLayout( graph_hl, 0, 0, 0, 0, Qt::AlignLeft | Qt::AlignBottom );
    plot->show();
}   // :: - -                                                                          - -
