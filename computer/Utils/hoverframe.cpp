/* hoverframe.cpp */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: hoverframe.cpp :: VSRG-UNO-R3: Color Shifting QFrame
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> This file's class object is a QFrame derived QWidget. When the user hovers over it,
 >> it shifts the color, and shifts back to the original when not hovering over it.
 >> There is a clickable option that allows for a connectable signal to be emitted.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#include "HoverFrame.h"
#include "qevent.h"

// Constructor initial settings :: - -                                                 - -
HoverFrame::HoverFrame( QWidget* p, const bool& click ) : QFrame( p ), click( click )
{
    setObjectName( "HF" );
    setStyleSheet(
        "#HF { border-radius: 16px;"
        "border: 0px solid transparent;"
        "background-color: #1f1e1f; }"
    );
    animationTimer = new QTimer( this );
    connect(
        animationTimer, &QTimer    ::timeout,
        this,           &HoverFrame::updateColor );
    if ( click ) { setCursor( Qt::PointingHandCursor ); }
}

// Start hovering over frame :: - -                                                    - -
void HoverFrame::enterEvent( QEnterEvent* e )
{
    hover = true;
    QFrame::enterEvent( e );
    startColorAnimation();
}

// Stop hovering over frame :: - -                                                     - -
void HoverFrame::leaveEvent( QEvent* e )
{
    hover = false;
    QFrame::leaveEvent( e );
    startColorAnimation();
}

// On click emit "clicked" signal :: - -                                               - -
void HoverFrame::mousePressEvent( QMouseEvent* e )
{
    if ( click && e->button() == Qt::LeftButton ) { emit clicked(); }
    QFrame::mousePressEvent( e );
}

// Color update cycle function :: - -                                                  - -
void HoverFrame::updateColor()
{
    if ( hover ) {
        buttonColor = buttonColor.darker( 103 );
        if ( buttonColor.red() <= 24 ) {
            animationTimer->stop();
        }
    } else
    {
        buttonColor = buttonColor.lighter( 103 );
        if ( buttonColor.red() >= 30 ) {
            animationTimer->stop();
        }
    }
    setStyleSheet(
        QString(
            "#HF { border-radius: 16px;"
            "border: 0px solid transparent;"
            "background-color: %1; }"
        ).arg( buttonColor.name() )
    );
}

// Animation cycle starter :: - -                                                      - -
void HoverFrame::startColorAnimation()
{
    buttonColor = palette().color( QPalette::Button );
    animationTimer->start( 30 );
}   // :: - -                                                                          - -
