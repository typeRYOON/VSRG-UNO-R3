/* imgbox.h */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: imgbox.h :: VSRG-UNO-R3: ImgBox QWidget
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> Header file, check implementation file for detailed class explanation.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#ifndef IMGBOX_H
#define IMGBOX_H
#include "../Utils/qcustomplot.h"
#include "../Utils/utils.h"

class ImgBox : public QF {
    Q_OBJECT

signals:
    void changeStack( const quint8& );
    void minus();
    void exit();

public:
    ImgBox( Utils*, QWidget* = nullptr );


public slots:
    void resetPaging( const quint8& );
    void opacityGraph( const bool& );
    void osuHandlerSet();
    void trailStop();
    void changeData(
        const QS&,
        const QS&,
        const QS&,
        const QS&
    );
    void updateData(
        const quint8&,
        const quint8&
    );

private:
    Utils* u;
    bool   osuHandlerRun = false;
    QL*    mapTitle;
    QL*    subTitle;
    QL*    keyFrame;

    // Fading bg image :: - -                                                          - -
    bool  BisVisible = false;
    QL*   beatmapBG_A;
    QL*   beatmapBG_B;
    QPA*  out_a;
    QPA*  in_a;

    // Text + cipher :: - -                                                            - -
    QS      subTitle_target_s, mapTitle_target_s;
    QS      mapTitle_s = "VSRG-UNO-R3。";
    QS      subTitle_s = "「Build v2.0」";
    QS      key;
    QTimer* cipherTimer;
    quint8  mapCipherLen, subCipherLen;
    quint8  mpDel_d, mpDel_u;
    quint8  sbDel_d, sbDel_u;
    quint8  step;
    bool    cipherMap = false, cipherSub = false;

    // Moving selector :: - -                                                          - -
    QMap<QPB*, quint8> buttonPos;
    QPB*               pressedButton;
    QPA*               scroll_a;
    QRect              origin;
    QF*                selected;
    QPB*               selectMania; // origin button

    // Background graph :: - -                                                         - -
    void            makeGraph( QGL* );
    QVector<double> xData, yData;
    QCustomPlot*    plot;
    QCPGraph*       graph;
    QPA*            plotOpacity_a;
    QL*             keystrokesBox;
    QL*             kpsBox;
    uchar           bitmap[28][128];

private slots:
    void cipherText();
    void buttonPress( QPB* );
    void resetData();
};
#endif // IMGBOX_H :: - -                                                              - -
