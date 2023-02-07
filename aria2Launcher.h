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
#include <QProcess>

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
    void showCmd();

    //DWORD pid;
    qint64 pid;
    QWebEngineView* view;
    QWidget* aria2CmdWidget;
private:
    Ui::aria2LauncherClass ui;
};


