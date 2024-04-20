#include "mainwindow.h"
#include "qevent.h"
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QMessageBox>
#include <QDir>
#include <QMainWindow>
#include <QKeyEvent>
#include <QLineEdit>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QScrollArea>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    scrollMapFont("Noto Sans JP Regular", 12, QFont::Bold),
    scrollDiffFont("Noto Sans JP Regular", 8, QFont::Bold),
    scrollStarFont("Noto Sans JP Regular", 6,  QFont::Bold)
{   // Window settings :: - -                                                          - -
    resize(700, 800);
    setFixedSize(size());
    setStyleSheet("background-color: #111011;");
    setWindowFlags(Qt::FramelessWindowHint);
    setRoundedCorners();
    QIcon icon("res\\Full.png");
    setWindowIcon(icon);

    // Title buttons :: - -                                                            - -
    QPushButton* titleMain = makeButton("res\\title.png", 200,  70, true);
    titleMain->setFixedSize(195, 50);
    connect(titleMain, &QPushButton::clicked, this, [this]() {
        QDesktopServices::openUrl(QUrl("https://github.com/typeRYOON/VSRG-UNO-R3"));
    });

    QPushButton* titleSett = makeButton("res\\title.png", 200,  70, true);
    titleSett->setFixedSize(195, 50);
    connect(titleSett, &QPushButton::clicked, this, [this]() {
        QDesktopServices::openUrl(QUrl("https://github.com/typeRYOON/VSRG-UNO-R3"));
    });

    // Settings Button :: - -                                                          - -
    settingsButtonMain = makeButton("res\\gear.png", 50, 50, true);
    settingsButtonSett = makeButton("res\\gear.png", 50, 50, true);
    connect(settingsButtonMain, &QPushButton::clicked, this, &MainWindow::togglePage);
    connect(settingsButtonSett, &QPushButton::clicked, this, &MainWindow::togglePage);

    // Minimize Button :: - -                                                          - -
    minimizeButtonMain = makeButton("res\\minus.png", 50, 50, true);
    minimizeButtonSett = makeButton("res\\minus.png", 50, 50, true);
    connect(minimizeButtonMain, &QPushButton::clicked, this, &MainWindow::minimizeWindow);
    connect(minimizeButtonSett, &QPushButton::clicked, this, &MainWindow::minimizeWindow);

    // Exit Button :: - -                                                              - -
    exitButtonMain = makeButton("res\\close.png", 50, 50, true);
    exitButtonSett = makeButton("res\\close.png", 50, 50, true);
    connect(exitButtonMain, &QPushButton::clicked, this, &MainWindow::exit);
    connect(exitButtonSett, &QPushButton::clicked, this, &MainWindow::exit);

    // Top Button Layouts :: - -                                                       - -
    QHBoxLayout* topButtonsMain = new QHBoxLayout();
    topButtonsMain->addWidget(titleMain);
    topButtonsMain->addStretch();
    topButtonsMain->addWidget(settingsButtonMain);
    topButtonsMain->addWidget(minimizeButtonMain);
    topButtonsMain->addWidget(exitButtonMain);
    topButtonsMain->setContentsMargins(5, 5, 5, 5);

    QHBoxLayout* topButtonsSett = new QHBoxLayout();
    topButtonsSett->addWidget(titleSett);
    topButtonsSett->addStretch();
    topButtonsSett->addWidget(settingsButtonSett);
    topButtonsSett->addWidget(minimizeButtonSett);
    topButtonsSett->addWidget(exitButtonSett);
    topButtonsSett->setContentsMargins(5, 5, 5, 5);

    // Top Frame :: - -                                                                - -
    QFrame* topFrameMain = new QFrame();
    topFrameMain->setFrameShape(QFrame::Box);
    topFrameMain->setFrameShadow(QFrame::Raised);
    topFrameMain->setLayout(topButtonsMain);
    topFrameMain->setStyleSheet(
        "border: 1px solid #c793a9; border-radius: 10px;"
        "background-color: rgba(0, 0, 0, 0);"
    );
    topFrameMain->setFixedWidth(675);
    topFrameMain->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QFrame* topFrameSett = new QFrame();
    topFrameSett->setFrameShape(QFrame::Box);
    topFrameSett->setFrameShadow(QFrame::Raised);
    topFrameSett->setLayout(topButtonsSett);
    topFrameSett->setStyleSheet(
        "border: 1px solid #c793a9; border-radius: 10px;"
        "background-color: rgba(0, 0, 0, 0);"
    );
    topFrameSett->setFixedWidth(675);
    topFrameSett->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Top box :: - -                                                                  - -
    auto topBoxMain = new QHBoxLayout();
    topBoxMain->addStretch();
    topBoxMain->addWidget(topFrameMain);
    topBoxMain->addStretch();

    auto topBoxSett = new QHBoxLayout();
    topBoxSett->addStretch();
    topBoxSett->addWidget(topFrameSett);
    topBoxSett->addStretch();

    // END TOP MENU BAR  :: - -                                                        - -

    // Fonts for QLabels :: - -                                                        - -
    QFont mapTitleFont("Noto Sans JP Regular", 15, QFont::Bold);
    QFont diffTitleFont("Noto Sans JP Regular", 10, QFont::Bold);
    QFont mapStarFont("Noto Sans JP Regular", 8,  QFont::Bold);
    QFont font("Noto Sans JP Regular", 11, QFont::Bold);

    // Map Background :: - -                                                           - -
    beatmapBG = new QLabel();
    beatmapBG->setStyleSheet(
        "border-radius: 10px;"
        "border: 1px solid #ffc1db;"
    );
    beatmapBG->setFixedSize(670, 210);
    beatmapBG->setScaledContents(true);
    beatmapBG->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    QPixmap startImage = trimImage("res\\noBG.png");
    beatmapBG->setPixmap(startImage);
    //beatmapBG->setAttribute(Qt::WA_OpaquePaintEvent, true);
    //beatmapBG->setAttribute(Qt::WA_NoSystemBackground, true);

    // [TOP] Map Title :: - -                                                          - -
    mapTitle = new QLabel("VSRG-UNO-R3");
    mapTitle->setFixedHeight(20);
    mapTitle->setFont(mapTitleFont);
    mapTitle->setStyleSheet(
        "QLabel { background-color: rgba(0, 0, 0, 0);"
        "color: #ffc1db; }"
    );

    // [BOTTOM] Map Difficulty Title :: - -                                            - -
    diffTitle = new QLabel(" by Ryoon & ChessZra");
    diffTitle->setFixedHeight(15);
    diffTitle->setFont(diffTitleFont);
    diffTitle->setStyleSheet(
        "QLabel { background-color: rgba(0, 0, 0, 0);"
        "color: #ffc1db; }"
    );

    // Star Rectangle :: - -                                                           - -
    starRect = new QFrame();
    starRect->setFixedSize(40, 20);
    starRect->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    starRect->setStyleSheet(
        "QFrame { background-color: #FFFFFF;"
        "border-radius: 10px; }"
    );

    // Star Rectangle Layout :: - -                                                    - -
    QHBoxLayout* starRectLayout = new QHBoxLayout(starRect);
    starRectLayout->setContentsMargins(0, 0, 0, 0);

    mapStar = new QLabel("0.00");
    mapStar->setFont(mapStarFont);
    mapStar->setStyleSheet("QLabel { color: #000000; }");

    starRectLayout->addWidget(mapStar);
    starRectLayout->setAlignment(Qt::AlignCenter);

    // Mania Icon Button :: - -                                                        - -
    maniaIcon = makeButton("res\\mania\\0.00.png", 40, 40, true);
    maniaIcon->setIconSize(QSize(30, 30));
    maniaIcon->setStyleSheet("QPushButton { border: 2px solid #ffc1db;"
        "border-radius: 10px;"
        "background-color: rgba(61, 61, 61, 0.4); }"
        "QPushButton:hover { background-color: rgba(61, 61, 61, 0.6); }"
        "QPushButton:pressed { background-color: rgba(61, 61, 61, 0.8); }"
    );
    connect(maniaIcon, &QPushButton::clicked, this, [this]() {
        QDesktopServices::openUrl(QUrl(mapLink));
    });

    // Vertical Map Info Layout, shift playbox up :: - -                               - -
    QVBoxLayout* mapInfo = new QVBoxLayout();
    playBox = makeButton("res\\inplay.png", 30, 30, true);
    playBox->setIconSize(QSize(25, 25));
    playBoxOpacityEffect = new QGraphicsOpacityEffect();
    playBoxOpacityEffect->setOpacity(0.0);
    playBox->setGraphicsEffect(playBoxOpacityEffect);

    mapInfo->addSpacerItem(new QSpacerItem(0, 5, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mapInfo->addWidget(playBox);
    mapInfo->addSpacerItem(new QSpacerItem(0, 48, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mapInfo->addStretch();

    // Map Info Layout :: - -                                                          - -
    QVBoxLayout* mapInfoLayout = new QVBoxLayout();
    mapInfoLayout->addWidget(starRect);
    mapInfoLayout->addWidget(mapTitle);
    mapInfoLayout->addWidget(diffTitle);

    // Map Info Shift Right, Mania Icon HFix :: - -                                    - -
    QHBoxLayout* hfixManiaIcon = new QHBoxLayout();
    hfixManiaIcon->addLayout(mapInfoLayout);
    hfixManiaIcon->setContentsMargins(2,0,0,1);

    mapInfo->addLayout(hfixManiaIcon);
    mapInfo->addWidget(maniaIcon);

    // Map Info HFix :: - -                                                            - -
    QHBoxLayout* hfixMapInfo = new QHBoxLayout();
    hfixMapInfo->addSpacerItem(new QSpacerItem(10, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    hfixMapInfo->addLayout(mapInfo);
    hfixMapInfo->addStretch();
    hfixMapInfo->setSpacing(2);

    // KPS Frame :: - -                                                                - -
    kpsFrame = new QFrame();
    kpsFrame->setStyleSheet(
        "QFrame { border: 2px solid #ffc1db;"
        "background-color: rgba(61, 61, 61, 0.4);"
        "border-radius: 10px; }"
        "QFrame:hover   { background-color: rgba(61, 61, 61, 0.6); }"
        "QFrame:pressed { background-color: rgba(61, 61, 61, 0.8); }"
    );
    kpsFrame->setFixedSize(QSize(40, 40));

    // KPS Frame Layout :: - -                                                         - -
    QHBoxLayout* kpsFrameLayout = new QHBoxLayout(kpsFrame);
    kpsField = new QLabel("00");
    kpsField->setFont(diffTitleFont);
    kpsField->setStyleSheet(
        "QLabel { border: 0px;"
        "background-color: rgba(0, 0, 0, 0); color: #ffc1db; }"
    );
    kpsFrameLayout->addStretch();
    kpsFrameLayout->addWidget(kpsField);
    kpsFrameLayout->addStretch();

    // Key Strokes Bitmap :: - -                                                       - -
    keystrokes = new QLabel();
    keystrokes->setFixedSize(28, 120);
    keystrokes->setStyleSheet("QLabel { border: 0px; background-color: rgba(0, 0, 0, 0); }");

    QHBoxLayout* centerKeyStrokes = new QHBoxLayout();
    centerKeyStrokes->addWidget(keystrokes);

    // KPS + Key Strokes VBox :: - -                                                   - -
    QVBoxLayout* kps_ksVBox = new QVBoxLayout();
    kps_ksVBox->addStretch();
    kps_ksVBox->addLayout(centerKeyStrokes);
    kps_ksVBox->addWidget(kpsFrame);
    kps_ksVBox->addSpacing(2);

    // KPS Frame HFix :: - -                                                           - -
    QHBoxLayout* hfixKPSFrame = new QHBoxLayout();
    hfixKPSFrame->addStretch();
    hfixKPSFrame->addLayout(kps_ksVBox);
    hfixKPSFrame->addSpacerItem(new QSpacerItem(6, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    hfixMapInfo->addLayout(hfixKPSFrame);

    // BG box put together :: - -                                                      - -
    QGridLayout* bgBox = new QGridLayout();
    bgBox->addWidget(beatmapBG, 0, 0, 0, 10);

    //QLabel* kpsGraph_____TEST = new QLabel();
    //kpsGraph_____TEST->setFixedSize(QSize(670, 210));
    //QPixmap aaa("res\\goober.png");
    //kpsGraph_____TEST->setPixmap(aaa);
    //kpsGraph_____TEST->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    //bgBox->addWidget(kpsGraph_____TEST, 0, 0, 0, 0);

    bgBox->addLayout(hfixMapInfo, 0, 0, 1, 1);

    // Bottom of Image Label shift up :: - -                                           - -
    QWidget* bottomVFix = new QWidget();
    bottomVFix->setFixedHeight(3);
    bgBox->addWidget(bottomVFix, 1, 1);

    // Center the BG image :: - -                                                      - -
    QHBoxLayout* centerBGBox = new QHBoxLayout();
    centerBGBox->addSpacerItem(new QSpacerItem(5, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    centerBGBox->addLayout(bgBox);

    // Generate / Run Buttons :: - -                                                   - -
    termButton = makeButton("res\\bTerminate.png", 330, 56, true);
    termButton->setIconSize(QSize(82, 82));
    termButton->setDisabled(true);
    connect(termButton, &QPushButton::clicked, this, &MainWindow::terminateArduino);

    runButton = makeButton("res\\bRun.png", 330, 56, true);
    runButton->setDisabled(true);
    runButton->setIconSize(QSize(30, 30));
    connect(runButton, &QPushButton::clicked, this, &MainWindow::startProcessing);

    // Generate/Run HBox :: - -                                                        - -
    QHBoxLayout* genRunBox = new QHBoxLayout();
    genRunBox->addStretch();
    genRunBox->addWidget(termButton);
    genRunBox->addStretch();
    genRunBox->addWidget(runButton);
    genRunBox->addStretch();
    genRunBox->addSpacerItem(new QSpacerItem(2, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Button area text :: - -                                                         - -
    QLabel* row1TextA = new QLabel("Key-Strokes");
    QLabel* row1TextB = new QLabel("Key-Strokes");
    row1TextA->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: #ffc1db; }");
    row1TextB->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: #ffc1db; }");
    row1TextA->setFont(font);
    row1TextB->setFont(font);

    QLabel* row2TextA = new QLabel("Keys per Second");
    QLabel* row2TextB = new QLabel("Keys per Second");
    row2TextA->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: #ffc1db; }");
    row2TextB->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: #ffc1db; }");
    row2TextA->setFont(font);
    row2TextB->setFont(font);

    row3TextA = new QLabel("Autoplayer");
    row3TextB = new QLabel("Autoplayer");
    row3TextA->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: #ffc1db; }");
    row3TextB->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: #ffc1db; }");
    row3TextA->setFont(font);
    row3TextB->setFont(font);

    QHBoxLayout* buttonRow1 = new QHBoxLayout();
    A_KS = makeButton("", 30, 30, false);
    C_KS = makeButton("", 30, 30, false);

    buttonRow1->addSpacerItem(new QSpacerItem(5, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    buttonRow1->addWidget(A_KS);
    buttonRow1->addWidget(row1TextA);
    buttonRow1->addSpacerItem(new QSpacerItem(214, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    buttonRow1->addWidget(C_KS);
    buttonRow1->addWidget(row1TextB);
    buttonRow1->addStretch();

    QHBoxLayout* buttonRow2 = new QHBoxLayout();
    A_KPS = makeButton("", 30, 30, false);
    C_KPS = makeButton("", 30, 30, false);

    buttonRow2->addSpacerItem(new QSpacerItem(5, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    buttonRow2->addWidget(A_KPS);
    buttonRow2->addWidget(row2TextA);
    buttonRow2->addSpacerItem(new QSpacerItem(182, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    buttonRow2->addWidget(C_KPS);
    buttonRow2->addWidget(row2TextB);
    buttonRow2->addStretch();

    QHBoxLayout* buttonRow3 = new QHBoxLayout();
    C_AP = makeButton("", 30, 30, false);
    A_AP = makeButton("", 30, 30, false);

    buttonRow3->addSpacerItem(new QSpacerItem(5, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    buttonRow3->addWidget(A_AP);
    buttonRow3->addWidget(row3TextA);
    buttonRow3->addSpacerItem(new QSpacerItem(223, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    buttonRow3->addWidget(C_AP);
    buttonRow3->addWidget(row3TextB);
    buttonRow3->addStretch();

    QHBoxLayout* buttonTopHBox = new QHBoxLayout();
    QVBoxLayout* buttonVBox    = new QVBoxLayout();
    buttonVBox->addLayout(buttonRow1);
    buttonVBox->addLayout(buttonRow2);
    buttonVBox->addLayout(buttonRow3);
    buttonTopHBox->addLayout(buttonVBox);

    // Animation Dummy :: - -                                                          - -
    dummy1 = new QWidget();
    dummy1->setGeometry(0, 0, 0, 0);
    dummy1->setFixedSize(QSize(0, 0));

    // Connect the button booleans :: - -                                              - -
    connect(A_KS, &QPushButton::clicked, this, [=, this]()
    {
        if (bA_KS) { A_KS->setIcon(QIcon());                   checkButtonCount(-1); }
        else       { A_KS->setIcon(QIcon("res\\checked.png")); checkButtonCount(1); }
        bA_KS = !bA_KS;
    });

    connect(A_KPS, &QPushButton::clicked, this, [=, this]()
    {
        if (bA_KPS) { A_KPS->setIcon(QIcon());                   checkButtonCount(-1); }
        else        { A_KPS->setIcon(QIcon("res\\checked.png")); checkButtonCount(1); }
        bA_KPS = !bA_KPS;
    });

    connect(A_AP, &QPushButton::clicked, this, [=, this]()
    {
        if (bA_AP) {
            A_AP->setIcon(QIcon());
            C_AP->setDisabled(false);
            checkButtonCount(-1);
            row3TextB->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: #ffc1db; }");
            C_AP->setStyleSheet("QPushButton { border: 2px solid #ffc1db; border-radius: 10px; background-color: rgba(0, 0, 0, 0); }"
                                "QPushButton:hover { background-color: rgba(0, 0, 0, 0.2); }"
                                "QPushButton:pressed { background-color: rgba(0, 0, 0, 0.4); }");;
        }
        else {
            A_AP->setIcon(QIcon("res\\checked.png"));
            C_AP->setDisabled(true);
            checkButtonCount(1);
            row3TextB->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: #888888; }");
            C_AP->setStyleSheet("QPushButton { border: 2px solid #888888; border-radius: 10px; background-color: rgba(0, 0, 0, 0); }"
                                "QPushButton:hover { background-color: rgba(0, 0, 0, 0.0); }"
                                "QPushButton:pressed { background-color: rgba(0, 0, 0, 0.0); }");;
        }
        bA_AP = !bA_AP;
    });

    connect(C_KS, &QPushButton::clicked, this, [=, this]()
    {
        if (bC_KS) { C_KS->setIcon(QIcon());                   checkButtonCount(-1); }
        else       { C_KS->setIcon(QIcon("res\\checked.png")); checkButtonCount(1); }
        bC_KS = !bC_KS;
    });

    connect(C_KPS, &QPushButton::clicked, this, [=, this]()
    {
        if (bC_KPS) { C_KPS->setIcon(QIcon());                  checkButtonCount(-1); }
        else       { C_KPS->setIcon(QIcon("res\\checked.png")); checkButtonCount(1); }
        bC_KPS = !bC_KPS;
    });

    connect(C_AP, &QPushButton::clicked, this, [=, this]()
    {
        if (bC_AP) {
            C_AP->setIcon(QIcon());
            A_AP->setDisabled(false);
            checkButtonCount(-1);
            row3TextA->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: #ffc1db; }");
            A_AP->setStyleSheet("QPushButton { border: 2px solid #ffc1db; border-radius: 10px; background-color: rgba(0, 0, 0, 0); }"
                                "QPushButton:hover { background-color: rgba(0, 0, 0, 0.2); }"
                                "QPushButton:pressed { background-color: rgba(0, 0, 0, 0.4); }");;
        }
        else {
            C_AP->setIcon(QIcon("res\\checked.png"));
            A_AP->setDisabled(true);
            checkButtonCount(1);
            row3TextA->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 0); color: #888888; }");
            A_AP->setStyleSheet("QPushButton { border: 2px solid #888888; border-radius: 10px; background-color: rgba(0, 0, 0, 0); }"
                                "QPushButton:hover { background-color: rgba(0, 0, 0, 0.0); }"
                                "QPushButton:pressed { background-color: rgba(0, 0, 0, 0.0); }");;
        }
        bC_AP = !bC_AP;
    });

    // Arduino/Computer Header :: - -                                                  - -
    QHBoxLayout* headerBox = new QHBoxLayout();
    QLabel* typeHeader = new QLabel();
    QPixmap typeHeaderPix("res\\typeHeader");

    typeHeader->setPixmap(typeHeaderPix);
    headerBox->addStretch();
    headerBox->addWidget(typeHeader);
    headerBox->addStretch();
    headerBox->addSpacerItem(new QSpacerItem(10, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Message Area :: - -                                                             - -
    QHBoxLayout* bottomHLayout = new QHBoxLayout();
    msgBox = new QTextEdit();
    msgBox->setReadOnly(true);
    msgBox->setStyleSheet(
        "QTextEdit { background-color: black;"
        "color: #ffc1db; font-size: 10pt; border: 2px solid #ffc1db; border-radius: 10px; }"
    );
    msgBox->setFixedSize(QSize(165, 250));
    msgBox->setFont(diffTitleFont);
    msgBox->setText("  ヽ(*ﾟдﾟ)ノｶｲﾊﾞｰ");

    QPixmap bongoCatPix("res\\Bongo-Cat");
    QLabel* bongoCat = new QLabel();
    bongoCat->setFixedSize(165, 43);
    bongoCat->setStyleSheet("QLabel { background-color: transparent; }");
    bongoCat->setFrameStyle(QFrame::NoFrame);
    bongoCat->setPixmap(bongoCatPix);
    QVBoxLayout* bongoVFix = new QVBoxLayout();
    bongoVFix->addSpacerItem(new QSpacerItem(0, 200, QSizePolicy::Minimum, QSizePolicy::Expanding));
    bongoVFix->addWidget(bongoCat);

    QGridLayout* msgBoxLayers = new QGridLayout();
    msgBoxLayers->addWidget(msgBox, 0, 0, 0, 0);
    msgBoxLayers->addLayout(bongoVFix, 0, 0, 0, 0);

    scrollWidget = new QWidget();
    scrollWidget->setAttribute(Qt::WA_TranslucentBackground);
    scrollVLayout = new QVBoxLayout(scrollWidget);

    scrollVLayout->addLayout(createScrollWidget("res\\noBG-Small.png", true));
    scrollVLayout->addLayout(createScrollWidget("res\\noBG-Small.png", true));
    scrollVLayout->addLayout(createScrollWidget("res\\noBG-Small.png", true));
    scrollVLayout->addLayout(createScrollWidget("res\\noBG-Small.png", true));
    scrollVLayout->addLayout(createScrollWidget("res\\noBG-Small.png", true));

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setMinimumSize(510, 250);
    scrollArea->setMaximumSize(510, 250);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setStyleSheet(
        "QScrollArea { border: none; background-color: transparent; }"
        "QScrollBar:vertical { width: 0; }"
    );

    bottomHLayout->addSpacerItem(new QSpacerItem(5, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    bottomHLayout->addLayout(msgBoxLayers);
    bottomHLayout->addWidget(scrollArea);
    bottomHLayout->addStretch();

    // SETTINGS :: - -                                                                 - -
    QFrame* creditsFrame = new QFrame();
    creditsFrame->setObjectName("creditsFrame");
    creditsFrame->setStyleSheet(
        "#creditsFrame { background-color: black;"
        "border: 1px solid #ffc1db; border-radius: 10px; }"
    );
    creditsFrame->setFixedSize(670, 210);
    creditsFrame->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    QHBoxLayout* cFrameHFix = new QHBoxLayout();
    cFrameHFix->addSpacerItem(new QSpacerItem(-1, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    cFrameHFix->addWidget(creditsFrame);

    QVBoxLayout* cFrameVLayout = new QVBoxLayout();

    QHBoxLayout* cFrameR1 = new QHBoxLayout();
    QLabel* cDesc = new QLabel();
    cDesc->setText("A stats tool for osu!mania.\n"
                   "Authors : :\n"
                   " Ryan Magdaleno\n"
                   " John Ezra See");
    cDesc->setStyleSheet(
        "QLabel { background-color: rgba(0, 0, 0, 0); color: #ffc1db; }"
    );

    QFont cDescFont("Noto Sans JP Regular", 13, QFont::Bold);
    cDesc->setFont(cDescFont);

    QFrame* versionBox = new QFrame();
    versionBox->setFixedSize(QSize(92, 92));
    versionBox->setObjectName("versionBox");
    versionBox->setStyleSheet(
        "#versionBox { background-color: black;"
        "border: 2px solid #ffc1db; border-radius: 10px; }"
    );
    QVBoxLayout* versionBoxVBox = new QVBoxLayout(versionBox);
    QHBoxLayout* versionTop = new QHBoxLayout();
    QLabel* vsrgICO = new QLabel();
    QPixmap vsrgPix("res\\VSmall.png");
    vsrgICO->setFixedSize(25, 25);
    vsrgICO->setPixmap(vsrgPix);

    QLabel* vsrgText = new QLabel();
    vsrgText->setText("1.00");
    vsrgText->setFont(diffTitleFont);
    vsrgText->setStyleSheet(
        "QLabel { background-color: rgba(0, 0, 0, 0); color: #ffc1db; }"
    );

    versionTop->addStretch();
    versionTop->addWidget(vsrgICO);
    versionTop->addWidget(vsrgText);
    versionTop->addStretch();

    QHBoxLayout* versionBot = new QHBoxLayout();
    QLabel* QtICO = new QLabel();
    QPixmap QtPix("res\\QTSmall.png");
    QtICO->setFixedSize(25, 25);
    QtICO->setPixmap(QtPix);

    QLabel* QtText = new QLabel();
    QtText->setText("6.12");
    QtText->setFont(diffTitleFont);
    QtText->setStyleSheet(
        "QLabel { background-color: rgba(0, 0, 0, 0); color: #ffc1db; }"
    );

    versionBot->addStretch();
    versionBot->addWidget(QtICO);
    versionBot->addWidget(QtText);
    versionBot->addStretch();

    versionBoxVBox->addStretch();
    versionBoxVBox->addLayout(versionTop);
    versionBoxVBox->addLayout(versionBot);
    versionBoxVBox->addStretch();

    cFrameR1->addSpacerItem(new QSpacerItem(2, 0, QSizePolicy::Minimum, QSizePolicy::Minimum));
    cFrameR1->addWidget(cDesc);
    cFrameR1->addStretch();
    cFrameR1->addWidget(versionBox);

    QHBoxLayout* cFrameR2 = new QHBoxLayout();


    QFrame* portBox = new QFrame();
    portBox->setFixedSize(QSize(260, 92));
    portBox->setObjectName("portBox");
    portBox->setStyleSheet(
        "#portBox { background-color: black;"
        "border: 2px solid #ffc1db; border-radius: 10px; }"
    );

    // portBox Horizontal Layout :: - -                                                - -
    QHBoxLayout* portBoxHL = new QHBoxLayout(portBox);
    QPushButton* portCheck = makeButton("res\\refresh.png", 60, 60, true);
    portCheck->setIconSize(QSize(45, 45));

    QVBoxLayout* portFieldRows = new QVBoxLayout();
    QHBoxLayout* portFieldR1   = new QHBoxLayout(), *portFieldR2 = new QHBoxLayout();

    // Row One :: - -                                                                  - -
    QLabel* KS_KPS_Label = new QLabel();
    KS_KPS_Label->setFixedSize(QSize(63, 24));
    KS_KPS_Label->setText("KS/KPS");
    KS_KPS_Label->setFont(diffTitleFont);
    KS_KPS_Label->setStyleSheet(
        "QLabel { background-color: transparent; color: #ffc1db;"
        " border: 2px solid #ffc1db; border-radius: 5px; }"
    );
    KS_KPS_PortField = new QLineEdit();
    KS_KPS_PortField->setFixedSize(QSize(90, 24));
    KS_KPS_PortField->setFont(diffTitleFont);
    KS_KPS_PortField->setStyleSheet(
        "QLineEdit { color: #ffc1db; border: none;"
        "background-color: transparent; border-bottom: 1px solid #ffc1db; }"
    );
    QPixmap startPortImg("res\\invalid.png");
    validPortR1 = new QLabel();
    validPortR1->setStyleSheet("QLabel { background-color: transparent; }");
    validPortR1->setFixedSize(QSize(24, 24));
    validPortR1->setPixmap(startPortImg);

    portFieldR1->addWidget(KS_KPS_Label);
    portFieldR1->addWidget(KS_KPS_PortField);
    portFieldR1->addWidget(validPortR1);
    portFieldR1->addStretch();

    // Row Two :: - -                                                                  - -
    QLabel* AU_Label = new QLabel();
    AU_Label->setFixedSize(QSize(63, 24));
    AU_Label->setText("    AU");
    AU_Label->setFont(diffTitleFont);
    AU_Label->setStyleSheet(
        "QLabel { background-color: transparent; color: #ffc1db;"
        " border: 2px solid #ffc1db; border-radius: 5px; }"
    );
    AU_PortField = new QLineEdit();
    AU_PortField->setFixedSize(QSize(90, 24));
    AU_PortField->setFont(diffTitleFont);
    AU_PortField->setStyleSheet(
        "QLineEdit { color: #ffc1db; border: none;"
        "background-color: transparent; border-bottom: 1px solid #ffc1db; }"
    );
    validPortR2 = new QLabel();
    validPortR2->setStyleSheet("QLabel { background-color: transparent; }");
    validPortR2->setFixedSize(QSize(24, 24));
    validPortR2->setPixmap(startPortImg);

    connect(portCheck, &QPushButton::clicked, this, &MainWindow::checkPorts);

    portFieldR2->addWidget(AU_Label);
    portFieldR2->addWidget(AU_PortField);
    portFieldR2->addWidget(validPortR2);
    portFieldR2->addStretch();

    portFieldRows->addStretch();
    portFieldRows->addLayout(portFieldR1);
    portFieldRows->addLayout(portFieldR2);
    portFieldRows->addStretch();

    portBoxHL->addSpacerItem(new QSpacerItem(4, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    portBoxHL->addWidget(portCheck);
    portBoxHL->addLayout(portFieldRows);
    portBoxHL->addStretch();

    QVBoxLayout* cDesc2VFix = new QVBoxLayout();
    cDesc2VFix->addStretch();

    cFrameR2->addLayout(cDesc2VFix);
    cFrameR2->addStretch();
    cFrameR2->addWidget(portBox);

    cFrameVLayout->addLayout(cFrameR1);
    cFrameVLayout->addLayout(cFrameR2);
    creditsFrame->setLayout(cFrameVLayout);


    QPixmap kurisuImg("res\\kurisu.png");
    QHBoxLayout* kurisuHFix = new QHBoxLayout();
    QPushButton* kurisu = new QPushButton();
    kurisu->setIconSize(QSize(150, 200));
    kurisu->setIcon(QIcon(kurisuImg));
    kurisu->setCursor(Qt::PointingHandCursor);
    kurisu->setStyleSheet("QPushButton { background-color: transparent; border: none; }");
    QObject::connect(kurisu, &QPushButton::clicked, [&]()
    {
        QDesktopServices::openUrl(QUrl("https://www.pixiv.net/en/artworks/13099007"));
    });

    kurisuHFix->addWidget(kurisu);
    kurisuHFix->addStretch();

    // Paging system :: - -                                                            - -
    mainPage = new QWidget(this);
    settPage = new QWidget(this);
    pager    = new QStackedWidget(this);

    // Main layout :: - -                                                              - -
    mainLayout = new QVBoxLayout(mainPage);
    mainLayout->setGeometry(QRect(0, 0, 700, 300));
    mainLayout->addSpacerItem(new QSpacerItem(0, 5, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addLayout(topBoxMain);
    mainLayout->addSpacerItem(new QSpacerItem(0, 15, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addLayout(centerBGBox);
    mainLayout->addLayout(genRunBox);
    mainLayout->addLayout(headerBox);
    mainLayout->addLayout(buttonTopHBox);
    mainLayout->addSpacerItem(new QSpacerItem(0, 15, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addLayout(bottomHLayout);

    mainLayout->addStretch(1);
    mainLayout->addStretch();

    // Setting layout :: - -                                                           - -
    settLayout = new QVBoxLayout(settPage);
    settLayout->setGeometry(QRect(0, 0, 700, 300));
    settLayout->addSpacerItem(new QSpacerItem(0, 5, QSizePolicy::Minimum, QSizePolicy::Expanding));
    settLayout->addLayout(topBoxSett);
    settLayout->addSpacerItem(new QSpacerItem(0, 15, QSizePolicy::Minimum, QSizePolicy::Expanding));
    settLayout->addLayout(cFrameHFix);
    settLayout->addSpacerItem(new QSpacerItem(0, 5, QSizePolicy::Minimum, QSizePolicy::Expanding));

    settLayout->addStretch(1);
    settLayout->addStretch();
    settLayout->addLayout(kurisuHFix);

    // Tie Pages to Pager :: - -                                                       - -
    pager->setGeometry(QRect(0, 0, 700, 800));
    pager->addWidget(mainPage);
    pager->addWidget(settPage);

    // Fade Window in :: - -                                                           - -
    makeBitmap();

    // Osu Memory Checking Object :: - -                                               - -
    webSocket = new WebSocketMonitor();
    connect(webSocket, &WebSocketMonitor::jsonDataReceived, this, &MainWindow::handleJsonData);
    connect(webSocket, &WebSocketMonitor::websocketError, this, &MainWindow::handleWebSocketError);
    connect(webSocket, &WebSocketMonitor::playChange, this, &MainWindow::changePlayStatus);
    connect(webSocket, &WebSocketMonitor::readOsu, this, &MainWindow::readOsuConfig);
    webSocket->startMonitoring();

    // Runner Object :: - -                                                            - -
    workerThread = new QThread();
    worker = new ArduinoHandler();
    connect(this, &MainWindow::sendVirtualKeys, worker, &ArduinoHandler::receiveVirtualKeys, Qt::DirectConnection);
    connect(this, &MainWindow::sendCurOsu, worker, &ArduinoHandler::receiveOsuFile, Qt::DirectConnection);
    connect(this, &MainWindow::terminateArduino, worker, &ArduinoHandler::receiveTermination, Qt::DirectConnection);

    connect(worker, &ArduinoHandler::running, this, &MainWindow::arduinoIsOn, Qt::DirectConnection);
    connect(worker, &ArduinoHandler::stopThread, this, &MainWindow::stopWorkerThread, Qt::QueuedConnection);
    connect(worker, &ArduinoHandler::sendKStoMain, this, &MainWindow::keystrokesMake, Qt::QueuedConnection);
    connect(worker, &ArduinoHandler::resetKeyStrokes, this, &MainWindow::makeBitmap);
    connect(worker, &ArduinoHandler::msg, this, &MainWindow::receiveMsg, Qt::DirectConnection);
    connect(workerThread, &QThread::started, [=, this]()
    {
        if (bC_AP) {
            emit sendCurOsu(QString("%1\\Songs\\%2\\%3").arg(beatmapInfo.at(0), beatmapInfo.at(5), beatmapInfo.at(6)));
        }
        worker->run(beatmapInfo.at(4).toInt(),
                    bA_KS, bA_KPS, bA_AP, bC_KS, bC_KPS, bC_AP,
                    KPS_KS_PortName, AU_PortName);
    });
    worker->moveToThread(workerThread);
}

// Destructor :: - -                                                                   - -
MainWindow::~MainWindow() { delete pager; }

// Button Factory :: - -                                                               - -
QPushButton* MainWindow::makeButton(const QString &str, const int w, const int h, const bool isImg)
{
    QPushButton* button = new QPushButton(str);
    if (isImg) {
        button->setText("");
        button->setIcon(QIcon(str));
        button->setIconSize(QSize(w-20, h-20));
    }
    button->setCursor(Qt::PointingHandCursor);
    button->setFixedSize(w, h);
    button->setStyleSheet("QPushButton { color: #ffc1db; border: 2px solid #ffc1db;"
                          "border-radius: 10px; background-color: rgba(0, 0, 0, 0); }"
                          "QPushButton:hover { background-color: rgba(0, 0, 0, 0.2); }"
                          "QPushButton:pressed { background-color: rgba(0, 0, 0, 0.4); }");
    return button;
}

// Run the ArduinoHandler's current configuration :: - -                               - -
void MainWindow::startProcessing()
{
    handlerOn = true;

    makeBitmap();
    msgBox->clear();
    msgBox->append("\n : : Ready to run: [H].");

    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect();
    scrollWidget->setGraphicsEffect(effect);

    QPropertyAnimation* fadeOutAnimation = new QPropertyAnimation(effect, "opacity");
    fadeOutAnimation->setDuration(500);
    fadeOutAnimation->setStartValue(1.0);
    fadeOutAnimation->setEndValue(0.0);
    fadeOutAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    QPropertyAnimation* fadeInAnimation = new QPropertyAnimation(effect, "opacity");
    fadeInAnimation->setDuration(500);
    fadeInAnimation->setStartValue(0.0);
    fadeInAnimation->setEndValue(1.0);
    fadeInAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    connect(fadeOutAnimation, &QPropertyAnimation::finished, [=, this]() {
        QString bgPath;
        if (!beatmapInfo.at(7).isEmpty()) {
            bgPath = QString("%1\\Songs\\%2\\%3").arg(beatmapInfo.at(0), beatmapInfo.at(5), beatmapInfo.at(7));
        } else {
            bgPath = "res\\noBG-Small.png";
        }
        QLayoutItem* layoutRemove = scrollVLayout->itemAt(4);
        scrollVLayout->removeItem(layoutRemove);
        delete layoutRemove->layout();
        scrollVLayout->insertLayout(0, createScrollWidget(bgPath, false));
        fadeInAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    });

    runButton->setDisabled(true);
    termButton->setDisabled(false);
    fadeOutAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    workerThread->start();
}

// Set the Virtual Key Vectors :: - -                                                  - -
void MainWindow::setKeys
    (const QString& k4, const QString& k5, const QString& k6, const QString& k7)
{
    QHash<QString, uint8_t> keyMappings;
    QStringList keys4 = k4.split(' ', Qt::SkipEmptyParts);
    QStringList keys5 = k5.split(' ', Qt::SkipEmptyParts);
    QStringList keys6 = k6.split(' ', Qt::SkipEmptyParts);
    QStringList keys7 = k7.split(' ', Qt::SkipEmptyParts);

    for (char c = 'A'; c <= 'Z'; ++c) {
        QString character(c);
        keyMappings[character] = static_cast<uint8_t>(c);
    }
    uint8_t val = 0x48; // 0
    for (char c = '0'; c <= '9'; ++c) {
        QString character(c);
        keyMappings[character] = val++;
    }
    keyMappings["OemCloseBrackets"] = 0xDD;
    keyMappings["OemOpenBrackets"]  = 0xDB;
    keyMappings["OemSemicolon"]     = 0xBA;
    keyMappings["OemQuestion"]      = 0xBF;
    keyMappings["OemQuotes"]        = 0xDE;
    keyMappings["OemPeriod"]        = 0xBE;
    keyMappings["OemComma"]         = 0xBC;
    keyMappings["OemMinus"]         = 0X6D;
    keyMappings["OemPipe"]          = 0xDC;
    keyMappings["OemPlus"]          = 0x6B;
    keyMappings["Space"]            = 0x20;
    QVector<uint8_t> virtual4k, virtual5k, virtual6k, virtual7k;

    // Push back :: - -
    foreach(const QString& key, keys4) { virtual4k.append(keyMappings.value(key)); }
    foreach(const QString& key, keys5) { virtual5k.append(keyMappings.value(key)); }
    foreach(const QString& key, keys6) { virtual6k.append(keyMappings.value(key)); }
    foreach(const QString& key, keys7) { virtual7k.append(keyMappings.value(key)); }

    // Sanity check :: - -
    if (virtual4k.size() != 4) {
        handleWebSocketError("Please set your 4K keys in-game.");
    } if (virtual5k.size() != 5) {
        handleWebSocketError("Please set your 5K keys in-game.");
    } if (virtual6k.size() != 6) {
        handleWebSocketError("Please set your 6K keys in-game.");
    } if (virtual7k.size() != 7) {
        handleWebSocketError("Please set your 7K keys in-game.");
    }
    emit sendVirtualKeys(virtual4k, virtual5k, virtual6k, virtual7k);
}

void MainWindow::toggleAlwaysOnTop()
{
    Qt::WindowFlags flags = windowFlags();
    m_isAlwaysOnTop = !m_isAlwaysOnTop;
    if (m_isAlwaysOnTop)
        setWindowFlags(flags | Qt::WindowStaysOnTopHint);
    else
        setWindowFlags(flags & ~Qt::WindowStaysOnTopHint);
    show();
}

void MainWindow::checkPorts()
{
    QString KS_KPS_Check = KS_KPS_PortField->text();
    QString AU_Check     = AU_PortField->text();
    KPS_KS_PortName = "-1", AU_PortName = "-1";

    const QList<QSerialPortInfo> serialPortInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &pInfo : serialPortInfos)
    {
        if (!pInfo.description().contains("Arduino")) { continue; }
        if (!KS_KPS_Check.isEmpty() &&
            pInfo.portName().contains(KS_KPS_Check, Qt::CaseInsensitive))
        {
            KPS_KS_PortName = pInfo.portName();
        }
        else if (!AU_Check.isEmpty() &&
                 pInfo.portName().contains(AU_Check, Qt::CaseInsensitive))
        {
            AU_PortName = pInfo.portName();
        }
    }

    QPixmap invalid("res\\invalid.png"), valid("res\\valid.png");
    if (KPS_KS_PortName != "-1") {
        KS_KPS_PortField->setText(KPS_KS_PortName);
        validPortR1->setPixmap(valid);
    } else {
        KS_KPS_PortField->setText("NO PORT");
        validPortR1->setPixmap(invalid);
    }
    if (AU_PortName != "-1") {
        AU_PortField->setText(AU_PortName);
        validPortR2->setPixmap(valid);
    } else {
        AU_PortField->setText("NO PORT");
        validPortR2->setPixmap(invalid);
    }
}

void MainWindow::checkButtonCount(const int8_t& i)
{
    if (firstMap && buttonsOn == 0 && i ==  1) { runButton->setDisabled(false); }
    if (firstMap && buttonsOn == 1 && i == -1) { runButton->setDisabled(true) ; }
    buttonsOn += i;
}

QLayout* MainWindow::createScrollWidget(const QString& bgPath, const bool& empty)
{
    QGridLayout* widgetLayout = new QGridLayout();
    QFrame* outerBox = new QFrame();
    outerBox->setFixedSize(QSize(490, 76));
    outerBox->setStyleSheet(
        "QFrame { color: #ffc1db; border: 2px solid #ffc1db;"
        "border-radius: 10px; background-color: transparent; }"
    );

    // Create mini image cutout :: - -
    QPixmap og(bgPath);
    uint16_t scalar = og.width() / 2.10526315789;
    og = og.copy(0, og.height() / 2 - scalar / 2, og.width(), scalar).scaled(
        160, 76, Qt::KeepAspectRatio, Qt::SmoothTransformation
    );

    QPixmap dimmedImage(og.size());
    dimmedImage.fill(Qt::transparent);
    QPainter painter(&dimmedImage);
    painter.setOpacity(0.6);
    QPainterPath clipPath;
    clipPath.addRoundedRect(dimmedImage.rect(), 8, 8);
    painter.setClipPath(clipPath);
    painter.drawPixmap(0, 0, og);
    painter.end();

    QLabel* imgFrame = new QLabel();
    imgFrame->setPixmap(dimmedImage);
    imgFrame->setAutoFillBackground(true);
    imgFrame->setFrameStyle(QFrame::NoFrame);
    imgFrame->setFixedSize(dimmedImage.size());

    // Map Info Frame :: - -                                                           - -
    QHBoxLayout* mapInfoHFix = new QHBoxLayout();
    QFrame* mapInfoFrame = new QFrame();
    mapInfoFrame->setFixedSize(QSize(338, 76));
    mapInfoFrame->setObjectName("mapInfoFrame");
    mapInfoFrame->setStyleSheet(
        "#mapInfoFrame { color: #ffc1db; border: 2px solid #ffc1db;"
        "border-radius: 10px; background-color: #111011; }"
    );

    QVBoxLayout* mapInfoLayout = new QVBoxLayout(mapInfoFrame);

    QString mania = "empty", color = "111011", starColor = "111011";
    QString s_map = "", s_diff = "", s_star = "";

    if (!empty)
    {
        change(mania, color, starColor);
        s_map  = beatmapInfo.at(2);
        s_diff = QString("[%1K] : : %2").arg(beatmapInfo.at(4), beatmapInfo.at(3));
        s_star = beatmapInfo.at(1);
    }

    QLabel* mapName = new QLabel(s_map);
    mapName->setFixedHeight(26);
    mapName->setFont(scrollMapFont);
    mapName->setStyleSheet("QLabel { background-color: transparent; color: #ffc1db; }" );

    QLabel* diffName = new QLabel(QString(" %1").arg(s_diff));
    diffName->setFixedHeight(14);
    diffName->setFont(scrollDiffFont);
    diffName->setStyleSheet("QLabel { background-color: transparent; color: #ffc1db; }");

    // Star Rectangle :: - -                                                           - -
    QFrame* starScrollRect = new QFrame();
    starScrollRect->setFixedSize(30, 16);
    starScrollRect->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    starScrollRect->setStyleSheet(
        QString("QFrame { background-color: #%1; border-radius: 8px; }").arg(color)
    );

    // Star Rectangle Layout :: - -                                                    - -
    QHBoxLayout* starRectLayout = new QHBoxLayout(starScrollRect);
    starRectLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* starText = new QLabel(s_star);
    starText->setFont(scrollStarFont);
    starText->setStyleSheet(
        QString("QLabel { color: #%1; }").arg(starColor)
    );

    starRectLayout->addWidget(starText);
    starRectLayout->setAlignment(Qt::AlignCenter);

    QPixmap maniaIconPix(QString("res\\mania\\%1.png").arg(mania));
    maniaIconPix = maniaIconPix.scaled(QSize(16, 16), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QLabel* maniaIcon = new QLabel();
    maniaIcon->setFixedSize(16, 16);
    maniaIcon->setStyleSheet("QLabel { background-color: transparent; }");
    maniaIcon->setFrameStyle(QFrame::NoFrame);
    maniaIcon->setPixmap(maniaIconPix);

    QHBoxLayout* maniaStar = new QHBoxLayout();
    maniaStar->setContentsMargins(6, 0, 0, 0);
    maniaStar->addWidget(maniaIcon);
    maniaStar->addWidget(starScrollRect);
    maniaStar->addStretch();

    mapInfoLayout->setContentsMargins(12, 8, 0, 6);
    QGridLayout* textLayers = new QGridLayout();
    QVBoxLayout* maniaStarVFix = new QVBoxLayout();
    maniaStarVFix->addSpacerItem(new QSpacerItem(0, 36, QSizePolicy::Minimum, QSizePolicy::Expanding));
    maniaStarVFix->addLayout(maniaStar);
    maniaStarVFix->addStretch();


    QVBoxLayout* mapVFix = new QVBoxLayout();
    mapVFix->addWidget(mapName);
    mapVFix->addStretch();

    QVBoxLayout* diffVFix = new QVBoxLayout();
    diffVFix->addSpacerItem(new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    diffVFix->addWidget(diffName);
    diffVFix->addStretch();
    textLayers->addLayout(mapVFix,       0, 0, 0, 0);
    textLayers->addLayout(diffVFix,      0, 0, 0, 0);
    textLayers->addLayout(maniaStarVFix, 0, 0, 0, 0);

    mapInfoLayout->addLayout(textLayers);
    mapInfoLayout->addStretch();

    mapInfoHFix->addStretch();
    mapInfoHFix->addWidget(mapInfoFrame);

    widgetLayout->addWidget(imgFrame   , 0, 0, 0, 0);
    widgetLayout->addWidget(outerBox   , 0, 0, 0, 0);
    widgetLayout->addLayout(mapInfoHFix, 0, 0, 0, 0);
    return widgetLayout;
}

void MainWindow::readOsuConfig(const QString& osuPath)
{
    QDir osuDir(osuPath);

    QStringList filters;
    filters << "osu!*cfg";
    osuDir.setNameFilters(filters);
    osuDir.setFilter(QDir::Files);

    QStringList fileList = osuDir.entryList();
    QString line, k4, k5, k6, k7;

    for (const QString& file : fileList) {
        QFile configFile(osuDir.absoluteFilePath(file));
        if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            handleWebSocketError("Unable to open cfg file.");
        }

        QTextStream in(&configFile);
        while (!in.atEnd())
        {
            line = in.readLine();
            if (line.startsWith("ManiaLayouts4K")) {
                k4 = line.mid(17);
            } else if (line.startsWith("ManiaLayouts5K")) {
                k5 = line.mid(17);
            } else if (line.startsWith("ManiaLayouts6K")) {
                k6 = line.mid(17);
            } else if (line.startsWith("ManiaLayouts7K")) {
                k7 = line.mid(17);
            }
        }
    }

    if (k4.isEmpty() || k5.isEmpty() || k6.isEmpty() || k7.isEmpty()) {
        handleWebSocketError("Please set your 4-7K keys.");
    }
    setKeys(k4, k5, k6, k7);
    show();
    fadeIn();
}

void MainWindow::arduinoIsOn()
{
    ArduinoOn = true;
    msgBox->append(" : : Running.");
}

void MainWindow::receiveMsg(const QString& message)
{
    msgBox->append(message);
}

QPixmap MainWindow::trimImage(const QString& filePath)
{
    QPixmap og(filePath);
    uint16_t scalar = og.width() / 3.19047619048;
    og = og.copy(0, og.height() / 2 - scalar / 2, og.width(), scalar).scaled(
        670, 210, Qt::KeepAspectRatio, Qt::SmoothTransformation
    );

    QPixmap dimmedImage(og.size());
    dimmedImage.fill(Qt::transparent);

    // Draw the original pixmap with reduced opacity
    QPainter painter(&dimmedImage);
    painter.setOpacity(0.6);

    // Apply clipping to the original image
    QPainterPath clipPath;
    clipPath.addRoundedRect(dimmedImage.rect(), 8, 8); // Adjust the radii as needed
    painter.setClipPath(clipPath);

    // Draw the original image inside the clipping region
    painter.drawPixmap(0, 0, og);
    painter.end();

    return dimmedImage;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();

        QPropertyAnimation* animation = new QPropertyAnimation(this, "windowOpacity");
        animation->setDuration(300);
        animation->setStartValue(1.0);
        animation->setEndValue(0.5);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
        animation->setDuration(300);
        animation->setStartValue(0.5);
        animation->setEndValue(1.0);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        event->accept();
    }
}

void MainWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::WindowStateChange) {
        QWindowStateChangeEvent *stateChangeEvent = static_cast<QWindowStateChangeEvent*>(event);
        if ((stateChangeEvent->oldState() & Qt::WindowMinimized) && !(windowState() & Qt::WindowMinimized)) {
            fadeTo(1.0, 500);
        }
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_T) {
        toggleAlwaysOnTop();
        event->accept();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::setRoundedCorners()
{
    QRect roundedRect(0, 0, this->width(), this->height());
    QRegion region(roundedRect, QRegion::Rectangle);
    // TL ::
    region -= QRegion(0, 0, 20, 20, QRegion::Rectangle);
    region += QRegion(0, 0, 40, 40, QRegion::Ellipse);

    // TR ::
    region -= QRegion(680, 0, 20, 20, QRegion::Rectangle);
    region += QRegion(660, 0, 40, 40, QRegion::Ellipse);

    // BL ::
    region -= QRegion(0, 780, 20, 20, QRegion::Rectangle);
    region += QRegion(0, 760, 40, 40, QRegion::Ellipse);


    // BR ::
    region -= QRegion(680, 780, 20, 20, QRegion::Rectangle);
    region += QRegion(660, 760, 40, 40, QRegion::Ellipse);
    setMask(region);
}

void MainWindow::fadeTo(const float target, const uint16_t duration)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(duration);
    animation->setStartValue(windowOpacity());
    animation->setEndValue(target);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::fadeIn()
{
    QPropertyAnimation *fadeInAnimation = new QPropertyAnimation(this, "windowOpacity");
    fadeInAnimation->setDuration(250); // Adjust the duration as needed
    setWindowOpacity(1);
    setEnabled(true);
    fadeInAnimation->setStartValue(0.0);
    fadeInAnimation->setEndValue(1.0);
    fadeInAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::minimizeWindow()
{
    fadeTo(0.0, 250);
    QTimer::singleShot(500, this, &QWidget::showMinimized);
}

void MainWindow::exit()
{
    // Create a fade-out animation for the main window
    QPropertyAnimation* fadeOutAnimation = new QPropertyAnimation(this, "windowOpacity");
    fadeOutAnimation->setDuration(250); // Adjust the duration as needed
    fadeOutAnimation->setStartValue(1.0);
    fadeOutAnimation->setEndValue(0.0);

    // Connect the finished signal of the fade-out animation to a slot that quits the application
    connect(fadeOutAnimation, &QPropertyAnimation::finished, this, &MainWindow::trueExit);
    // Start the fade-out animation
    fadeOutAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::trueExit()
{
    this->close();
    qApp->quit();
}

void MainWindow::togglePage()
{
    int i = pager->currentIndex();
    if (i == 0) {
        pager->setCurrentIndex(1);
    } else {
        pager->setCurrentIndex(0);
    }
}

void MainWindow::handleJsonData(QStringList &osuInfo)
{
    beatmapInfo = osuInfo;
    mapLink = beatmapInfo.at(8);
    starStr = beatmapInfo.at(1);

    QString bgPath;
    // TODO check if bg image even exists, sometimes mapper forgot to put bg image in folder
    // if (!mapInfo.at(7).isEmpty() || image exists) << for example try that.
    if (!beatmapInfo.at(7).isEmpty()) {
        bgPath = QString("%1\\Songs\\%2\\%3").arg(beatmapInfo.at(0), beatmapInfo.at(5), beatmapInfo.at(7));
    } else {
        bgPath = "res\\noBG.png";
    }

    QList<QWidget*> widgets;
    if (prevPath != bgPath) {
        widgets << beatmapBG << starRect << diffTitle << mapTitle << maniaIcon << kpsFrame;
    } else {
        widgets << dummy1 << starRect << diffTitle << mapTitle << maniaIcon << kpsFrame;
    }

    QList<QPropertyAnimation*> animations;
    foreach (QWidget* widget, widgets)
    {
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(widget);
        widget->setGraphicsEffect(effect);

        QPropertyAnimation* fadeOutAnimation = new QPropertyAnimation(effect, "opacity");
        fadeOutAnimation->setDuration(250); // Duration of the fade-out animation in milliseconds
        fadeOutAnimation->setStartValue(1.0);
        fadeOutAnimation->setEndValue(0.0);
        fadeOutAnimation->setEasingCurve(QEasingCurve::InOutQuad);

        QPropertyAnimation* fadeInAnimation = new QPropertyAnimation(effect, "opacity");
        fadeInAnimation->setDuration(200); // Duration of the fade-in animation in milliseconds
        fadeInAnimation->setStartValue(0.0);
        fadeInAnimation->setEndValue(1.0);
        fadeInAnimation->setEasingCurve(QEasingCurve::InOutQuad);

        animations << fadeOutAnimation << fadeInAnimation;
    }

    // difRect ::
    connect(animations.at(2), &QPropertyAnimation::finished, [=, this]() {
        animations.at(3)->start(QAbstractAnimation::DeleteWhenStopped);
    });

    // difName ::
    connect(animations.at(4), &QPropertyAnimation::finished, [=, this]() {
        animations.at(5)->start(QAbstractAnimation::DeleteWhenStopped);
    });

    // mapName ::
    connect(animations.at(6), &QPropertyAnimation::finished, [=, this]() {
        animations.at(7)->start(QAbstractAnimation::DeleteWhenStopped);
    });

    // xx ::
    connect(animations.at(8), &QPropertyAnimation::finished, [=, this]()
    {
        QString setIcon, color, starColor;
        change(setIcon, color, starColor);

        mapStar->setText(starStr);
        mapTitle->setText(osuInfo.at(2));
        diffTitle->setText(QString(" [%1K] : : %2").arg(beatmapInfo.at(4), beatmapInfo.at(3)));
        mapStar->setStyleSheet(QString("QLabel { color: #%1; }").arg(starColor));
        maniaIcon->setIcon(QIcon(QString("res\\mania\\%1.png").arg(setIcon)));
        starRect->setStyleSheet(QString("QFrame { background-color: #%1; border-radius: 10px; }").arg(color));
        animations.at(9)->start(QAbstractAnimation::DeleteWhenStopped);
    });

    // kpsFrame ::
    connect(animations.at(10), &QPropertyAnimation::finished, [=, this]() {
        animations.at(11)->start(QAbstractAnimation::DeleteWhenStopped);
    });

    QPixmap dimmedImage = trimImage(bgPath);
    if (prevPath != bgPath)
    {
        connect(animations.at(0), &QPropertyAnimation::finished, [=, this]()
        {
            beatmapBG->setPixmap(dimmedImage);
            animations.at(1)->start(QAbstractAnimation::DeleteWhenStopped);
        });
    }
    animations.at(2)->start(QAbstractAnimation::DeleteWhenStopped);
    animations.at(4)->start(QAbstractAnimation::DeleteWhenStopped);
    animations.at(6)->start(QAbstractAnimation::DeleteWhenStopped);
    animations.at(8)->start(QAbstractAnimation::DeleteWhenStopped);
    animations.at(10)->start(QAbstractAnimation::DeleteWhenStopped);
    if (prevPath != bgPath) {
        prevPath = bgPath;
        beatmapBG->resize(dimmedImage.size());
        animations.at(0)->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else {
        delete animations.at(0);
        delete animations.at(1);
    }
    firstMap = true;
    if (!handlerOn && buttonsOn) { runButton->setDisabled(false); }
}

void MainWindow::change(QString& a, QString& b, QString& c)
{
    double dif = starStr.toDouble();
    if      (dif < 1.50) { a = "0.00"; b = "ffffff"; }
    else if (dif < 2.00) { a = "1.50"; b = "4ebfff"; }
    else if (dif < 2.30) { a = "2.00"; b = "4ef4db"; }
    else if (dif < 2.80) { a = "2.30"; b = "70ff73"; }
    else if (dif < 3.30) { a = "2.80"; b = "b0f854"; }
    else if (dif < 3.75) { a = "3.30"; b = "f6ee5c"; }
    else if (dif < 4.20) { a = "3.75"; b = "f9c560"; }
    else if (dif < 4.60) { a = "4.20"; b = "fc9e65"; }
    else if (dif < 5.10) { a = "4.60"; b = "ff7a69"; }
    else if (dif < 5.60) { a = "5.10"; b = "ff666b"; }
    else if (dif < 6.00) { a = "5.60"; b = "ff516e"; }
    else if (dif < 6.40) { a = "6.00"; b = "ff3c70"; }
    else if (dif < 6.80) { a = "6.40"; b = "bb4ca0"; }
    else if (dif < 7.30) { a = "6.80"; b = "775ed1"; }
    else if (dif < 7.75) { a = "7.30"; b = "4c49c3"; }
    else if (dif < 8.00) { a = "7.75"; b = "2a27a1"; }
    else                 { a = "8.00"; b = "000000"; }
    c = dif < 6.80 ? "000000" : "dfbe58";
}

void MainWindow::handleWebSocketError(const QString &errorMessage)
{
    QMessageBox msgBox;
    msgBox.setText(errorMessage);
    msgBox.setWindowFlags(Qt::CustomizeWindowHint);
    msgBox.setIconPixmap(QPixmap("res\\icon.ico"));
    msgBox.setStyleSheet("QLabel { color: #ffc1db; }"
                         "QMessageBox { background-color: #111011; }"
                         "QPushButton { background-color: #211011; color: #ffc1db; }");
    msgBox.exec();
    exit();
}

void MainWindow::changePlayStatus(const bool inPlay)
{
    QPropertyAnimation* animation = new QPropertyAnimation(playBoxOpacityEffect, "opacity");
    animation->setDuration(1000);
    if (inPlay)
    {
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
    }
    else
    {
        animation->setStartValue(1.0);
        animation->setEndValue(0.0);
        if (ArduinoOn) { emit terminateArduino(); }
    }
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::keystrokesMake(const uint16_t KS)
{
    if (bC_KPS)
    {
        kpsField->setText(QString("%1").arg((KS & 0x3FC00) >> 10, 2, 10, QChar('0')));
    }
    if (!bC_KS) { return; }

    // Shift values up :: - -
    for (uint8_t x = 0; x < 28; ++x) {
        for (uint8_t y = 0; y < 128; y += 2) {
            bitmap[x][y+1] = bitmap[x][y+2];
            bitmap[x][y]   = bitmap[x][y+1];
        }
    }
    // Add new Keystroke values :: - -
    uint8_t x = 0, stopX;
    for (uint16_t i = 2; i <= 128; i <<= 1)
    {
        stopX = x + 4;
        for (; x < stopX; ++x)
        {
            bitmap[x][127] = KS & i;
            bitmap[x][126] = KS & i;
        }
    }
    // Create pixmap :: - -
    QImage image(28, 128, QImage::Format_ARGB32);
    for (uint8_t x = 0; x < 28; ++x)
    {
        for (uint32_t y = 0; y < 128; ++y)
        {
            QRgb color = bitmap[x][y] ? 0x00FFC0CB + (y << 25) : 0;
            image.setPixel(x, y, color);
        }
    }
    keystrokes->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::makeBitmap()
{
    for (int x = 0; x < 28; ++x) {
        for (int y = 0; y < 128; ++y) {
            bitmap[x][y] = 0;
        }
    }
    keystrokesMake(0);
    kpsField->setText("00");
}

void MainWindow::stopWorkerThread()
{
    workerThread->quit();
    workerThread->wait();
    ArduinoOn = false;
    handlerOn = false;
    msgBox->append(" : : Stopped.");
    runButton->setDisabled(false);
    termButton->setDisabled(true);
}
