#include "about.h"

about::about(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());

    connect(ui->github, &QPushButton::clicked, [this]() {QDesktopServices::openUrl(QUrl("https://github.com/15dd/Aria2_Launcher", QUrl::TolerantMode)); });
}
about::~about() {
    delete ui;
}


