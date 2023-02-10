#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_aria2Launcher.h"
#include "about.h"
#include "windows.h"
#include <TlHelp32.h>
#include <QWindow>
#include <QMessagebox>
#include <QLabel>
#include <QActionGroup>
#include <QSystemTrayIcon>
#include <QCloseEvent>


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
    void trayInitialize();
    void closeEvent(QCloseEvent* event);
private:
    qint64 pid;
    QWidget* aria2CmdWidget;
    QCloseEvent* event;
private:
    Ui::aria2LauncherClass* ui;
    about* aboutWin = new about(this);
private slots:
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void showOrHide();
};


