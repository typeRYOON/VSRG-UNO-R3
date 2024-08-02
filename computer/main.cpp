#include "GUI/mainwindow.h"
#include <QApplication>
#include <QProcess>

// Entrypoint :: - -                                                                   - -
int main( int argc, char* argv[] )
{
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::Unset
    );
    QApplication a( argc, argv );
    QProcess gosu;
    gosu.start( "res/gosumemory.exe" );
    QObject::connect( &a, &QCoreApplication::aboutToQuit, &gosu, [&gosu]() {
        if ( gosu.state() == QProcess::Running ) {
             gosu.kill();
        }
    } );

    MainWindow w;
    w.setWindowOpacity( 0 );
    return a.exec();
}   // :: - -                                                                          - -
