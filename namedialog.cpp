#include "namedialog.h"
#include "ui_namedialog.h"

NameDialog::NameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NameDialog)
{
    ui->setupUi(this);
}

NameDialog::~NameDialog()
{
    delete ui;
}

QString NameDialog::getName()
{
    return ui->lineEdit->text();
}

void NameDialog::setName(QString name)
{
    ui->lineEdit->setText(name);
}
