/* mainwindow.h */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: mainwindow.h :: VSRG-UNO-R3: MainWindow QMainWindow
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> Header file, check implementation file for detailed class explanation.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "../Osu/osuhandler.h"
#include "../Utils/osumem.h"
#include "consolebox.h"
#include "historybox.h"
#include "configbox.h"
#include "imgbox.h"
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT

signals:
    void saveHistoryNodes();

public:
    MainWindow( QWidget* = nullptr );
    ~MainWindow() = default;

public slots:
    void minimize();

protected:
    void mousePressEvent( QMouseEvent* )   override;
    void mouseMoveEvent( QMouseEvent* )    override;
    void mouseReleaseEvent( QMouseEvent* ) override;
    void keyPressEvent( QKeyEvent* )       override;
    void changeEvent( QEvent* )            override;

private:
    void setMainWindow();
    QF * createMainWidget();
    QPA* fadeTo(
        const float&,
        const quint16&,
        const QEasingCurve& = QEasingCurve::Linear
    );
    bool   m_isAlwaysOnTop = false;
    QPoint dragPosition;

    // All custom GUI objects :: - -                                                   - -
    Utils     * utils     ;
    OsuMem    * osumem    ;
    OsuHandler* osuhandler;
    ImgBox    * imgbox    ;
    ConfigBox * configbox ;
    ConsoleBox* consolebox;
    HistoryBox* historybox;
};
#endif // MAINWINDOW_H :: - -                                                          - -
