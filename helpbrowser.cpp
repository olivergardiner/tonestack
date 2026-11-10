#include "helpbrowser.h"
#include "ui_helpbrowser.h"

HelpBrowser::HelpBrowser(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::HelpBrowser)
{
    ui->setupUi(this);
}

HelpBrowser::~HelpBrowser()
{
    delete ui;
}

void HelpBrowser::setHelp(QString qhc, QString index)
{
    QString path = QApplication::applicationDirPath() + qhc;
    QHelpEngine *helpEngine = new QHelpEngine(QApplication::applicationDirPath() + qhc, this);
    helpEngine->setReadOnly(true);
    helpEngine->setupData();

    ui->tabWidget->clear();
    ui->tabWidget->addTab(helpEngine->contentWidget(), "Contents");
    ui->tabWidget->addTab(helpEngine->indexWidget(), "Index");

    if (textBrowser != nullptr) {
        ui->browser->removeWidget(textBrowser);
    }

    textBrowser = new HelpTextViewer(helpEngine, this);
    textBrowser->setSource(QUrl("qthelp://" + index));

    connect(helpEngine->contentWidget(),
            SIGNAL(linkActivated(QUrl)),
            textBrowser, SLOT(setSource(QUrl)));

    connect(helpEngine->indexWidget(),
            SIGNAL(linkActivated(QUrl, QString)),
            textBrowser, SLOT(setSource(QUrl)));

    ui->browser->addWidget(textBrowser);
}
