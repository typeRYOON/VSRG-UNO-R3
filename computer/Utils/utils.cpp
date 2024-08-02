/* utils.cpp */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: utils.cpp :: VSRG-UNO-R3: Utility Object
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> This file's class object is a utility object that sets up tedious things.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#include "utils.h"
#include <QPainterPath>
#include <QPainter>

// Constructor, set up program font :: - -                                             - -
Utils::Utils()
{
    const qint32 fontID = QFontDatabase::addApplicationFont(
        "res/Hiragino Maru Gothic ProN W4.otf"
    );
    if ( fontID == -1 ) { return; }
    fontFamily = QFontDatabase::applicationFontFamilies( fontID ).at(0);
}

// Round corner, set w/h, return pixmap :: - -                                         - -
QPixmap Utils::roundScaleImage(
    const QString & path,
    const quint8  & radius,
    const quint16 & width,
    const quint16 & height,
    const float   & opacity,
    const quint8  & index
) { // Transform base image to given width and height :: - -                           - -
    QPixmap base( path );
    uint16_t scalar = base.width();
    scalar /= width / height;

    // Index 1: imagebox image :: - -                                                  - -
    if ( index == 1 ) {
        base = base.copy( 0, base.height() / 2 - scalar / 2, base.width(), scalar ).scaled(
            width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation
        );
    } else
    {   // Square style :: - -                                                         - -
        QRect captureRect;
        if ( base.width() > base.height() ) {
            quint32 sideLength = base.height();
            captureRect
                = QRect( ( base.width() - sideLength ) / 2, 0, sideLength, sideLength );
        } else {
            quint32 sideLength = base.width();;
            captureRect
                = QRect( 0, ( base.height() - sideLength ) / 2, sideLength, sideLength );
        }
        base = base.copy( captureRect ).scaled(
            width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation
        );
    }
    // Create a opacity mask :: - -                                                    - -
    QPixmap final( base.size() );
    final.fill( Qt::transparent );

    QPainter painter( &final );
    painter.setOpacity( opacity );

    // Combine Pixmaps and clip rounded corners :: - -                                 - -
    QPainterPath clipPath;
    QRectF rect = final.rect();

    switch (index) {
    case 1:
        // Start from the top-left corner
        clipPath.moveTo( rect.left() + radius, rect.top() );
        clipPath.arcTo(
            rect.left(), rect.top() , radius * 2, radius * 2, 90, 90
        );
        clipPath.closeSubpath();

        // Top-right corner
        clipPath.lineTo( rect.right() - radius, rect.top() );
        clipPath.lineTo( rect.right() - radius, rect.top() );
        clipPath.arcTo(
            rect.right() - radius * 2, rect.top(), radius * 2, radius * 2, 0, 90
        );
        clipPath.lineTo( rect.right(), rect.top() + radius );

        // // Bottom-right corner (straight line)
        clipPath.lineTo( rect.right(), rect.bottom() );

        // // Bottom-left corner (straight line)
        clipPath.lineTo( rect.left(), rect.bottom() );
        clipPath.lineTo( rect.left(), rect.top() + radius );
        clipPath.closeSubpath();
        break;

    default: // All four corners rounded :: - -                                        - -
        clipPath.addRoundedRect( rect, radius, radius );
        break;
    }
    painter.setOpacity( opacity );
    painter.setClipPath( clipPath );
    painter.drawPixmap( 0, 0, base );
    painter.setOpacity( 1 );

    // Set up colored gradients for imageBox :: - -                                    - -
    if ( index == 1 )
    {
        QLinearGradient gradient_w( 0, height - 80, 0, height );
        gradient_w.setColorAt( 0, QColor( 255, 255, 255,  0 ) );
        gradient_w.setColorAt( 1, QColor( 125, 125, 255, 50 ) );
        painter.fillRect( 0, height - 80, width, height - radius, gradient_w );

        QLinearGradient gradient( 0, height - 155, 0, height );
        gradient.setColorAt( 0, QColor( 0, 0, 0,   0 ) );
        gradient.setColorAt( 1, QColor( 0, 0, 0, 155 ) );
        painter.fillRect( 0, height - 155, width, height - radius, gradient );
    }
    painter.end();
    return final;
}

// Retrieve font with passed size :: - -                                               - -
QFont Utils::getFont( const quint8& pointSize ) {
    return QFont( fontFamily, pointSize, QFont::Bold );
}

// Drop shadow maker :: - -                                                            - -
QDS_E* Utils::makeDropShadow( const QColor& color, QObject* p, const quint8& r )
{
    QDS_E* ret = new QDS_E( p );
    ret->setBlurRadius( r );
    ret->setOffset( 0, 0 );
    ret->setColor( color );
    return ret;
}

QF* Utils::makeVLine( const QSize& size, const QColor& color, QObject* p )
{
    QF* ret = new QF();
    ret->setFixedSize( size );
    ret->setStyleSheet( "background-color: white; border: 0px;" );
    ret->setGraphicsEffect(
        makeDropShadow( QColor( color ), p )
    );
    return ret;
}   // :: - -                                                                          - -
