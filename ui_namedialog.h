/********************************************************************************
** Form generated from reading UI file 'namedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NAMEDIALOG_H
#define UI_NAMEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_NameDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLineEdit *lineEdit;

    void setupUi(QDialog *NameDialog)
    {
        if (NameDialog->objectName().isEmpty())
            NameDialog->setObjectName(QString::fromUtf8("NameDialog"));
        NameDialog->resize(400, 150);
        buttonBox = new QDialogButtonBox(NameDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 100, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(NameDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 40, 71, 16));
        lineEdit = new QLineEdit(NameDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(120, 40, 231, 20));

        retranslateUi(NameDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, NameDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, NameDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(NameDialog);
    } // setupUi

    void retranslateUi(QDialog *NameDialog)
    {
        NameDialog->setWindowTitle(QCoreApplication::translate("NameDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("NameDialog", "Circuit Name:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NameDialog: public Ui_NameDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NAMEDIALOG_H
