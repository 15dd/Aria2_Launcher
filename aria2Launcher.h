#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_aria2Launcher.h"
#include "windows.h"
#include <TlHelp32.h>
#include <QWindow>
#include <QMessagebox>
#include <QLabel>
#include <QActionGroup>
#include <QWebengineview>

class aria2Launcher : public QMainWindow
{
    Q_OBJECT

public:
    aria2Launcher(QWidget *parent = nullptr);
    ~aria2Launcher();

    qint64 startProcess(QString cmd);
    BOOL KillProcess(DWORD ProcessId);

    WId getProcessWId(qint64 pid);

    void startAria2();
    void menuInitialize();
    void showHtml();

    DWORD pid;
    QWebEngineView* view;
private:
    Ui::aria2LauncherClass ui;
};


