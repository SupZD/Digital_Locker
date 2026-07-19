#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>
#include <QSplashScreen>
#include <QPixmap>
#include <QTimer>
#include <QEventLoop>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));

    QPixmap splashPixmap(":/resources/splash.png");
    splashPixmap = splashPixmap.scaled(440, 380, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QSplashScreen splash(splashPixmap);
    splash.show();
    a.processEvents();

    QEventLoop loop;
    QTimer::singleShot(4000, &loop, &QEventLoop::quit);
    loop.exec();

    MainWindow w;
    splash.finish(&w);
    w.show();

    return QApplication::exec();
}
