#include "about.h"

about::about(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());

    connect(ui->github, &QPushButton::clicked, [this]() {QDesktopServices::openUrl(QUrl("https://www.baidu.com", QUrl::TolerantMode)); });
}
about::~about() {
    delete ui;
}


