/* mainwindow.cpp */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: mainwindow.cpp :: VSRG-UNO-R3: MainWindow QMainWindow
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> This file's class object is the MainWindow object, instantiates all custom objects,
 >> is the entry point of the program. Sets up signal/slots connections between the
 >> custom objects.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#include "../Utils/hoverframe.h"
#include "mainwindow.h"

// MainWindow Entrypoint :: - -                                                        - -
MainWindow::MainWindow( QWidget* p ) : QMainWindow( p )
{   // Initial thread + utility objects :: - -                                         - -
    utils  = new Utils();

    QThread* osumem_t = new QThread();
    osumem = new OsuMem();

    QThread* osuhandler_t = new QThread();
    osuhandler = new OsuHandler( utils, this );

    // Create all custom GUI QWidgets :: - -                                           - -
    setMainWindow();
    setCentralWidget( createMainWidget() );

    // osumem_t connections :: - -                                                     - -
    connect(
        osumem_t, &QThread::started,
        osumem,   &OsuMem ::runner );
    connect( osumem_t, &QThread::started, this, [&](){
        show();
        fadeTo( 1.0, 400 );
    } );

    // osumem connections :: - -                                                       - -
    connect(
        osumem,     &OsuMem::imgbox_SIG,
        imgbox,     &ImgBox::changeData );
    connect(
        osumem,     &OsuMem    ::sendOsuConfigs,
        osuhandler, &OsuHandler::setOsuConfig,             Qt::DirectConnection );
    connect(
        osumem,     &OsuMem    ::keyChange,
        osuhandler, &OsuHandler::keyChange,                Qt::DirectConnection );
    connect(
        osumem,     &OsuMem    ::playChange,
        osuhandler, &OsuHandler::playChange,               Qt::DirectConnection );
    connect(
        osumem,     &OsuMem    ::onConnect_ConsoleBox_SIG,
        consolebox, &ConsoleBox::setErrCode,               Qt::DirectConnection );
    connect(
        osumem,     &OsuMem    ::consolebox_SIG,
        consolebox, &ConsoleBox::beatmapChange,            Qt::DirectConnection );
    connect(
        osumem,     &OsuMem    ::memosu_err,
        consolebox, &ConsoleBox::setErrCode,               Qt::DirectConnection );
    connect(
        osumem,     &OsuMem    ::resetPaging,
        configbox,  &ConfigBox ::changeStack );
    connect(
        osumem,     &OsuMem    ::resetPaging,
        imgbox,     &ImgBox    ::resetPaging );

    // osuhandler_t connections :: - -                                                 - -
    connect(
        osuhandler_t, &QThread   ::started,
        osuhandler,   &OsuHandler::runner );

    // imgbox connections :: - -                                                       - -
    connect( imgbox, &ImgBox::exit, this, [&](){
        emit saveHistoryNodes();
        connect( fadeTo( 0, 400 ), &QPropertyAnimation::finished, this, [&](){
            this->close();
        } );
    } );
    connect(
        imgbox,    &ImgBox    ::minus,
        this,      &MainWindow::minimize );
    connect(
        imgbox,    &ImgBox    ::changeStack,
        configbox, &ConfigBox ::changeStack  );

    // osuhandler connections :: - -                                                   - -
    connect(
        osuhandler, &OsuHandler::keys,
        imgbox,     &ImgBox    ::updateData );
    connect(
        osuhandler, &OsuHandler::stop_SIG,
        imgbox,     &ImgBox    ::trailStop );
    connect(
        osuhandler, &OsuHandler::playChange_SIG,
        imgbox,     &ImgBox    ::opacityGraph );
    connect(
        osuhandler, &OsuHandler::osuhandler_run,
        imgbox,     &ImgBox    ::osuHandlerSet );
    connect(
        osuhandler, &OsuHandler::sendHistoryNode,
        historybox, &HistoryBox::addHistoryNode );
    connect(
        osuhandler, &OsuHandler::consoleBox_SIG,
        consolebox, &ConsoleBox::inPlayLabel );
    connect(
        osuhandler, &OsuHandler::sendObjectData,
        consolebox, &ConsoleBox::setObjectData,  Qt::DirectConnection );
    connect(
        osuhandler, &OsuHandler::osuhandler_err,
        consolebox, &ConsoleBox::setErrCode,     Qt::DirectConnection );
    connect(
        osuhandler, &OsuHandler::delay,
        consolebox, &ConsoleBox::delay );

    // configbox connections :: - -                                                    - -
    connect(
        configbox,  &ConfigBox ::newConfig,
        osuhandler, &OsuHandler::getButtonState, Qt::DirectConnection );
    connect(
        configbox,  &ConfigBox ::portChange,
        osuhandler, &OsuHandler::portChange,     Qt::DirectConnection );

    // mainwindow connections :: - -                                                   - -
    connect(
        this,       &MainWindow::saveHistoryNodes,
        historybox, &HistoryBox::saveHistoryNodes, Qt::DirectConnection );

    // Thread startpoints :: - -                                                       - -
    osuhandler->moveToThread( osuhandler_t );
    osumem->moveToThread( osumem_t );
    osuhandler_t->start();
    osumem_t->start();
}

// Set program style :: - -                                                            - -
void MainWindow::setMainWindow()
{
    setWindowIcon( QIcon( "res/taskbar.png" ) );
    resize( 831, 1002 );
    setFixedSize( size() );
    setWindowFlags( Qt::FramelessWindowHint );
    setAttribute( Qt::WA_TranslucentBackground );
}


QF* MainWindow::createMainWidget()
{
    QF* mainWidget = new QF();
    mainWidget->setFixedSize( 831, 1002 );
    mainWidget->setStyleSheet(
        "background-color: #111011; border-radius:"
        "20px; border: 3px solid #d3d1d3; color: white"
    );
    mainWidget->setContentsMargins( 0, 0, 0, 0 );

    // Vertical Box that contains all main widgets :: - -                              - -
    QVL* vlayout = new QVL( mainWidget );
    vlayout->setContentsMargins( 0, 0, 0, 0 );

    // Image Box Widget :: - -                                                         - -
    imgbox = new ImgBox( utils, this );
    vlayout->addWidget( imgbox );

    // Bottom portion :: - -                                                           - -
    QVL* bottom_vl  = new QVL();

    // Run and Terminate Buttons (class obj is overkill) :: - -                        - -
    QHL* buttons_hl      = new QHL();
    HF * beatmap_hf      = new HF( this, true );
    HF * clearHistory_hf = new HF( this, true );

    QVL* beatmap_vl      = new QVL( beatmap_hf );
    QVL* clearHistory_vl = new QVL( clearHistory_hf );

    QFont text( utils->getFont( 12 ) );
    beatmap_hf->setFixedSize( 390, 66 );
    clearHistory_hf->setFixedSize( 390, 66 );

    QL* beatmap_ql = new QL( "Beatmap Webpage" );
    beatmap_ql->setStyleSheet( "background-color: transparent; border: 0px" );
    beatmap_ql->setAlignment( Qt::AlignCenter );
    beatmap_ql->setFont( text );
    beatmap_vl->addWidget( beatmap_ql );

    QL* clearHistory_ql = new QL( "Clear History" );
    clearHistory_ql->setStyleSheet( "background-color: transparent; border: 0px" );
    clearHistory_ql->setAlignment( Qt::AlignCenter );
    clearHistory_ql->setFont( text );
    clearHistory_vl->addWidget( clearHistory_ql );

    beatmap_ql->setGraphicsEffect(
        utils->makeDropShadow( QColor( 255, 255, 255 ), this )
    );
    clearHistory_ql->setGraphicsEffect(
        utils->makeDropShadow( QColor( 255, 255, 255 ), this )
    );

    buttons_hl->setContentsMargins( 20, 10, 0, 0 );
    buttons_hl->addWidget( beatmap_hf );
    buttons_hl->addSpacing( 6 );
    buttons_hl->addWidget( clearHistory_hf );
    buttons_hl->addStretch();
    bottom_vl->addLayout( buttons_hl );

    auto bottom_hl = new QHBoxLayout();
    auto left_vl   = new QVBoxLayout();
    auto right_vl  = new QVBoxLayout();

    bottom_hl->addLayout( left_vl );
    bottom_hl->addLayout( right_vl );
    bottom_hl->addStretch();
    bottom_hl->setContentsMargins (20, 6, 0, 0 );

    // Config Box :: - -                                                               - -
    configbox = new ConfigBox( utils, this );
    left_vl->addWidget( configbox );
    left_vl->addSpacing( 6 );

    // Console Box :: - -                                                              - -
    consolebox = new ConsoleBox( utils, this );
    left_vl->addWidget( consolebox );

    // History Box :: - -                                                              - -
    historybox = new HistoryBox(utils, this);
    right_vl->addWidget(historybox);

    left_vl->addStretch();
    right_vl->setContentsMargins( 6, 0, 0, 0 );
    right_vl->addStretch();

    vlayout->addLayout( bottom_vl );
    vlayout->addLayout( bottom_hl );
    vlayout->addStretch();

    connect(
        beatmap_hf,          &HF::clicked,
        osumem,              &OsuMem    ::openBeatmapLink );
    connect(
        clearHistory_hf,     &HF::clicked,
        historybox,          &HistoryBox::clearHistory );

    return mainWidget;
}

// Fade animation maker/starter :: - -                                                 - -
QPA* MainWindow::fadeTo(
    const float& target, const quint16& duration, const QEasingCurve& curve )
{
    QPA* fade_a = new QPA( this, "windowOpacity" );
    fade_a->setDuration( duration );
    fade_a->setStartValue( windowOpacity() );
    fade_a->setEndValue( target );
    fade_a->setEasingCurve( curve );
    fade_a->start( QAbstractAnimation::DeleteWhenStopped );
    return fade_a;
}

void MainWindow::minimize()
{
    fadeTo( 0.0, 250 );
    QTimer::singleShot( 500, this, &QWidget::showMinimized );
}

// Overloaded mouse press fade window function :: - -                                  - -
void MainWindow::mousePressEvent( QMouseEvent* e )
{
    if ( e->button() == Qt::LeftButton ) {
        dragPosition = e->globalPosition().toPoint() - frameGeometry().topLeft();
        e->accept();
        fadeTo( 0.5, 300 );
    }
}

// Overloaded mouse drag fade window function :: - -                                   - -
void MainWindow::mouseMoveEvent( QMouseEvent* e )
{
    if ( e->buttons() & Qt::LeftButton ) {
        move( e->globalPosition().toPoint() - dragPosition );
        e->accept();
    }
}

// Overloaded mouse press fade window function :: - -                                  - -
void MainWindow::mouseReleaseEvent(QMouseEvent* e)
{
    if ( e->button() == Qt::LeftButton ) {
        fadeTo( 1, 300 );
        e->accept();
    }
}

// Ctrl+W -> Exit, Alt+T -> Always on top toggle :: - -                                - -
void MainWindow::keyPressEvent( QKeyEvent* e )
{
    if ( e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_W ) {
        emit saveHistoryNodes();
        connect( fadeTo( 0, 400 ), &QPropertyAnimation::finished, this, [&](){
            this->close();
        } );
    }
    else if ( e->modifiers() == Qt::AltModifier && e->key() == Qt::Key_T )
    {
        m_isAlwaysOnTop = !m_isAlwaysOnTop;
        e->accept();

        QPA* fade = fadeTo( 0, 500, QEasingCurve::OutCubic );
        connect( fade, &QPropertyAnimation::finished, this, [&]()
        {
            if ( m_isAlwaysOnTop ) {
                setWindowFlags(
                    windowFlags() | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint
                );
            }
            else {
                setWindowFlags(
                    windowFlags() ^ ( Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint )
                );
            }
            show();
            fadeTo( 1, 500, QEasingCurve::InCubic );
        } );
    }
    else { QMainWindow::keyPressEvent( e ); }
}

// Overloaded function that allows for unminimized state to go to focused :: - -       - -
void MainWindow::changeEvent( QEvent* e )
{
    if ( e->type() == QEvent::WindowStateChange )
    {
        QWindowStateChangeEvent* sce = static_cast<QWindowStateChangeEvent*>( e );
        if ( ( sce->oldState() & Qt::WindowMinimized ) &&
            !( windowState() & Qt::WindowMinimized ) )
        { fadeTo( 1, 500 ); }
    }
    QMainWindow::changeEvent( e );
}   // :: - -                                                                          - -
