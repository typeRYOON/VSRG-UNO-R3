#include <QApplication>
#include <QProcess>
#include <QDir>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QProcess gosu;
    gosu.start("res\\gosumemory.exe");
    QObject::connect(&a, &QCoreApplication::aboutToQuit, [&gosu]() {
        if (gosu.state() == QProcess::Running)
            gosu.terminate();
    });
    MainWindow w;
    w.setWindowOpacity(0);
    w.setEnabled(false);
    return a.exec();
}
