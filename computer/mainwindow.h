#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QEvent>
#include <QWindowStateChangeEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPropertyAnimation>
#include <QPainterPath>
#include <QRegion>
#include <QTimer>
#include <QStackedWidget>
#include <QComboBox>
#include <QDesktopServices>
#include <QUrl>
#include <QGraphicsOpacityEffect>
#include <QCheckBox>
#include <QThread>
#include "arduinohandler.h"
#include "websocketmonitor.h"
#include <QCoreApplication>
#include <QTextEdit>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    WebSocketMonitor* webSocket;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void changeEvent(QEvent*) override;
    void keyPressEvent(QKeyEvent* event) override;

signals:
    void sendVirtualKeys(
        const QVector<uint8_t>&,
        const QVector<uint8_t>&,
        const QVector<uint8_t>&,
        const QVector<uint8_t>&
    );
    void sendCurOsu(const QString&);
    void terminateArduino();


private:
    QStringList beatmapInfo;
    QPushButton* exitButtonMain;
    QPushButton* minimizeButtonMain;
    QPushButton* settingsButtonMain;
    QPushButton* exitButtonSett;
    QPushButton* minimizeButtonSett;
    QPushButton* settingsButtonSett;
    QPoint dragPosition;
    QLabel* beatmapBG;

    QGraphicsOpacityEffect* playBoxOpacityEffect;
    QPushButton* playBox;
    QLabel* mapTitle;
    QLabel* diffTitle;
    QLabel* mapStar;
    QPushButton* maniaIcon;

    QWidget *mainPage;
    QWidget *settPage;

    QVBoxLayout* mainLayout;
    QVBoxLayout* settLayout;
    QStackedWidget *pager;
    QString mapLink;
    QString starStr;

    QPixmap trimImage(const QString&);
    void setRoundedCorners();
    void fadeTo(const float, const uint16_t);
    void fadeIn();
    void change(QString&, QString&, QString&);

    QPushButton* makeButton(
        const QString&,
        const int,
        const int,
        const bool
    );




    QFrame* starRect;
    QString prevPath;
    QWidget *dummy1, *dummy2;

    // Main unique ::
    QPushButton* termButton;
    QPushButton* runButton;
    QLabel* row3TextA;
    QLabel* row3TextB;

    QPushButton* A_KS;
    QPushButton* A_KPS;
    QPushButton* A_AP;
    QPushButton* C_KS;
    QPushButton* C_KPS;
    QPushButton* C_AP;

    bool bA_KS  = false;
    bool bA_KPS = false;
    bool bA_AP  = false;
    bool bC_KS  = false;
    bool bC_KPS = false;
    bool bC_AP  = false;


    QLabel* keystrokes;
    uchar bitmap[28][128];

    QLabel* kpsField;
    QFrame* kpsFrame;

    QTimer* timer;

    QThread*  workerThread;
    ArduinoHandler* worker;
    void startProcessing();
    void setKeys(
        const QString&,
        const QString&,
        const QString&,
        const QString&
    );
    bool checkConfig = true;

    void toggleAlwaysOnTop();
    void isAlwaysOnTop();
    bool m_isAlwaysOnTop;

    bool ArduinoOn = false;


    QLabel* validPortR1;
    QLabel* validPortR2;
    QLineEdit* KS_KPS_PortField;
    QLineEdit* AU_PortField;
    void checkPorts();
    QString AU_PortName     = "-1";
    QString KPS_KS_PortName = "-1";


    QTextEdit* msgBox;

    int8_t buttonsOn = 0;
    void checkButtonCount(const int8_t&);
    bool firstMap = false;

    QWidget* scrollWidget;
    QLayout* createScrollWidget(const QString&, const bool&);
    QVBoxLayout* scrollVLayout;

    QFont scrollMapFont;
    QFont scrollDiffFont;
    QFont scrollStarFont;

    bool handlerOn = false;


private slots:
    void minimizeWindow();
    void exit();
    void trueExit();
    void togglePage();
    void handleJsonData(QStringList &osuInfo);
    void handleWebSocketError(const QString& errorMessage);
    void changePlayStatus(const bool);
    void keystrokesMake(const uint16_t);
    void makeBitmap();
    void stopWorkerThread();
    void readOsuConfig(const QString&);
    void arduinoIsOn();
    void receiveMsg(const QString&);


};
#endif // MAINWINDOW_H
