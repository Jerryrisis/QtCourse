/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QPushButton *btnNum5;
    QPushButton *btnMultiple;
    QPushButton *btnInverse;
    QPushButton *btnClearAll;
    QPushButton *btnPeriod;
    QPushButton *btnNum7;
    QPushButton *btnDivide;
    QPushButton *btnEqual;
    QPushButton *btnPercentage;
    QPushButton *btnNum0;
    QPushButton *btnNum6;
    QPushButton *btnNum1;
    QPushButton *btnNum8;
    QPushButton *btnClear;
    QPushButton *btnNum9;
    QPushButton *btnSqrt;
    QPushButton *btnDel;
    QPushButton *btnNum3;
    QPushButton *btnPlus;
    QPushButton *btnSquare;
    QPushButton *btnMinus;
    QPushButton *btnNum2;
    QPushButton *btnNum4;
    QPushButton *btnSign;
    QLineEdit *Display;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(669, 528);
        QIcon icon;
        icon.addFile(QString::fromUtf8("C:/Users/Jerry/Desktop/qq\345\244\264\345\203\217.jpg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"	font-size:48pt;\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(3);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(1, 1, 1, 1);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        btnNum5 = new QPushButton(centralwidget);
        btnNum5->setObjectName("btnNum5");
        sizePolicy.setHeightForWidth(btnNum5->sizePolicy().hasHeightForWidth());
        btnNum5->setSizePolicy(sizePolicy);
        btnNum5->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(60, 60, 60);\n"
"	font-size:16pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(50, 50, 50);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(45, 38, 36);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnNum5, 4, 1, 1, 1);

        btnMultiple = new QPushButton(centralwidget);
        btnMultiple->setObjectName("btnMultiple");
        sizePolicy.setHeightForWidth(btnMultiple->sizePolicy().hasHeightForWidth());
        btnMultiple->setSizePolicy(sizePolicy);
        btnMultiple->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(49, 49, 49);\n"
"	font-size:12pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(60, 60, 60);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(49, 49, 49);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnMultiple, 3, 3, 1, 1);

        btnInverse = new QPushButton(centralwidget);
        btnInverse->setObjectName("btnInverse");
        sizePolicy.setHeightForWidth(btnInverse->sizePolicy().hasHeightForWidth());
        btnInverse->setSizePolicy(sizePolicy);
        btnInverse->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(49, 49, 49);\n"
"	font-size:12pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(60, 60, 60);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(49, 49, 49);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnInverse, 2, 0, 1, 1);

        btnClearAll = new QPushButton(centralwidget);
        btnClearAll->setObjectName("btnClearAll");
        sizePolicy.setHeightForWidth(btnClearAll->sizePolicy().hasHeightForWidth());
        btnClearAll->setSizePolicy(sizePolicy);
        btnClearAll->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(49, 49, 49);\n"
"	font-size:12pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(60, 60, 60);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(49, 49, 49);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnClearAll, 1, 1, 1, 1);

        btnPeriod = new QPushButton(centralwidget);
        btnPeriod->setObjectName("btnPeriod");
        sizePolicy.setHeightForWidth(btnPeriod->sizePolicy().hasHeightForWidth());
        btnPeriod->setSizePolicy(sizePolicy);
        btnPeriod->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(60, 60, 60);\n"
"	font-size:16pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(50, 50, 50);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(45, 38, 36);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnPeriod, 6, 2, 1, 1);

        btnNum7 = new QPushButton(centralwidget);
        btnNum7->setObjectName("btnNum7");
        sizePolicy.setHeightForWidth(btnNum7->sizePolicy().hasHeightForWidth());
        btnNum7->setSizePolicy(sizePolicy);
        btnNum7->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(60, 60, 60);\n"
"	font-size:16pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(50, 50, 50);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(45, 38, 36);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnNum7, 3, 0, 1, 1);

        btnDivide = new QPushButton(centralwidget);
        btnDivide->setObjectName("btnDivide");
        sizePolicy.setHeightForWidth(btnDivide->sizePolicy().hasHeightForWidth());
        btnDivide->setSizePolicy(sizePolicy);
        btnDivide->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(49, 49, 49);\n"
"	font-size:12pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(60, 60, 60);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(49, 49, 49);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnDivide, 2, 3, 1, 1);

        btnEqual = new QPushButton(centralwidget);
        btnEqual->setObjectName("btnEqual");
        sizePolicy.setHeightForWidth(btnEqual->sizePolicy().hasHeightForWidth());
        btnEqual->setSizePolicy(sizePolicy);
        btnEqual->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(150, 199, 216);\n"
"	font-size:12pt;\n"
"	color:black;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(138, 182, 197);\n"
"	color:black;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(126, 165, 179);\n"
"	color:rgb(81, 105, 114);\n"
"}"));

        gridLayout->addWidget(btnEqual, 6, 3, 1, 1);

        btnPercentage = new QPushButton(centralwidget);
        btnPercentage->setObjectName("btnPercentage");
        sizePolicy.setHeightForWidth(btnPercentage->sizePolicy().hasHeightForWidth());
        btnPercentage->setSizePolicy(sizePolicy);
        btnPercentage->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(49, 49, 49);\n"
"	font-size:12pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(60, 60, 60);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(49, 49, 49);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnPercentage, 1, 0, 1, 1);

        btnNum0 = new QPushButton(centralwidget);
        btnNum0->setObjectName("btnNum0");
        sizePolicy.setHeightForWidth(btnNum0->sizePolicy().hasHeightForWidth());
        btnNum0->setSizePolicy(sizePolicy);
        btnNum0->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(60, 60, 60);\n"
"	font-size:16pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(50, 50, 50);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(45, 38, 36);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnNum0, 6, 1, 1, 1);

        btnNum6 = new QPushButton(centralwidget);
        btnNum6->setObjectName("btnNum6");
        sizePolicy.setHeightForWidth(btnNum6->sizePolicy().hasHeightForWidth());
        btnNum6->setSizePolicy(sizePolicy);
        btnNum6->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(60, 60, 60);\n"
"	font-size:16pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(50, 50, 50);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(45, 38, 36);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnNum6, 4, 2, 1, 1);

        btnNum1 = new QPushButton(centralwidget);
        btnNum1->setObjectName("btnNum1");
        sizePolicy.setHeightForWidth(btnNum1->sizePolicy().hasHeightForWidth());
        btnNum1->setSizePolicy(sizePolicy);
        btnNum1->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(60, 60, 60);\n"
"	font-size:16pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(50, 50, 50);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(45, 38, 36);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnNum1, 5, 0, 1, 1);

        btnNum8 = new QPushButton(centralwidget);
        btnNum8->setObjectName("btnNum8");
        sizePolicy.setHeightForWidth(btnNum8->sizePolicy().hasHeightForWidth());
        btnNum8->setSizePolicy(sizePolicy);
        btnNum8->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(60, 60, 60);\n"
"	font-size:16pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(50, 50, 50);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(45, 38, 36);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnNum8, 3, 1, 1, 1);

        btnClear = new QPushButton(centralwidget);
        btnClear->setObjectName("btnClear");
        sizePolicy.setHeightForWidth(btnClear->sizePolicy().hasHeightForWidth());
        btnClear->setSizePolicy(sizePolicy);
        btnClear->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(49, 49, 49);\n"
"	font-size:12pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(60, 60, 60);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(49, 49, 49);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnClear, 1, 2, 1, 1);

        btnNum9 = new QPushButton(centralwidget);
        btnNum9->setObjectName("btnNum9");
        sizePolicy.setHeightForWidth(btnNum9->sizePolicy().hasHeightForWidth());
        btnNum9->setSizePolicy(sizePolicy);
        btnNum9->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(60, 60, 60);\n"
"	font-size:16pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(50, 50, 50);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(45, 38, 36);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnNum9, 3, 2, 1, 1);

        btnSqrt = new QPushButton(centralwidget);
        btnSqrt->setObjectName("btnSqrt");
        sizePolicy.setHeightForWidth(btnSqrt->sizePolicy().hasHeightForWidth());
        btnSqrt->setSizePolicy(sizePolicy);
        btnSqrt->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(49, 49, 49);\n"
"	font-size:12pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(60, 60, 60);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(49, 49, 49);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnSqrt, 2, 2, 1, 1);

        btnDel = new QPushButton(centralwidget);
        btnDel->setObjectName("btnDel");
        sizePolicy.setHeightForWidth(btnDel->sizePolicy().hasHeightForWidth());
        btnDel->setSizePolicy(sizePolicy);
        btnDel->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(49, 49, 49);\n"
"	font-size:12pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(60, 60, 60);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(49, 49, 49);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnDel, 1, 3, 1, 1);

        btnNum3 = new QPushButton(centralwidget);
        btnNum3->setObjectName("btnNum3");
        sizePolicy.setHeightForWidth(btnNum3->sizePolicy().hasHeightForWidth());
        btnNum3->setSizePolicy(sizePolicy);
        btnNum3->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(60, 60, 60);\n"
"	font-size:16pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(50, 50, 50);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(45, 38, 36);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnNum3, 5, 2, 1, 1);

        btnPlus = new QPushButton(centralwidget);
        btnPlus->setObjectName("btnPlus");
        sizePolicy.setHeightForWidth(btnPlus->sizePolicy().hasHeightForWidth());
        btnPlus->setSizePolicy(sizePolicy);
        btnPlus->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(49, 49, 49);\n"
"	font-size:12pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(60, 60, 60);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(49, 49, 49);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnPlus, 5, 3, 1, 1);

        btnSquare = new QPushButton(centralwidget);
        btnSquare->setObjectName("btnSquare");
        sizePolicy.setHeightForWidth(btnSquare->sizePolicy().hasHeightForWidth());
        btnSquare->setSizePolicy(sizePolicy);
        btnSquare->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(49, 49, 49);\n"
"	font-size:12pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(60, 60, 60);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(49, 49, 49);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnSquare, 2, 1, 1, 1);

        btnMinus = new QPushButton(centralwidget);
        btnMinus->setObjectName("btnMinus");
        sizePolicy.setHeightForWidth(btnMinus->sizePolicy().hasHeightForWidth());
        btnMinus->setSizePolicy(sizePolicy);
        btnMinus->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(49, 49, 49);\n"
"	font-size:12pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(60, 60, 60);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(49, 49, 49);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnMinus, 4, 3, 1, 1);

        btnNum2 = new QPushButton(centralwidget);
        btnNum2->setObjectName("btnNum2");
        sizePolicy.setHeightForWidth(btnNum2->sizePolicy().hasHeightForWidth());
        btnNum2->setSizePolicy(sizePolicy);
        btnNum2->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(60, 60, 60);\n"
"	font-size:16pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(50, 50, 50);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(45, 38, 36);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnNum2, 5, 1, 1, 1);

        btnNum4 = new QPushButton(centralwidget);
        btnNum4->setObjectName("btnNum4");
        sizePolicy.setHeightForWidth(btnNum4->sizePolicy().hasHeightForWidth());
        btnNum4->setSizePolicy(sizePolicy);
        btnNum4->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(60, 60, 60);\n"
"	font-size:16pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(50, 50, 50);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(45, 38, 36);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnNum4, 4, 0, 1, 1);

        btnSign = new QPushButton(centralwidget);
        btnSign->setObjectName("btnSign");
        sizePolicy.setHeightForWidth(btnSign->sizePolicy().hasHeightForWidth());
        btnSign->setSizePolicy(sizePolicy);
        btnSign->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(60, 60, 60);\n"
"	font-size:16pt;\n"
"	color:white;\n"
"	border-radius:2px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color:rgb(50, 50, 50);\n"
"	color:white;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color:rgb(45, 38, 36);\n"
"	color:white;\n"
"}"));

        gridLayout->addWidget(btnSign, 6, 0, 1, 1);

        Display = new QLineEdit(centralwidget);
        Display->setObjectName("Display");
        sizePolicy.setHeightForWidth(Display->sizePolicy().hasHeightForWidth());
        Display->setSizePolicy(sizePolicy);
        Display->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        gridLayout->addWidget(Display, 0, 0, 1, 4);


        verticalLayout->addLayout(gridLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 669, 20));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\350\256\241\347\256\227\345\231\250-\346\236\227\344\277\212\351\237\254-2023414300210", nullptr));
        btnNum5->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        btnMultiple->setText(QCoreApplication::translate("MainWindow", "\303\227", nullptr));
        btnInverse->setText(QCoreApplication::translate("MainWindow", "1/\317\207", nullptr));
        btnClearAll->setText(QCoreApplication::translate("MainWindow", "CE", nullptr));
        btnPeriod->setText(QCoreApplication::translate("MainWindow", ".", nullptr));
        btnNum7->setText(QCoreApplication::translate("MainWindow", "7", nullptr));
        btnDivide->setText(QCoreApplication::translate("MainWindow", "/", nullptr));
        btnEqual->setText(QCoreApplication::translate("MainWindow", "=", nullptr));
        btnPercentage->setText(QCoreApplication::translate("MainWindow", "%", nullptr));
        btnNum0->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        btnNum6->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        btnNum1->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        btnNum8->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        btnClear->setText(QCoreApplication::translate("MainWindow", "C", nullptr));
        btnNum9->setText(QCoreApplication::translate("MainWindow", "9", nullptr));
        btnSqrt->setText(QCoreApplication::translate("MainWindow", "\342\210\232", nullptr));
        btnDel->setText(QCoreApplication::translate("MainWindow", "\342\214\253", nullptr));
        btnNum3->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        btnPlus->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        btnSquare->setText(QCoreApplication::translate("MainWindow", "\317\207^2", nullptr));
        btnMinus->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        btnNum2->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        btnNum4->setText(QCoreApplication::translate("MainWindow", "4", nullptr));
        btnSign->setText(QCoreApplication::translate("MainWindow", "+/-", nullptr));
        Display->setText(QString());
        Display->setPlaceholderText(QCoreApplication::translate("MainWindow", "0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
