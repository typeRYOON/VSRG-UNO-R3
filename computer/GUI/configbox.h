/* configbox.h */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: configbox.h :: VSRG-UNO-R3: ConfigBox QWidget
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> Header file, check implementation file for detailed class explanation.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#ifndef CONFIGBOX_H
#define CONFIGBOX_H
#include "../Utils/shorthand.h"
#include "../Utils/utils.h"
#include <QStackedLayout>
#include <QLineEdit>
using QSL = QStackedLayout;

class ConfigBox : public HF {
    Q_OBJECT

signals:
    void newConfig ( const quint8& );
    void portChange( const quint8&, const QS& );

public:
    explicit ConfigBox( Utils*, QObject* = nullptr );

public slots:
    void changeStack( const quint8& );

private:
    Utils* u;
    QPB*   makeConfigButton();
    void   evalLineEdit( const quint8&, const QS& );
    void   makeButtonConnections( const QVector<QPB*>& );
    void   handleClick( QPB*, const quint8&, QPB* = nullptr );
    QF*    makeConfigLayer();
    QF*    makeArduinoLayer();
    QF*    makeCreditsLayer();
    quint8 config = 0x0;
    QSL*   stack;
    QL*    kps_ql;
    QL*    ks_ql;
};
#endif // CONFIGBOX_H :: - -                                                           - -
