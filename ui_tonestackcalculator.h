/********************************************************************************
** Form generated from reading UI file 'tonestackcalculator.ui'
**
** Created by: Qt User Interface Compiler version 6.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TONESTACKCALCULATOR_H
#define UI_TONESTACKCALCULATOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ToneStackCalculator
{
public:
    QAction *actionAbout;
    QAction *actionQuit;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionPrint;
    QAction *actionTone_Stack_Mode;
    QAction *actionBasic_Electronics_Mode;
    QAction *actionActive_Circuits_Mode;
    QAction *actionUser_Circuits_Mode;
    QAction *actionTonestack_Help;
    QAction *actionSettings;
    QWidget *centralwidget;
    QGraphicsView *frequencyResponse;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *stackSelection;
    QSpacerItem *horizontalSpacer;
    QLabel *circuitDisplay;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *pot1Label;
    QDial *pot1Position;
    QLineEdit *pot1Value;
    QComboBox *pot1Type;
    QSpacerItem *horizontalSpacer_4;
    QVBoxLayout *verticalLayout_4;
    QLabel *pot2Label;
    QDial *pot2Position;
    QLineEdit *pot2Value;
    QComboBox *pot2Type;
    QSpacerItem *horizontalSpacer_5;
    QVBoxLayout *verticalLayout_5;
    QLabel *pot3Label;
    QDial *pot3Position;
    QLineEdit *pot3Value;
    QComboBox *pot3Type;
    QSpacerItem *horizontalSpacer_2;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_23;
    QLabel *rSLabel;
    QLineEdit *rSValue;
    QHBoxLayout *horizontalLayout_8;
    QLabel *r1Label;
    QLineEdit *r1Value;
    QHBoxLayout *horizontalLayout_9;
    QLabel *r2Label;
    QLineEdit *r2Value;
    QHBoxLayout *horizontalLayout_10;
    QLabel *r3Label;
    QLineEdit *r3Value;
    QHBoxLayout *horizontalLayout_11;
    QLabel *r4Label;
    QLineEdit *r4Value;
    QHBoxLayout *horizontalLayout_12;
    QLabel *r5Label;
    QLineEdit *r5Value;
    QHBoxLayout *horizontalLayout_13;
    QLabel *c1Label;
    QLineEdit *c1Value;
    QHBoxLayout *horizontalLayout_14;
    QLabel *c2Label;
    QLineEdit *c2Value;
    QHBoxLayout *horizontalLayout_15;
    QLabel *c3Label;
    QLineEdit *c3Value;
    QHBoxLayout *horizontalLayout_16;
    QLabel *c4Label;
    QLineEdit *c4Value;
    QHBoxLayout *horizontalLayout_17;
    QLabel *c5Label;
    QLineEdit *c5Value;
    QHBoxLayout *horizontalLayout_18;
    QLabel *l1Label;
    QLineEdit *l1Value;
    QHBoxLayout *horizontalLayout_19;
    QLabel *l2Label;
    QLineEdit *l2Value;
    QHBoxLayout *horizontalLayout_20;
    QLabel *l3Label;
    QLineEdit *l3Value;
    QHBoxLayout *horizontalLayout_21;
    QLabel *l4Label;
    QLineEdit *l4Value;
    QHBoxLayout *horizontalLayout_22;
    QLabel *l5Label;
    QLineEdit *l5Value;
    QHBoxLayout *horizontalLayout_24;
    QLabel *rLLabel;
    QLineEdit *rLValue;
    QPushButton *resetButton;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuMode;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ToneStackCalculator)
    {
        if (ToneStackCalculator->objectName().isEmpty())
            ToneStackCalculator->setObjectName(QString::fromUtf8("ToneStackCalculator"));
        ToneStackCalculator->resize(1261, 631);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ToneStackCalculator->sizePolicy().hasHeightForWidth());
        ToneStackCalculator->setSizePolicy(sizePolicy);
        actionAbout = new QAction(ToneStackCalculator);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAbout->setMenuRole(QAction::AboutRole);
        actionQuit = new QAction(ToneStackCalculator);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionQuit->setMenuRole(QAction::QuitRole);
        actionOpen = new QAction(ToneStackCalculator);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave = new QAction(ToneStackCalculator);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave->setEnabled(false);
        actionSave_As = new QAction(ToneStackCalculator);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        actionSave_As->setMenuRole(QAction::TextHeuristicRole);
        actionPrint = new QAction(ToneStackCalculator);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        actionTone_Stack_Mode = new QAction(ToneStackCalculator);
        actionTone_Stack_Mode->setObjectName(QString::fromUtf8("actionTone_Stack_Mode"));
        actionTone_Stack_Mode->setCheckable(true);
        actionTone_Stack_Mode->setChecked(true);
        actionBasic_Electronics_Mode = new QAction(ToneStackCalculator);
        actionBasic_Electronics_Mode->setObjectName(QString::fromUtf8("actionBasic_Electronics_Mode"));
        actionBasic_Electronics_Mode->setCheckable(true);
        actionActive_Circuits_Mode = new QAction(ToneStackCalculator);
        actionActive_Circuits_Mode->setObjectName(QString::fromUtf8("actionActive_Circuits_Mode"));
        actionActive_Circuits_Mode->setCheckable(true);
        actionUser_Circuits_Mode = new QAction(ToneStackCalculator);
        actionUser_Circuits_Mode->setObjectName(QString::fromUtf8("actionUser_Circuits_Mode"));
        actionUser_Circuits_Mode->setCheckable(true);
        actionTonestack_Help = new QAction(ToneStackCalculator);
        actionTonestack_Help->setObjectName(QString::fromUtf8("actionTonestack_Help"));
        actionSettings = new QAction(ToneStackCalculator);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        centralwidget = new QWidget(ToneStackCalculator);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        frequencyResponse = new QGraphicsView(centralwidget);
        frequencyResponse->setObjectName(QString::fromUtf8("frequencyResponse"));
        frequencyResponse->setGeometry(QRect(670, 40, 581, 541));
        frequencyResponse->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        frequencyResponse->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 471, 571));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        stackSelection = new QComboBox(layoutWidget);
        stackSelection->setObjectName(QString::fromUtf8("stackSelection"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(stackSelection->sizePolicy().hasHeightForWidth());
        stackSelection->setSizePolicy(sizePolicy1);
        stackSelection->setMinimumSize(QSize(60, 0));
        stackSelection->setBaseSize(QSize(120, 20));
        stackSelection->setMaxVisibleItems(20);

        horizontalLayout->addWidget(stackSelection);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        circuitDisplay = new QLabel(layoutWidget);
        circuitDisplay->setObjectName(QString::fromUtf8("circuitDisplay"));
        sizePolicy1.setHeightForWidth(circuitDisplay->sizePolicy().hasHeightForWidth());
        circuitDisplay->setSizePolicy(sizePolicy1);
        circuitDisplay->setMinimumSize(QSize(469, 360));
        circuitDisplay->setMaximumSize(QSize(16777215, 360));
        circuitDisplay->setFrameShape(QFrame::Panel);
        circuitDisplay->setScaledContents(false);
        circuitDisplay->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(circuitDisplay);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        pot1Label = new QLabel(layoutWidget);
        pot1Label->setObjectName(QString::fromUtf8("pot1Label"));
        pot1Label->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(pot1Label);

        pot1Position = new QDial(layoutWidget);
        pot1Position->setObjectName(QString::fromUtf8("pot1Position"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pot1Position->sizePolicy().hasHeightForWidth());
        pot1Position->setSizePolicy(sizePolicy2);
        pot1Position->setMinimumSize(QSize(0, 0));
        pot1Position->setBaseSize(QSize(0, 0));
        pot1Position->setTracking(true);
        pot1Position->setOrientation(Qt::Horizontal);
        pot1Position->setInvertedAppearance(false);
        pot1Position->setNotchesVisible(true);

        verticalLayout_3->addWidget(pot1Position);

        pot1Value = new QLineEdit(layoutWidget);
        pot1Value->setObjectName(QString::fromUtf8("pot1Value"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(pot1Value->sizePolicy().hasHeightForWidth());
        pot1Value->setSizePolicy(sizePolicy3);
        pot1Value->setMaximumSize(QSize(100, 16777215));

        verticalLayout_3->addWidget(pot1Value);

        pot1Type = new QComboBox(layoutWidget);
        pot1Type->addItem(QString());
        pot1Type->addItem(QString());
        pot1Type->addItem(QString());
        pot1Type->addItem(QString());
        pot1Type->addItem(QString());
        pot1Type->setObjectName(QString::fromUtf8("pot1Type"));
        pot1Type->setMaximumSize(QSize(100, 16777215));

        verticalLayout_3->addWidget(pot1Type);


        horizontalLayout_6->addLayout(verticalLayout_3);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        pot2Label = new QLabel(layoutWidget);
        pot2Label->setObjectName(QString::fromUtf8("pot2Label"));
        pot2Label->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(pot2Label);

        pot2Position = new QDial(layoutWidget);
        pot2Position->setObjectName(QString::fromUtf8("pot2Position"));
        sizePolicy2.setHeightForWidth(pot2Position->sizePolicy().hasHeightForWidth());
        pot2Position->setSizePolicy(sizePolicy2);
        pot2Position->setNotchesVisible(true);

        verticalLayout_4->addWidget(pot2Position);

        pot2Value = new QLineEdit(layoutWidget);
        pot2Value->setObjectName(QString::fromUtf8("pot2Value"));
        sizePolicy3.setHeightForWidth(pot2Value->sizePolicy().hasHeightForWidth());
        pot2Value->setSizePolicy(sizePolicy3);
        pot2Value->setMaximumSize(QSize(100, 16777215));

        verticalLayout_4->addWidget(pot2Value);

        pot2Type = new QComboBox(layoutWidget);
        pot2Type->addItem(QString());
        pot2Type->addItem(QString());
        pot2Type->addItem(QString());
        pot2Type->addItem(QString());
        pot2Type->addItem(QString());
        pot2Type->setObjectName(QString::fromUtf8("pot2Type"));
        pot2Type->setMaximumSize(QSize(100, 16777215));

        verticalLayout_4->addWidget(pot2Type);


        horizontalLayout_6->addLayout(verticalLayout_4);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        pot3Label = new QLabel(layoutWidget);
        pot3Label->setObjectName(QString::fromUtf8("pot3Label"));
        pot3Label->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(pot3Label);

        pot3Position = new QDial(layoutWidget);
        pot3Position->setObjectName(QString::fromUtf8("pot3Position"));
        sizePolicy2.setHeightForWidth(pot3Position->sizePolicy().hasHeightForWidth());
        pot3Position->setSizePolicy(sizePolicy2);
        pot3Position->setNotchesVisible(true);

        verticalLayout_5->addWidget(pot3Position);

        pot3Value = new QLineEdit(layoutWidget);
        pot3Value->setObjectName(QString::fromUtf8("pot3Value"));
        pot3Value->setEnabled(true);
        sizePolicy3.setHeightForWidth(pot3Value->sizePolicy().hasHeightForWidth());
        pot3Value->setSizePolicy(sizePolicy3);
        pot3Value->setMinimumSize(QSize(0, 0));
        pot3Value->setMaximumSize(QSize(100, 16777215));
        pot3Value->setBaseSize(QSize(0, 0));

        verticalLayout_5->addWidget(pot3Value);

        pot3Type = new QComboBox(layoutWidget);
        pot3Type->addItem(QString());
        pot3Type->addItem(QString());
        pot3Type->addItem(QString());
        pot3Type->addItem(QString());
        pot3Type->addItem(QString());
        pot3Type->setObjectName(QString::fromUtf8("pot3Type"));
        pot3Type->setMaximumSize(QSize(100, 16777215));

        verticalLayout_5->addWidget(pot3Type);


        horizontalLayout_6->addLayout(verticalLayout_5);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_6);

        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(490, 40, 171, 526));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_2);

        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setObjectName(QString::fromUtf8("horizontalLayout_23"));
        rSLabel = new QLabel(verticalLayoutWidget);
        rSLabel->setObjectName(QString::fromUtf8("rSLabel"));
        rSLabel->setMinimumSize(QSize(30, 0));
        rSLabel->setBaseSize(QSize(30, 0));

        horizontalLayout_23->addWidget(rSLabel);

        rSValue = new QLineEdit(verticalLayoutWidget);
        rSValue->setObjectName(QString::fromUtf8("rSValue"));

        horizontalLayout_23->addWidget(rSValue);


        verticalLayout_2->addLayout(horizontalLayout_23);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        r1Label = new QLabel(verticalLayoutWidget);
        r1Label->setObjectName(QString::fromUtf8("r1Label"));
        r1Label->setMinimumSize(QSize(30, 0));
        r1Label->setBaseSize(QSize(30, 0));

        horizontalLayout_8->addWidget(r1Label);

        r1Value = new QLineEdit(verticalLayoutWidget);
        r1Value->setObjectName(QString::fromUtf8("r1Value"));

        horizontalLayout_8->addWidget(r1Value);


        verticalLayout_2->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        r2Label = new QLabel(verticalLayoutWidget);
        r2Label->setObjectName(QString::fromUtf8("r2Label"));
        r2Label->setMinimumSize(QSize(30, 0));
        r2Label->setMaximumSize(QSize(30, 16777215));
        r2Label->setBaseSize(QSize(30, 0));

        horizontalLayout_9->addWidget(r2Label);

        r2Value = new QLineEdit(verticalLayoutWidget);
        r2Value->setObjectName(QString::fromUtf8("r2Value"));

        horizontalLayout_9->addWidget(r2Value);


        verticalLayout_2->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        r3Label = new QLabel(verticalLayoutWidget);
        r3Label->setObjectName(QString::fromUtf8("r3Label"));
        r3Label->setMinimumSize(QSize(30, 0));
        r3Label->setMaximumSize(QSize(30, 16777215));
        r3Label->setBaseSize(QSize(30, 0));

        horizontalLayout_10->addWidget(r3Label);

        r3Value = new QLineEdit(verticalLayoutWidget);
        r3Value->setObjectName(QString::fromUtf8("r3Value"));

        horizontalLayout_10->addWidget(r3Value);


        verticalLayout_2->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        r4Label = new QLabel(verticalLayoutWidget);
        r4Label->setObjectName(QString::fromUtf8("r4Label"));
        r4Label->setMinimumSize(QSize(30, 0));
        r4Label->setMaximumSize(QSize(30, 16777215));
        r4Label->setBaseSize(QSize(30, 0));

        horizontalLayout_11->addWidget(r4Label);

        r4Value = new QLineEdit(verticalLayoutWidget);
        r4Value->setObjectName(QString::fromUtf8("r4Value"));

        horizontalLayout_11->addWidget(r4Value);


        verticalLayout_2->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        r5Label = new QLabel(verticalLayoutWidget);
        r5Label->setObjectName(QString::fromUtf8("r5Label"));
        r5Label->setMinimumSize(QSize(30, 0));
        r5Label->setMaximumSize(QSize(30, 16777215));
        r5Label->setBaseSize(QSize(30, 0));

        horizontalLayout_12->addWidget(r5Label);

        r5Value = new QLineEdit(verticalLayoutWidget);
        r5Value->setObjectName(QString::fromUtf8("r5Value"));

        horizontalLayout_12->addWidget(r5Value);


        verticalLayout_2->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        c1Label = new QLabel(verticalLayoutWidget);
        c1Label->setObjectName(QString::fromUtf8("c1Label"));
        c1Label->setMinimumSize(QSize(30, 0));
        c1Label->setMaximumSize(QSize(30, 16777215));
        c1Label->setBaseSize(QSize(50, 0));

        horizontalLayout_13->addWidget(c1Label);

        c1Value = new QLineEdit(verticalLayoutWidget);
        c1Value->setObjectName(QString::fromUtf8("c1Value"));

        horizontalLayout_13->addWidget(c1Value);


        verticalLayout_2->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        c2Label = new QLabel(verticalLayoutWidget);
        c2Label->setObjectName(QString::fromUtf8("c2Label"));
        c2Label->setMinimumSize(QSize(30, 0));
        c2Label->setMaximumSize(QSize(30, 16777215));
        c2Label->setBaseSize(QSize(30, 0));

        horizontalLayout_14->addWidget(c2Label);

        c2Value = new QLineEdit(verticalLayoutWidget);
        c2Value->setObjectName(QString::fromUtf8("c2Value"));

        horizontalLayout_14->addWidget(c2Value);


        verticalLayout_2->addLayout(horizontalLayout_14);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        c3Label = new QLabel(verticalLayoutWidget);
        c3Label->setObjectName(QString::fromUtf8("c3Label"));
        c3Label->setMinimumSize(QSize(30, 0));
        c3Label->setMaximumSize(QSize(30, 16777215));
        c3Label->setBaseSize(QSize(30, 0));

        horizontalLayout_15->addWidget(c3Label);

        c3Value = new QLineEdit(verticalLayoutWidget);
        c3Value->setObjectName(QString::fromUtf8("c3Value"));

        horizontalLayout_15->addWidget(c3Value);


        verticalLayout_2->addLayout(horizontalLayout_15);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        c4Label = new QLabel(verticalLayoutWidget);
        c4Label->setObjectName(QString::fromUtf8("c4Label"));
        c4Label->setMinimumSize(QSize(30, 0));
        c4Label->setMaximumSize(QSize(30, 16777215));
        c4Label->setBaseSize(QSize(30, 0));

        horizontalLayout_16->addWidget(c4Label);

        c4Value = new QLineEdit(verticalLayoutWidget);
        c4Value->setObjectName(QString::fromUtf8("c4Value"));

        horizontalLayout_16->addWidget(c4Value);


        verticalLayout_2->addLayout(horizontalLayout_16);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        c5Label = new QLabel(verticalLayoutWidget);
        c5Label->setObjectName(QString::fromUtf8("c5Label"));
        c5Label->setMinimumSize(QSize(30, 0));
        c5Label->setMaximumSize(QSize(30, 16777215));
        c5Label->setBaseSize(QSize(30, 0));

        horizontalLayout_17->addWidget(c5Label);

        c5Value = new QLineEdit(verticalLayoutWidget);
        c5Value->setObjectName(QString::fromUtf8("c5Value"));

        horizontalLayout_17->addWidget(c5Value);


        verticalLayout_2->addLayout(horizontalLayout_17);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        l1Label = new QLabel(verticalLayoutWidget);
        l1Label->setObjectName(QString::fromUtf8("l1Label"));
        l1Label->setMinimumSize(QSize(30, 0));
        l1Label->setMaximumSize(QSize(30, 16777215));
        l1Label->setBaseSize(QSize(30, 0));

        horizontalLayout_18->addWidget(l1Label);

        l1Value = new QLineEdit(verticalLayoutWidget);
        l1Value->setObjectName(QString::fromUtf8("l1Value"));

        horizontalLayout_18->addWidget(l1Value);


        verticalLayout_2->addLayout(horizontalLayout_18);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        l2Label = new QLabel(verticalLayoutWidget);
        l2Label->setObjectName(QString::fromUtf8("l2Label"));
        l2Label->setMinimumSize(QSize(30, 0));
        l2Label->setMaximumSize(QSize(30, 16777215));
        l2Label->setBaseSize(QSize(30, 0));

        horizontalLayout_19->addWidget(l2Label);

        l2Value = new QLineEdit(verticalLayoutWidget);
        l2Value->setObjectName(QString::fromUtf8("l2Value"));

        horizontalLayout_19->addWidget(l2Value);


        verticalLayout_2->addLayout(horizontalLayout_19);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        l3Label = new QLabel(verticalLayoutWidget);
        l3Label->setObjectName(QString::fromUtf8("l3Label"));
        l3Label->setMinimumSize(QSize(30, 0));
        l3Label->setMaximumSize(QSize(30, 16777215));
        l3Label->setBaseSize(QSize(30, 0));

        horizontalLayout_20->addWidget(l3Label);

        l3Value = new QLineEdit(verticalLayoutWidget);
        l3Value->setObjectName(QString::fromUtf8("l3Value"));

        horizontalLayout_20->addWidget(l3Value);


        verticalLayout_2->addLayout(horizontalLayout_20);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setObjectName(QString::fromUtf8("horizontalLayout_21"));
        l4Label = new QLabel(verticalLayoutWidget);
        l4Label->setObjectName(QString::fromUtf8("l4Label"));
        l4Label->setMinimumSize(QSize(30, 0));
        l4Label->setMaximumSize(QSize(30, 16777215));
        l4Label->setBaseSize(QSize(30, 0));

        horizontalLayout_21->addWidget(l4Label);

        l4Value = new QLineEdit(verticalLayoutWidget);
        l4Value->setObjectName(QString::fromUtf8("l4Value"));

        horizontalLayout_21->addWidget(l4Value);


        verticalLayout_2->addLayout(horizontalLayout_21);

        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setObjectName(QString::fromUtf8("horizontalLayout_22"));
        l5Label = new QLabel(verticalLayoutWidget);
        l5Label->setObjectName(QString::fromUtf8("l5Label"));
        l5Label->setMinimumSize(QSize(30, 0));
        l5Label->setMaximumSize(QSize(30, 16777215));
        l5Label->setBaseSize(QSize(30, 0));

        horizontalLayout_22->addWidget(l5Label);

        l5Value = new QLineEdit(verticalLayoutWidget);
        l5Value->setObjectName(QString::fromUtf8("l5Value"));

        horizontalLayout_22->addWidget(l5Value);


        verticalLayout_2->addLayout(horizontalLayout_22);

        horizontalLayout_24 = new QHBoxLayout();
        horizontalLayout_24->setObjectName(QString::fromUtf8("horizontalLayout_24"));
        rLLabel = new QLabel(verticalLayoutWidget);
        rLLabel->setObjectName(QString::fromUtf8("rLLabel"));
        rLLabel->setMinimumSize(QSize(30, 0));
        rLLabel->setBaseSize(QSize(30, 0));

        horizontalLayout_24->addWidget(rLLabel);

        rLValue = new QLineEdit(verticalLayoutWidget);
        rLValue->setObjectName(QString::fromUtf8("rLValue"));

        horizontalLayout_24->addWidget(rLValue);


        verticalLayout_2->addLayout(horizontalLayout_24);

        resetButton = new QPushButton(verticalLayoutWidget);
        resetButton->setObjectName(QString::fromUtf8("resetButton"));

        verticalLayout_2->addWidget(resetButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(750, 10, 75, 23));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(670, 10, 75, 23));
        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(830, 10, 75, 23));
        ToneStackCalculator->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ToneStackCalculator);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1261, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuMode = new QMenu(menubar);
        menuMode->setObjectName(QString::fromUtf8("menuMode"));
        ToneStackCalculator->setMenuBar(menubar);
        statusbar = new QStatusBar(ToneStackCalculator);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ToneStackCalculator->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuMode->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addAction(actionQuit);
        menuFile->addAction(actionPrint);
        menuFile->addAction(actionSettings);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionTonestack_Help);
        menuMode->addAction(actionTone_Stack_Mode);
        menuMode->addAction(actionActive_Circuits_Mode);
        menuMode->addAction(actionBasic_Electronics_Mode);

        retranslateUi(ToneStackCalculator);

        QMetaObject::connectSlotsByName(ToneStackCalculator);
    } // setupUi

    void retranslateUi(QMainWindow *ToneStackCalculator)
    {
        ToneStackCalculator->setWindowTitle(QCoreApplication::translate("ToneStackCalculator", "ToneStackCalculator", nullptr));
        actionAbout->setText(QCoreApplication::translate("ToneStackCalculator", "About...", nullptr));
        actionQuit->setText(QCoreApplication::translate("ToneStackCalculator", "Quit", nullptr));
#if QT_CONFIG(shortcut)
        actionQuit->setShortcut(QCoreApplication::translate("ToneStackCalculator", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpen->setText(QCoreApplication::translate("ToneStackCalculator", "&Open...", nullptr));
#if QT_CONFIG(shortcut)
        actionOpen->setShortcut(QCoreApplication::translate("ToneStackCalculator", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave->setText(QCoreApplication::translate("ToneStackCalculator", "&Save", nullptr));
#if QT_CONFIG(shortcut)
        actionSave->setShortcut(QCoreApplication::translate("ToneStackCalculator", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave_As->setText(QCoreApplication::translate("ToneStackCalculator", "&Save As...", nullptr));
#if QT_CONFIG(shortcut)
        actionSave_As->setShortcut(QCoreApplication::translate("ToneStackCalculator", "Ctrl+Shift+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPrint->setText(QCoreApplication::translate("ToneStackCalculator", "Print", nullptr));
#if QT_CONFIG(shortcut)
        actionPrint->setShortcut(QCoreApplication::translate("ToneStackCalculator", "Ctrl+P", nullptr));
#endif // QT_CONFIG(shortcut)
        actionTone_Stack_Mode->setText(QCoreApplication::translate("ToneStackCalculator", "Tone Stack Mode", nullptr));
        actionBasic_Electronics_Mode->setText(QCoreApplication::translate("ToneStackCalculator", "Basic Electronics Mode", nullptr));
        actionActive_Circuits_Mode->setText(QCoreApplication::translate("ToneStackCalculator", "Active Circuits Mode", nullptr));
        actionUser_Circuits_Mode->setText(QCoreApplication::translate("ToneStackCalculator", "User Circuits Mode", nullptr));
        actionTonestack_Help->setText(QCoreApplication::translate("ToneStackCalculator", "Tonestack Help", nullptr));
#if QT_CONFIG(shortcut)
        actionTonestack_Help->setShortcut(QCoreApplication::translate("ToneStackCalculator", "F1", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSettings->setText(QCoreApplication::translate("ToneStackCalculator", "Settings...", nullptr));
        label->setText(QCoreApplication::translate("ToneStackCalculator", "Circuit:", nullptr));
        stackSelection->setPlaceholderText(QCoreApplication::translate("ToneStackCalculator", "Select...", nullptr));
        circuitDisplay->setText(QString());
        pot1Label->setText(QCoreApplication::translate("ToneStackCalculator", "Bass", nullptr));
        pot1Type->setItemText(0, QCoreApplication::translate("ToneStackCalculator", "Lin", nullptr));
        pot1Type->setItemText(1, QCoreApplication::translate("ToneStackCalculator", "Log", nullptr));
        pot1Type->setItemText(2, QCoreApplication::translate("ToneStackCalculator", "Log A", nullptr));
        pot1Type->setItemText(3, QCoreApplication::translate("ToneStackCalculator", "Reverse Log", nullptr));
        pot1Type->setItemText(4, QCoreApplication::translate("ToneStackCalculator", "S Type", nullptr));

        pot2Label->setText(QCoreApplication::translate("ToneStackCalculator", "Mid", nullptr));
        pot2Type->setItemText(0, QCoreApplication::translate("ToneStackCalculator", "Lin", nullptr));
        pot2Type->setItemText(1, QCoreApplication::translate("ToneStackCalculator", "Log", nullptr));
        pot2Type->setItemText(2, QCoreApplication::translate("ToneStackCalculator", "Log A", nullptr));
        pot2Type->setItemText(3, QCoreApplication::translate("ToneStackCalculator", "Reverse Log", nullptr));
        pot2Type->setItemText(4, QCoreApplication::translate("ToneStackCalculator", "S Type", nullptr));

        pot3Label->setText(QCoreApplication::translate("ToneStackCalculator", "Treble", nullptr));
        pot3Type->setItemText(0, QCoreApplication::translate("ToneStackCalculator", "Lin", nullptr));
        pot3Type->setItemText(1, QCoreApplication::translate("ToneStackCalculator", "Log", nullptr));
        pot3Type->setItemText(2, QCoreApplication::translate("ToneStackCalculator", "Log A", nullptr));
        pot3Type->setItemText(3, QCoreApplication::translate("ToneStackCalculator", "Reverse Log", nullptr));
        pot3Type->setItemText(4, QCoreApplication::translate("ToneStackCalculator", "S Type", nullptr));

        label_2->setText(QCoreApplication::translate("ToneStackCalculator", "Circuit Parameters", nullptr));
        rSLabel->setText(QCoreApplication::translate("ToneStackCalculator", "RS", nullptr));
        r1Label->setText(QCoreApplication::translate("ToneStackCalculator", "R1", nullptr));
        r2Label->setText(QCoreApplication::translate("ToneStackCalculator", "R2", nullptr));
        r3Label->setText(QCoreApplication::translate("ToneStackCalculator", "R3", nullptr));
        r4Label->setText(QCoreApplication::translate("ToneStackCalculator", "R4", nullptr));
        r5Label->setText(QCoreApplication::translate("ToneStackCalculator", "R5", nullptr));
        c1Label->setText(QCoreApplication::translate("ToneStackCalculator", "C1", nullptr));
        c2Label->setText(QCoreApplication::translate("ToneStackCalculator", "C2", nullptr));
        c3Label->setText(QCoreApplication::translate("ToneStackCalculator", "C3", nullptr));
        c4Label->setText(QCoreApplication::translate("ToneStackCalculator", "C4", nullptr));
        c5Label->setText(QCoreApplication::translate("ToneStackCalculator", "C5", nullptr));
        l1Label->setText(QCoreApplication::translate("ToneStackCalculator", "L1", nullptr));
        l2Label->setText(QCoreApplication::translate("ToneStackCalculator", "L2", nullptr));
        l3Label->setText(QCoreApplication::translate("ToneStackCalculator", "L3", nullptr));
        l4Label->setText(QCoreApplication::translate("ToneStackCalculator", "L4", nullptr));
        l5Label->setText(QCoreApplication::translate("ToneStackCalculator", "L5", nullptr));
        rLLabel->setText(QCoreApplication::translate("ToneStackCalculator", "RL", nullptr));
        resetButton->setText(QCoreApplication::translate("ToneStackCalculator", "Reset", nullptr));
        pushButton->setText(QCoreApplication::translate("ToneStackCalculator", "Snapshot", nullptr));
        pushButton_2->setText(QCoreApplication::translate("ToneStackCalculator", "Colour", nullptr));
        pushButton_3->setText(QCoreApplication::translate("ToneStackCalculator", "Clear", nullptr));
        menuFile->setTitle(QCoreApplication::translate("ToneStackCalculator", "File", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("ToneStackCalculator", "Help", nullptr));
        menuMode->setTitle(QCoreApplication::translate("ToneStackCalculator", "Mode", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ToneStackCalculator: public Ui_ToneStackCalculator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TONESTACKCALCULATOR_H
