/* historybox.h */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: historybox.h :: VSRG-UNO-R3: HistoryBox QWidget
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> Header file, check implementation file for detailed class explanation.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#ifndef HISTORYBOX_H
#define HISTORYBOX_H
#include "../Utils/utils.h"

using HistoryNode =
    std::tuple<QPixmap, QPixmap, QPair<QS, QS>, quint8>;

class HistoryBox : public HF {
    Q_OBJECT

public:
    explicit HistoryBox( Utils*, QObject* = nullptr );

public slots:
    void saveHistoryNodes();
    void clearHistory();
    void addHistoryNode(
        const QPixmap&,
        const QPixmap&,
        const QPair<QS, QS>&,
        const quint8 &
    );

private:
    Utils* u;
    bool loadHistoryNodes();
    void setFrameStyle();
    void deleteFrame();
    QF*  makeHistoryNode(
        const QPixmap&,
        const QPixmap&,
        const QPair<QS, QS>&,
        const quint8 &
    );
    QList<HistoryNode> nodes;
    QList<QF*>         frames;
    QFont              title_qf, sub_qf;
    bool               afterLoad = false;
    QVL*               history;
};
#endif // HISTORYBOX_H :: - -                                                          - -
