#include "aria2Launcher.h"

aria2Launcher::aria2Launcher(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    view = new QWebEngineView(this);
    view->hide();

    startAria2();
    menuInitialize();

    //QString path = QApplication::applicationDirPath() + "/yaaw/index.html";  //将html文件放入debug目录下
    //view->load(QUrl(path));
    
    //QString htmlPath = "D:/Download/yaaw/index.html";
    //view->setUrl(QUrl("file:///" + htmlPath));

    view->setUrl(QUrl("http://aria2c.com/"));


    ui.cmdPage->setChecked(true);
    ui.yaaw->setChecked(true);

    connect(ui.webuiPage, &QAction::triggered, this, &aria2Launcher::showHtml);
    connect(ui.cmdPage, &QAction::triggered, this, &aria2Launcher::showCmd);
}

aria2Launcher::~aria2Launcher()
{
    //system("taskkill /f /t /im cmd.exe");
    KillProcess(pid);
}

void aria2Launcher::showHtml() {
    centralWidget()->setParent(0);
    setCentralWidget(view);
    view->show();
}
//https://blog.csdn.net/weixin_43193739/article/details/116333157

void aria2Launcher::showCmd() {
    centralWidget()->setParent(0);
    setCentralWidget(aria2CmdWidget);
}


void aria2Launcher::menuInitialize() { //设置QActionGroup，使只能单选
    menuBar()->removeAction(ui.cmdPage);
    menuBar()->removeAction(ui.webuiPage);

    menuBar()->removeAction(ui.ariang);
    menuBar()->removeAction(ui.webui);
    menuBar()->removeAction(ui.yaaw);

    QActionGroup* switchPageGroup = new QActionGroup(this);
    ui.switchPage->addAction(switchPageGroup->addAction(ui.cmdPage));
    ui.switchPage->addAction(switchPageGroup->addAction(ui.webuiPage));

    QActionGroup* switchWebuiGroup = new QActionGroup(this);
    ui.switchWebui->addAction(switchWebuiGroup->addAction(ui.ariang));
    ui.switchWebui->addAction(switchWebuiGroup->addAction(ui.webui));
    ui.switchWebui->addAction(switchWebuiGroup->addAction(ui.yaaw));
}

void aria2Launcher::startAria2() { //启动aria2c.exe

    //pid = startProcess("aria2c --enable-rpc --rpc-allow-origin-all"); //启动aria2c.exe
    pid = startProcess("aria2c --conf-path=aria2.conf"); //启动aria2c.exe
    //pid = startProcess("aria2c.exe --conf-path='C:\\Users\\15726\\Desktop\\c++ test\\aria2Launcher\\x64\\Release\\aria2.conf'"); //启动aria2c.exe
    //HWND Hpid = (HWND)(QString::number(pid).toInt()); //将pid转为HWND类型的数据

    QLabel* showPid = new QLabel("pid:" + QString::number(pid), this); //状态栏显示aria2c.exe的pid
    ui.statusBar->addWidget(showPid);

    QWindow* aria2Cmd = QWindow::fromWinId(getProcessWId(pid)); //根据pid，将aria2c.exe的窗口嵌入至主窗口中
    aria2Cmd->setFlags(aria2Cmd->flags() | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    aria2CmdWidget = QWidget::createWindowContainer(aria2Cmd);
    this->setCentralWidget(aria2CmdWidget);

}

qint64 aria2Launcher::startProcess(QString cmd) //启动程序
{
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    bool bRet = CreateProcess(
        NULL,
        (LPWSTR)cmd.toStdWString().c_str(),
        NULL,
        NULL,
        FALSE,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL, &si, &pi);

    Sleep(500);
    return (qint64)pi.dwProcessId;

}

BOOL aria2Launcher::KillProcess(DWORD ProcessId) //根据pid杀死创建
{
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessId);
    if (hProcess == NULL)
        return FALSE;
    if (!TerminateProcess(hProcess, 0))
        return FALSE;
    return TRUE;
}
// https://blog.csdn.net/jfkidear/article/details/27057861


//qint64转WId------------------------------------------------------------------
typedef struct EnumHWndsArg
{
    std::vector<HWND>* vecHWnds;
    DWORD dwProcessId;
}EnumHWndsArg, * LPEnumHWndsArg;


BOOL CALLBACK lpEnumFunc(HWND hwnd, LPARAM lParam)
{
    EnumHWndsArg* pArg = (LPEnumHWndsArg)lParam;
    DWORD  processId;
    GetWindowThreadProcessId(hwnd, &processId);
    if (processId == pArg->dwProcessId)
    {
        pArg->vecHWnds->push_back(hwnd);
    }
    return TRUE;
}

void GetHWndsByProcessID(DWORD processID, std::vector<HWND>& vecHWnds)
{
    EnumHWndsArg wi;
    wi.dwProcessId = processID;
    wi.vecHWnds = &vecHWnds;
    EnumWindows(lpEnumFunc, (LPARAM)&wi);
}


WId aria2Launcher::getProcessWId(qint64 pid) //pid转为WId类型的数据
{
    if (pid != 0)
    {
        std::vector<HWND> vecHWnds;
        GetHWndsByProcessID((DWORD)pid, vecHWnds);
        for (const HWND& h : vecHWnds)
        {
            HWND parent = GetParent(h);
            if (parent == NULL)
            {
                return (WId)h;
            }
        }
    }
    return 0;
}
//https://blog.csdn.net/xiaopei_yan/article/details/125931178
//------------------------------------------------------------------------------