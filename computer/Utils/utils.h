/* utils.h */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: utils.h :: VSRG-UNO-R3: Utility Object
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> Header file, check implementation file for detailed class explanation.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#ifndef UTILS_H
#define UTILS_H
#include "shorthand.h"
#include <QFontDatabase>
#include <QPixmap>
#include <QFont>

class Utils {
public:
    Utils();
    QPixmap roundScaleImage(
        const QString & path,
        const quint8  & radius,
        const quint16 & width,
        const quint16 & height,
        const float   & opacity,
        const quint8  & index
    );
    QFont  getFont( const quint8& );
    QDS_E* makeDropShadow( const QColor&, QObject*, const quint8& = 10 );
    QF   * makeVLine( const QSize&, const QColor&, QObject* );

private:
    QString fontFamily = "Arial";
};
#endif // UTILS_H :: - -                                                               - -
