﻿#include "aria2Launcher.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);
    aria2Launcher w;
    return a.exec();
}
