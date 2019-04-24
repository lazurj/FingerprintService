/********************************************************************************
** Form generated from reading UI file 'service.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVICE_H
#define UI_SERVICE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Service
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *serverInfoTab;
    QTableWidget *clientTable;
    QWidget *preprocessingTab;
    QLabel *originalImg;
    QLabel *skeletonImg;
    QPushButton *startServerBT;
    QPushButton *stopServerBT;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Service)
    {
        if (Service->objectName().isEmpty())
            Service->setObjectName(QString::fromUtf8("Service"));
        Service->resize(1175, 705);
        centralWidget = new QWidget(Service);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(30, 90, 871, 571));
        serverInfoTab = new QWidget();
        serverInfoTab->setObjectName(QString::fromUtf8("serverInfoTab"));
        clientTable = new QTableWidget(serverInfoTab);
        clientTable->setObjectName(QString::fromUtf8("clientTable"));
        clientTable->setGeometry(QRect(10, 10, 841, 511));
        tabWidget->addTab(serverInfoTab, QString());
        preprocessingTab = new QWidget();
        preprocessingTab->setObjectName(QString::fromUtf8("preprocessingTab"));
        originalImg = new QLabel(preprocessingTab);
        originalImg->setObjectName(QString::fromUtf8("originalImg"));
        originalImg->setGeometry(QRect(20, 20, 271, 421));
        skeletonImg = new QLabel(preprocessingTab);
        skeletonImg->setObjectName(QString::fromUtf8("skeletonImg"));
        skeletonImg->setGeometry(QRect(360, 20, 271, 421));
        tabWidget->addTab(preprocessingTab, QString());
        startServerBT = new QPushButton(centralWidget);
        startServerBT->setObjectName(QString::fromUtf8("startServerBT"));
        startServerBT->setGeometry(QRect(30, 20, 141, 61));
        stopServerBT = new QPushButton(centralWidget);
        stopServerBT->setObjectName(QString::fromUtf8("stopServerBT"));
        stopServerBT->setGeometry(QRect(190, 20, 141, 61));
        Service->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Service);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1175, 22));
        Service->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Service);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        Service->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Service);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Service->setStatusBar(statusBar);

        retranslateUi(Service);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Service);
    } // setupUi

    void retranslateUi(QMainWindow *Service)
    {
        Service->setWindowTitle(QApplication::translate("Service", "Service", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(serverInfoTab), QApplication::translate("Service", "Tab 1", nullptr));
        originalImg->setText(QString());
        skeletonImg->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(preprocessingTab), QApplication::translate("Service", "Tab 2", nullptr));
        startServerBT->setText(QApplication::translate("Service", "START", nullptr));
        stopServerBT->setText(QApplication::translate("Service", "STOP", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Service: public Ui_Service {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVICE_H
