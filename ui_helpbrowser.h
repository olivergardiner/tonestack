/********************************************************************************
** Form generated from reading UI file 'helpbrowser.ui'
**
** Created by: Qt User Interface Compiler version 6.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELPBROWSER_H
#define UI_HELPBROWSER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HelpBrowser
{
public:
    QWidget *dockWidgetContents;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *contents;
    QWidget *index;
    QHBoxLayout *browser;

    void setupUi(QDockWidget *HelpBrowser)
    {
        if (HelpBrowser->objectName().isEmpty())
            HelpBrowser->setObjectName(QString::fromUtf8("HelpBrowser"));
        HelpBrowser->resize(673, 654);
        HelpBrowser->setFloating(true);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dockWidgetContents->sizePolicy().hasHeightForWidth());
        dockWidgetContents->setSizePolicy(sizePolicy);
        widget = new QWidget(dockWidgetContents);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(20, 20, 631, 591));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(widget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setMaximumSize(QSize(200, 16777215));
        contents = new QWidget();
        contents->setObjectName(QString::fromUtf8("contents"));
        tabWidget->addTab(contents, QString());
        index = new QWidget();
        index->setObjectName(QString::fromUtf8("index"));
        tabWidget->addTab(index, QString());

        horizontalLayout->addWidget(tabWidget);

        browser = new QHBoxLayout();
        browser->setObjectName(QString::fromUtf8("browser"));
        browser->setSizeConstraint(QLayout::SetMaximumSize);

        horizontalLayout->addLayout(browser);

        HelpBrowser->setWidget(dockWidgetContents);

        retranslateUi(HelpBrowser);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(HelpBrowser);
    } // setupUi

    void retranslateUi(QDockWidget *HelpBrowser)
    {
        HelpBrowser->setWindowTitle(QCoreApplication::translate("HelpBrowser", "Help", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(contents), QCoreApplication::translate("HelpBrowser", "Contents", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(index), QCoreApplication::translate("HelpBrowser", "Index", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HelpBrowser: public Ui_HelpBrowser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELPBROWSER_H
