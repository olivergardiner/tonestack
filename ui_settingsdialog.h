/********************************************************************************
** Form generated from reading UI file 'settingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QDialogButtonBox *buttonBox;
    QTabWidget *tabWidget;
    QWidget *tab1;
    QWidget *tab2;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer;
    QComboBox *algorithm;
    QGroupBox *groupBox;
    QWidget *widget1;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QDoubleSpinBox *logFactor;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QDoubleSpinBox *logAFactor;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QDoubleSpinBox *rLogFactor;
    QHBoxLayout *horizontalLayout_4;
    QGraphicsView *potentiometerResponse;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
        SettingsDialog->resize(509, 557);
        buttonBox = new QDialogButtonBox(SettingsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(140, 500, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        tabWidget = new QTabWidget(SettingsDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(20, 20, 471, 471));
        tab1 = new QWidget();
        tab1->setObjectName(QString::fromUtf8("tab1"));
        tabWidget->addTab(tab1, QString());
        tab2 = new QWidget();
        tab2->setObjectName(QString::fromUtf8("tab2"));
        widget = new QWidget(tab2);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 10, 441, 421));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_4 = new QLabel(widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_5->addWidget(label_4);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        algorithm = new QComboBox(widget);
        algorithm->setObjectName(QString::fromUtf8("algorithm"));
        algorithm->setMinimumSize(QSize(100, 0));
        algorithm->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_5->addWidget(algorithm);


        verticalLayout_2->addLayout(horizontalLayout_5);

        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(0, 120));
        groupBox->setMaximumSize(QSize(16777215, 140));
        widget1 = new QWidget(groupBox);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        widget1->setGeometry(QRect(10, 10, 421, 101));
        verticalLayout = new QVBoxLayout(widget1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(widget1);
        label->setObjectName(QString::fromUtf8("label"));
        label->setTextFormat(Qt::AutoText);

        horizontalLayout->addWidget(label);

        logFactor = new QDoubleSpinBox(widget1);
        logFactor->setObjectName(QString::fromUtf8("logFactor"));
        logFactor->setMaximumSize(QSize(80, 16777215));
        logFactor->setMaximum(1.000000000000000);
        logFactor->setSingleStep(0.010000000000000);

        horizontalLayout->addWidget(logFactor);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(widget1);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        logAFactor = new QDoubleSpinBox(widget1);
        logAFactor->setObjectName(QString::fromUtf8("logAFactor"));
        logAFactor->setMaximumSize(QSize(80, 16777215));
        logAFactor->setMaximum(1.000000000000000);
        logAFactor->setSingleStep(0.010000000000000);

        horizontalLayout_2->addWidget(logAFactor);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(widget1);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        rLogFactor = new QDoubleSpinBox(widget1);
        rLogFactor->setObjectName(QString::fromUtf8("rLogFactor"));
        rLogFactor->setMaximumSize(QSize(80, 16777215));
        rLogFactor->setMaximum(1.000000000000000);
        rLogFactor->setSingleStep(0.010000000000000);

        horizontalLayout_3->addWidget(rLogFactor);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addWidget(groupBox);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        potentiometerResponse = new QGraphicsView(widget);
        potentiometerResponse->setObjectName(QString::fromUtf8("potentiometerResponse"));
        potentiometerResponse->setMaximumSize(QSize(280, 280));

        horizontalLayout_4->addWidget(potentiometerResponse);


        verticalLayout_2->addLayout(horizontalLayout_4);

        tabWidget->addTab(tab2, QString());

        retranslateUi(SettingsDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, SettingsDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, SettingsDialog, qOverload<>(&QDialog::reject));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QCoreApplication::translate("SettingsDialog", "Dialog", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab1), QCoreApplication::translate("SettingsDialog", "General Settings", nullptr));
        label_4->setText(QCoreApplication::translate("SettingsDialog", "Log Algorithm:", nullptr));
        groupBox->setTitle(QCoreApplication::translate("SettingsDialog", "Midpoint Attenuation Factor", nullptr));
        label->setText(QCoreApplication::translate("SettingsDialog", "<span style=\"color: #bf2828\">Log Taper</span>", nullptr));
        label_2->setText(QCoreApplication::translate("SettingsDialog", "<span style=\"color: #28bf28\">Log A Taper</span>", nullptr));
        label_3->setText(QCoreApplication::translate("SettingsDialog", "<span style=\"color: #2828bf\">Reverse Log Taper</span>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab2), QCoreApplication::translate("SettingsDialog", "Potentiometer Tapers", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
