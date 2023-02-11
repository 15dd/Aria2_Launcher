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

	void settingInitialize();
	void startWithWindows(bool isStart);
public slots:
	void startWithWindowsEnabled();
	void startWithWindowsDisabled();
	void startHideEnabled();
	void startHideDisabled();
	void trayNoticeEnabled();
	void trayNoticeDisabled();
public:
	QSettings* iniSetting;
	bool tn;
	bool sh;
private:
	Ui::setting* ui;
};