#pragma once

#include "ui_settingWindow.h"
#include <QFileInfo>
#include <QSettings>
#include <QDir>

class setting : public QDialog {
	Q_OBJECT
public:
	setting(QWidget* parent = 0);
	~setting();

	void settingInitialize(); //读取数据，初始化单选项
	void startWithWindows(bool isStart); //开机启动
public slots:
	void startWithWindowsEnabled();
	void startWithWindowsDisabled();
	void startHideEnabled();
	void startHideDisabled();
	void trayNoticeEnabled();
	void trayNoticeDisabled();
	void showWebuiEnabled();
	void showWebuiDisabled();
public:
	QSettings* iniSetting;
	Ui::setting* ui;
};