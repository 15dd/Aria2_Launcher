#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_aria2Launcher.h"
#include "windows.h"
#include <TlHelp32.h>
#include <QWindow>
#include <QMessagebox>
#include <QLabel>
#include <QActionGroup>
#include <QSystemTrayIcon>
#include <QDir>
#include <QSettings>


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
    void showOrHide();

    //DWORD pid;
    qint64 pid;
    QWidget* aria2CmdWidget;
    int n = 1;
private:
    Ui::aria2LauncherClass ui;

private:
    QSystemTrayIcon* trayIcon;
    QAction* SOH;
    QAction* Close;
    QMenu* Menu;

private slots:
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
};


