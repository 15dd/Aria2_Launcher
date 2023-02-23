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
private slots:
	void startWithWindows(int state);
	void startHide(int state);
	void trayNotice(int state);
	void showWebui(int state);

	void yaaw();
	void ariang();
public:
	QSettings* iniSetting;
	Ui::setting* ui;
};