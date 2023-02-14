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
#include <QWebEngineView>

class aria2Launcher : public QMainWindow
{
    Q_OBJECT

public:
    aria2Launcher(QWidget *parent = nullptr);
    ~aria2Launcher();

    qint64 startProcess(QString cmd); //执行cmd命令
    BOOL KillProcess(DWORD ProcessId); //kill
    WId getProcessWId(qint64 pid); // qint64 to WId

    void checkAria2Status(); //检测是否已经有aria2c运行,可选择新建进程或kill已存在的进程再新建进程
    void Start(); //启动aria2c.exe,并嵌入窗口中
    void uiInitialize();//ui初始化
    void checkFile();//检测所需文件是否存在
    void closeEvent(QCloseEvent* event);//关闭事件
    bool quitApp();//退出询问
    DWORD FindProcessIDByName(const std::string& processName); //寻找aria2c.exe的pid
private:
    qint64 pid; //pid
    QWidget* aria2CmdWidget; //截获的aria2c.exe窗口
    QCloseEvent* event; //关闭事件
    QSystemTrayIcon* trayIcon; //托盘
    QWebEngineView* view = NULL;
private:
    Ui::aria2LauncherClass* ui;
    about* aboutWin = new about(this); //关于窗口
    setting* settingWin = new setting(this); //设置窗口
private slots:
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason); //单击托盘图标事件
    void showOrHide(); //显示主窗口或隐藏主窗口
    void showWindowsMessage(); //通知横幅，提示程序已进入托盘
    void showHide(); //根据设置决定是显示窗口还是启动到托盘
    void showCmd();//显示cmd
    void showWebui();//显示webui
    void showWindow();//决定开屏显示webui或cmd
};


