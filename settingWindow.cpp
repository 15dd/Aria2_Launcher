#include "settingWindow.h"

setting::setting(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::setting) 
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());//固定窗口尺寸

    QString iniPath(QApplication::applicationDirPath() + "/aria2Launcher.ini");
    bool flag = QFile::exists(iniPath); //检测aria2Launcher.ini是否存在

    QFile Ini(iniPath); //有则打开，无则新建
    iniSetting = new QSettings(iniPath, QSettings::IniFormat); //ini读写

    if (flag) { //如果存在，读取数据，初始化单选项
        Ini.open(QIODevice::ReadWrite);
        settingInitialize();
    }
    else { //如果没有ini，即第一次打开程序，配置默认选项
        startWithWindowsDisabled();
        startHideDisabled();
        trayNoticeEnabled();
        showWebuiEnabled();
    }
    
    connect(ui->radioButton,   &QRadioButton::clicked, this, &setting::startWithWindowsEnabled);
    connect(ui->radioButton_2, &QRadioButton::clicked, this, &setting::startWithWindowsDisabled);
    connect(ui->radioButton_5, &QRadioButton::clicked, this, &setting::startHideEnabled);
    connect(ui->radioButton_6, &QRadioButton::clicked, this, &setting::startHideDisabled);
    connect(ui->radioButton_3, &QRadioButton::clicked, this, &setting::trayNoticeEnabled);
    connect(ui->radioButton_4, &QRadioButton::clicked, this, &setting::trayNoticeDisabled);
    connect(ui->radioButton_7, &QRadioButton::clicked, this, &setting::showWebuiEnabled);
    connect(ui->radioButton_8, &QRadioButton::clicked, this, &setting::showWebuiDisabled);
}
setting::~setting() {
    delete ui;
}

void setting::settingInitialize() { //读取数据，初始化单选项
    QVariant sww = iniSetting->value("setting/startWithWindows").toString();
    if (sww.toBool() == true) {
        ui->radioButton->setChecked(true);
    }
    else {
        ui->radioButton_2->setChecked(true);
    }

    QVariant sh = iniSetting->value("setting/startHide").toString();
    if (sh.toBool() == true) {
        ui->radioButton_5->setChecked(true);
    }
    else {
        ui->radioButton_6->setChecked(true);
    }

    QVariant tn = iniSetting->value("setting/trayNotice").toString();
    if (tn.toBool() == true) {
        ui->radioButton_3->setChecked(true);
    }
    else {
        ui->radioButton_4->setChecked(true);
    }

    QVariant sw = iniSetting->value("setting/showWebui").toString();
    if (sw.toBool() == true) {
        ui->radioButton_7->setChecked(true);
    }
    else {
        ui->radioButton_8->setChecked(true);
    }
}

void setting::startWithWindowsEnabled() {
    iniSetting->setValue("setting/startWithWindows","true");
    startWithWindows(true);
}

void setting::startWithWindowsDisabled() {
    iniSetting->setValue("setting/startWithWindows","false");
    startWithWindows(false);
}

void setting::startHideEnabled() {
    iniSetting->setValue("setting/startHide","true");
}

void setting::startHideDisabled() {
    iniSetting->setValue("setting/startHide", "false");
}

void setting::trayNoticeEnabled() {
    iniSetting->setValue("setting/trayNotice", "true");
}

void setting::trayNoticeDisabled() {
    iniSetting->setValue("setting/trayNotice", "false");
}

void setting::showWebuiEnabled() {
    iniSetting->setValue("setting/showWebui", "true");
}

void setting::showWebuiDisabled() {
    iniSetting->setValue("setting/showWebui", "false");
}

//开机启动=============================================================================================
//https://blog.csdn.net/qq_41632571/article/details/126105512
//设置/取消自启动   
//isStart: true(开机启动)    false(开机不启动)
void setting::startWithWindows(bool isStart)
{
    QSettings reg("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString application_name = QApplication::applicationName();
    QString application_path = QCoreApplication::applicationFilePath();
    if (isStart) {
        //设置开机自启注册表
        application_path.replace(".exe", ".lnk");
        if (!QFile::exists(application_path)) {
            QFile::link(QCoreApplication::applicationFilePath(), application_name + ".lnk");
        }
        QString strAppPath = QDir::toNativeSeparators(application_path);
        reg.setValue(application_name, strAppPath);
    }
    else {
        //取消开机自启注册表
        reg.remove(application_name);
    }

}
//end=================================================================================================