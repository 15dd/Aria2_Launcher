#include "aria2Launcher.h"

aria2Launcher::aria2Launcher(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::aria2LauncherClass) 
{
    ui->setupUi(this);

    checkFile();//检测所需文件是否存在
    checkAria2Status();//检测是否已经有aria2c运行,可选择新建进程或kill已存在的进程再新建进程
    uiInitialize();//ui初始化
    showWindow();//根据设置决定是否开启webui
    showHide();//根据设置决定是显示窗口还是启动到托盘

    //this->resize((this->width())+1,(this->height())+1);//界面刷新
    
    connect(ui->aboutqt, &QAction::triggered, [this]() {QMessageBox::aboutQt(this, tr("About Qt")); });//弹出qt版本
    connect(ui->aboutthis, &QAction::triggered, [this]() {aboutWin->exec(); });//弹出关于窗口
    connect(ui->set, &QAction::triggered, [this]() {settingWin->exec(); });//弹出设置窗口
    connect(ui->close, &QAction::triggered, [this]() {qApp->quit(); });

    connect(settingWin->ui->radioButton_7, &QRadioButton::clicked, [this]() {ui->webui->setEnabled(true); });//webui启用时，webui选择菜单项可用
    connect(settingWin->ui->radioButton_8, &QRadioButton::clicked, [this]() {ui->webui->setEnabled(false); showCmd(); ui->cmd->setChecked(true); });//webui禁用时，webui选择菜单项禁用，将当前主窗口显示画面变为cmd界面
    connect(ui->cmd, &QAction::triggered, this, &aria2Launcher::showCmd);
    connect(ui->webui, &QAction::triggered, this, &aria2Launcher::showWebui);
}

aria2Launcher::~aria2Launcher()
{
    //system("taskkill /f /t /im cmd.exe");
    if (pid != 0) { //如果aria2c还在运行，则kill
        KillProcess(pid);
    }
    delete ui;
}

void aria2Launcher::showWindow() { //决定开屏显示webui或cmd
    if (settingWin->ui->radioButton_7->isChecked()) {
        showWebui();
    }
    else {
        this->ui->webui->setEnabled(false);
        this->ui->cmd->setChecked(true);
        showCmd();
    }
}

void aria2Launcher::showWebui() { //显示webui
    centralWidget()->setParent(NULL); //在切换窗口时，centralwidget一定要先置空（即在函数的最前面）不然会有问题
    if (view != NULL) {
        delete view;
        view = NULL;
    }
    view = new QWebEngineView(this);
    view->setUrl(QUrl(QApplication::applicationDirPath() + "/yaaw/index.html"));
    setCentralWidget(view);
}

void aria2Launcher::showCmd() { //显示cmd
    centralWidget()->setParent(NULL); //在切换窗口时，centralwidget一定要先置空（即在函数的最前面）不然会有问题
    if (view != NULL) {
        delete view;
        view = NULL;
    }
    setCentralWidget(aria2CmdWidget);
}

void aria2Launcher::checkFile() { //检测所需文件是否存在
    //QFileInfo file1(QApplication::applicationDirPath() + "/aria2c.exe");
    QFileInfo file1("aria2c.exe");
    QFileInfo file2("aria2.conf");
    QFileInfo file3("aria2.session");
    QFileInfo file4("yaaw");
    if (!file1.isFile()) {
        QMessageBox::critical(this, "缺少文件", "aria2c.exe不存在，无法启动");
        exit(0);
    }
    else if (!file2.isFile()) {
        QMessageBox::critical(this, "缺少文件", "aria2.conf不存在，无法启动");
        exit(0);
    }
    else if (!file3.isFile()) {
        QMessageBox::critical(this, "缺少文件", "aria2.session不存在，无法启动");
        exit(0);
    }
    else if (!file4.isDir()) {
        QMessageBox::critical(this, "缺少文件夹", "/yaaw/不存在，无法启动");
        exit(0);
    }
}

void aria2Launcher::checkAria2Status() { //检测是否已经有aria2c运行,可选择新建进程或kill已存在的进程再新建进程
    int n = 1; //记录检测次数
    bool flag = true; 
    int findPid = (int)FindProcessIDByName("aria2c.exe"); //检测aria2c是否已经存在
    while (findPid != 0 && flag == true) { //如果aria2c.exe已存在
        int id = QMessageBox::information(this, "提示", QString("第%1次检测\naria2c.exe(PID:%2)已在运行(造成这种情况的原因可能是以下几点)\n1：有正在使用aria2c的应用\n2：您未正确关闭此软件或其他使用aria2c的软件\n\n继续：创建一个新的aria2c进程，不影响已存在的进程\n结束进程：结束已存在的aria2c进程(请确保您没有使用此进程下载文件中)\n\n如果您不知道如何选择，请点击<继续>").arg(n).arg(findPid), QString("继续"), QString("结束进程(PID:%1)").arg(findPid), 0);
        switch (id) {
        case 0: { //继续，选择此选项时退出循环，不再检测，直接创建新进程
            Start(); 
            flag = false;
            break;
        }
        case 1: { //结束进程，选择此选项时会一直循环下去，直到airac.exe不存在进程列表中时
            KillProcess(findPid);
            break;
        }
        }
        findPid = (int)FindProcessIDByName("aria2c.exe"); //检测是否杀完进程，杀完则退出循环，没有则继续
        n++;
    }
    if (findPid == 0) { //杀完aria2后，新建aria2c的进程
        Start();
    }
}

void aria2Launcher::showHide() { //根据设置决定是显示窗口还是启动到托盘
    if (settingWin->ui->radioButton_6->isChecked()) {
        this->showNormal();

        this->setWindowState(Qt::WindowActive);//窗口置顶
        this->activateWindow();

        this->resize((this->width()) + 1, (this->height()) + 1);//界面刷新
    }
    else {
        showWindowsMessage();
    }
}


void aria2Launcher::uiInitialize() { //ui初始化
    //托盘初始化
    QMenu* Menu = new QMenu(this);
    QIcon icon(":/aria2Launcher/resource/img/ico.png");
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    trayIcon->setToolTip("Aria2后台");
    QAction* SOH = new QAction("显示/隐藏", this);
    QAction* Close = new QAction("退出程序", this);
    SOH->setIcon(QIcon(":/aria2Launcher/resource/img/show.png"));
    Close->setIcon(QIcon(":/aria2Launcher/resource/img/close.png"));
    
    Menu->addAction(SOH);
    Menu->addSeparator();
    Menu->addAction(Close);

    trayIcon->setContextMenu(Menu);
    trayIcon->show();

    //菜单初始化
    connect(SOH, &QAction::triggered, this, &aria2Launcher::showOrHide);
    connect(Close, &QAction::triggered, this, &aria2Launcher::quitApp);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &aria2Launcher::on_activatedSysTrayIcon);

    QActionGroup* group = new QActionGroup(this);
    group->addAction(ui->cmd);
    group->addAction(ui->webui);
}

void aria2Launcher::closeEvent(QCloseEvent* event) { //关闭事件
    if (!quitApp()) { 
        event->ignore();
    }
}

bool aria2Launcher::quitApp() { //退出询问
    int id = QMessageBox::information(this, "二次确认", "是否退出程序?\n退出请确保没有文件正在下载中", QString("退出"), QString("最小化到系统托盘"), QString("取消"), 2);
    switch (id) {
    case 0: {
        qApp->quit();
        break;
    }
    case 1: {
        this->hide();
        showWindowsMessage();//提示程序已进入托盘
        return false;
        break;
    }
    case 2: {
        return false;
        break;
    }
    }
}

void aria2Launcher::showWindowsMessage() {//提示程序已进入托盘
    if (settingWin->ui->radioButton_3->isChecked()) {
        trayIcon->showMessage("Aria2 Launcher已最小化到托盘", "如果您希望不再看到这个提示，请前往设置关闭");
    }
}

void aria2Launcher::showOrHide() {//显示主窗口或隐藏主窗口，托盘事件
    if (this->isHidden()) {
        this->showNormal();

        this->setWindowState(Qt::WindowActive);//窗口置顶
        this->activateWindow();

        this->resize((this->width()) + 1, (this->height()) + 1);//界面刷新
    }
    else {
        showWindowsMessage();
        this->hide();
    }
}

void aria2Launcher::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason) //单击托盘图标事件
{
    switch (reason) {

    case QSystemTrayIcon::Trigger: //单击
        showOrHide();
        break;
    default:
        break;
    }
}

void aria2Launcher::Start() { //启动aria2c.exe,并嵌入窗口中
    pid = startProcess("aria2c --conf-path=aria2.conf"); //启动aria2c.exe
    //HWND Hpid = (HWND)(QString::number(pid).toInt()); //将pid转为HWND类型的数据

    QLabel* showPid = new QLabel("Aria2c.exe PID - " + QString::number(pid), this); //状态栏显示aria2c.exe的pid
    statusBar()->addWidget(showPid);

    QWindow* aria2Cmd = QWindow::fromWinId(getProcessWId(pid)); //根据pid，将aria2c.exe的窗口嵌入至主窗口中
    aria2Cmd->setFlags(aria2Cmd->flags() | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    aria2CmdWidget = QWidget::createWindowContainer(aria2Cmd);
}

//寻找是否有已存在的aria2c.exe========================================================================
std::wstring string2wstring(std::string str)//string to wstring https://blog.csdn.net/mercy_ps/article/details/81218608
{
    std::wstring result;
    //获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
    TCHAR* buffer = new TCHAR[len + 1];
    //多字节编码转换成宽字节编码  
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
    buffer[len] = '\0';             //添加字符串结尾  
    //删除缓冲区并返回值  
    result.append(buffer);
    delete[] buffer;
    return result;
}

//根据程序名寻找pid https://blog.csdn.net/calmreason/article/details/104971324
DWORD aria2Launcher::FindProcessIDByName(const std::string& processName)//0 not found ; other found; processName "processName.exe"
{
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return(0);
    }
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcessSnap, &pe32))
    {
        CloseHandle(hProcessSnap);          // clean the snapshot object
        return(0);
    }
    DWORD processId = 0;
    do
    {
        if (std::wstring(pe32.szExeFile) == string2wstring(processName))//进程名称
        {
            processId = pe32.th32ProcessID;//进程ID
            break;
        }
    } while (Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);
    return(processId);
}
//end=============================================================================================

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

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    Sleep(500);
    return (qint64)pi.dwProcessId;
}

BOOL aria2Launcher::KillProcess(DWORD ProcessId) //根据pid杀死进程
{
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessId);
    if (hProcess == NULL)
        return FALSE;
    if (!TerminateProcess(hProcess, 0))
        return FALSE;
    return TRUE;
}
// https://blog.csdn.net/jfkidear/article/details/27057861


//qint64转WId=====================================================================================
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
//end=============================================================================================