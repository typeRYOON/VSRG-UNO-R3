/* consolebox.h */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: consolebox.h :: VSRG-UNO-R3: ConsoleBox QWidget
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> Header file, check implementation file for detailed class explanation.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#ifndef CONSOLEBOX_H
#define CONSOLEBOX_H
#include "../Utils/utils.h"

class ConsoleBox : public HF {
    Q_OBJECT

signals:
    void sendReplayPath( const QS& );

public:
    explicit ConsoleBox( Utils*, QObject* = nullptr );

public slots:
    void inPlayLabel( const bool& );
    void setErrCode( const QS& );
    void delay( const qint16& );
    void beatmapChange(
        const QS&,
        const QS&,
        const QS&,
        const QS&,
        const QS&
    );
    void setObjectData(
        const QS&,
        const QS&,
        const QS&,
        const QS&
    );

private:
    Utils* u;

    // Changing QFrames :: - -                                                         - -
    QL* bgBox;
    QL* levelTitle_ql;
    QL* levelSub_ql;
    QL* objects_ql;
    QL* circles_ql;
    QL* sliders_ql;
    QL* stars_ql;
    QL* length_ql;
    QL* creator_ql;
    QL* errorCode_ql;
    QL* cat_ql;
    QPA* inPlay_a;
    QPA* term_a;
    QTimer* termTimer;
};
#endif // CONSOLEBOX_H :: - -                                                          - -
