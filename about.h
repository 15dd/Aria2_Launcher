#pragma once

#include <QDialog>
#include "ui_about.h"
#include <QDesktopServices>
#include <QUrl>

class about : public QDialog {
	Q_OBJECT
public:
	about(QWidget* parent = 0);
	~about();
private:
	Ui::About* ui;
};