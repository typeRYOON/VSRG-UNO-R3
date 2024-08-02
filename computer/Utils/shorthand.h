/* shorthand.h */
/* ---------------------------------------------------------------------------------------
 >> Assignment details are created and owned by Patrick Troy.
 >> University of Illinois at Chicago - CS 362, Spring 2024
 >> --------------------------------------------------------------------------------------
 >> File    :: shorthand.h :: VSRG-UNO-R3: Short Type Names Header File
 >> Course  :: CS 362 (47019), SP24
 >> Author  :: Ryan Magdaleno
 >> UIN/nID :: 668523658 (rmagd2)
 >> System  :: Windows 10 w/ Mingw-w64
 - -                               - -
 >> File overview ::
 >> Sets abbreviated type names for long/common types found in this program.
--------------------------------------------------------------------------------------- */
// Preprocessor directives ::
#ifndef SHORTHAND_H
#define SHORTHAND_H
#include "../Utils/hoverframe.h"
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>

// QLayout types :: - -                                                                 - -
using QGL   = QGridLayout;
using QHL   = QHBoxLayout;
using QVL   = QVBoxLayout;

// QWidget types :: - -                                                                 - -
using QPB   = QPushButton;
using HF    = HoverFrame;
using QL    = QLabel;
using QF    = QFrame;

// Graphics / Animation types :: - -                                                    - -
using QDS_E = QGraphicsDropShadowEffect;
using QO_E  = QGraphicsOpacityEffect;
using QPA   = QPropertyAnimation;

// Other types :: - -                                                                   - -
using QS    = QString;

#endif // SHORTHAND_H :: - -                                                            - -
