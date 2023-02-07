#include "aria2Launcher.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{   
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication a(argc, argv);
    aria2Launcher w;
    w.show();
    return a.exec();
}
