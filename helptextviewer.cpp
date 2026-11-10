#include "helptextviewer.h"

HelpTextViewer::HelpTextViewer(QHelpEngine* helpEngine,
                         QWidget* parent):QTextBrowser(parent),
                         helpEngine(helpEngine)
{
}

QVariant HelpTextViewer::loadResource(int type, const QUrl &name){
    QList<QUrl> files = helpEngine->files("whitecottage.tonestack.help", "");

    if (name.scheme() == "qthelp") {
        QVariant file = helpEngine->fileData(name);
        return QVariant(helpEngine->fileData(name));
    } else {
        return QTextBrowser::loadResource(type, name);
    }
}
