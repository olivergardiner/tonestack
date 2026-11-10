#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include <QDockWidget>
#include <QHelpEngine>
#include <QHelpContentWidget>
#include <QHelpIndexWidget>

#include "helptextviewer.h"

namespace Ui {
class HelpBrowser;
}

class HelpBrowser : public QDockWidget
{
    Q_OBJECT

public:
    explicit HelpBrowser(QWidget *parent = nullptr);
    ~HelpBrowser();

    void setHelp(QString qhc, QString index);

private:
    Ui::HelpBrowser *ui;

    HelpTextViewer *textBrowser = nullptr;
};

#endif // HELPBROWSER_H
