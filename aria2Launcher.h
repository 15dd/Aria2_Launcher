#pragma once

#include <iostream>
#include "ui_aria2Launcher.h"
#include "about.h"
#include "settingWindow.h"
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <QWindow>
#include <QMessagebox>
#include <QLabel>
#include <QActionGroup>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QFileInfo>


class aria2Launcher : public QMainWindow
{
    Q_OBJECT

public:
    aria2Launcher(QWidget *parent = nullptr);
    ~aria2Launcher();

    qint64 startProcess(QString cmd);
    BOOL KillProcess(DWORD ProcessId);
    WId getProcessWId(qint64 pid);

    void checkAria2Status();
    void start();
    void trayInitialize();
    void checkFile();
    void closeEvent(QCloseEvent* event);
    bool quitApp();
    DWORD FindProcessIDByName(const std::string& processName);
private:
    qint64 pid;
    QWidget* aria2CmdWidget;
    QCloseEvent* event;
    QSystemTrayIcon* trayIcon;
private:
    Ui::aria2LauncherClass* ui;
    about* aboutWin = new about(this);
    setting* settingWin = new setting(this);
private slots:
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void showOrHide();
    void showWindowsMessage();
    void showHide();
};


