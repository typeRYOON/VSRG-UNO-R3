/* hoverframe.h */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: hoverframe.h :: VSRG-UNO-R3: Color Shifting QFrame
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> Header file, check implementation file for detailed class explanation.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#ifndef HOVERFRAME_H
#define HOVERFRAME_H
#include <QColor>
#include <QEvent>
#include <QFrame>
#include <QTimer>

class HoverFrame : public QFrame {
    Q_OBJECT

signals:
    void clicked();

protected:
    void enterEvent( QEnterEvent* )      override;
    void leaveEvent( QEvent* )           override;
    void mousePressEvent( QMouseEvent* ) override;

public:
    HoverFrame( QWidget* = nullptr, const bool& = false );

private:
    void startColorAnimation();

    QTimer* animationTimer;
    QColor  buttonColor;
    bool    hover = false;
    bool    click;

private slots:
    void updateColor();
};
#endif // HOVERFRAME_H :: - -                                                          - -
