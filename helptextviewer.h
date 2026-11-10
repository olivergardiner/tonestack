#ifndef HELPTEXTVIEWER_H
#define HELPTEXTVIEWER_H

#include <QTextBrowser>
#include <QHelpEngine>

class HelpTextViewer : public QTextBrowser
{
public:
    HelpTextViewer(QHelpEngine* helpEngine, QWidget* parent = 0);
    QVariant loadResource (int type, const QUrl& name);
private:
    QHelpEngine* helpEngine;
    HelpTextViewer();
};

#endif // HELPTEXTVIEWER_H
