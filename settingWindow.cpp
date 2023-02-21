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
        startWithWindows(Qt::Unchecked);
        startHide(Qt::Unchecked);
        trayNotice(Qt::Checked);
        showWebui(Qt::Checked);
    }
    
    connect(ui->sww, SIGNAL(stateChanged(int)), this, SLOT(startWithWindows(int)), Qt::UniqueConnection);
    connect(ui->sh, SIGNAL(stateChanged(int)), this, SLOT(startHide(int)));
    connect(ui->tn, SIGNAL(stateChanged(int)), this, SLOT(trayNotice(int)));
    connect(ui->isWebui, SIGNAL(stateChanged(int)), this, SLOT(showWebui(int)));

    connect(ui->yaaw, SIGNAL(clicked()), this, SLOT(yaaw()));
    connect(ui->aria2webui, SIGNAL(clicked()), this, SLOT(aria2webui()));
    connect(ui->ariang, SIGNAL(clicked()), this, SLOT(ariang()));
}

setting::~setting() {
    delete ui;
}

void setting::settingInitialize() { //读取数据，初始化单选项
    QVariant sww = iniSetting->value("setting/startWithWindows").toString();
    if (sww.toBool() == true) {
        ui->sww->setCheckState(Qt::Checked);
    }
    else {
        ui->sww->setCheckState(Qt::Unchecked);
    }

    QVariant sh = iniSetting->value("setting/startHide").toString();
    if (sh.toBool() == true) {
        ui->sh->setCheckState(Qt::Checked);
    }
    else {
        ui->sh->setCheckState(Qt::Unchecked);
    }

    QVariant tn = iniSetting->value("setting/trayNotice").toString();
    if (tn.toBool() == true) {
        ui->tn->setCheckState(Qt::Checked);
    }
    else {
        ui->tn->setCheckState(Qt::Unchecked);
    }

    QVariant sw = iniSetting->value("setting/showWebui").toString();
    if (sw.toBool() == true) {
        ui->isWebui->setCheckState(Qt::Checked);
    }
    else {
        ui->isWebui->setCheckState(Qt::Unchecked);
    }

    QVariant ws = iniSetting->value("setting/webuiStyle").toString();
    switch (ws.toInt()) {
    case 1:
        ui->yaaw->setChecked(true);
        break;
    case 2:
        ui->aria2webui->setChecked(true);
        break;
    case 3:
        ui->ariang->setChecked(true);
        break;
    }
}

void setting::startWithWindows(int state) {
    if (state == Qt::Checked) {
        iniSetting->setValue("setting/startWithWindows", "true");
    }
    else {
        iniSetting->setValue("setting/startWithWindows", "false");
    }
}

void setting::startHide(int state) {
    if (state == Qt::Checked) {
        iniSetting->setValue("setting/startHide", "true");
    }
    else {
        iniSetting->setValue("setting/startHide", "false");
    }
}

void setting::trayNotice(int state) {
    if (state == Qt::Checked) {
        iniSetting->setValue("setting/trayNotice", "true");
    }
    else {
        iniSetting->setValue("setting/trayNotice", "false");
    }
}

void setting::showWebui(int state) {
    if (state == Qt::Checked) {
        iniSetting->setValue("setting/showWebui", "true");
    }
    else {
        iniSetting->setValue("setting/showWebui", "false");
    }
}

void setting::yaaw() {
    iniSetting->setValue("setting/webuiStyle", "1");
}

void setting::aria2webui() {
    iniSetting->setValue("setting/webuiStyle", "2");
}

void setting::ariang() {
    iniSetting->setValue("setting/webuiStyle", "3");
}
