/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QRadioButton *ServerRadio;
    QRadioButton *ClientRadio;
    QLineEdit *ClientIPLine;
    QLabel *label;
    QSpinBox *PortBox;
    QPushButton *ConnectButton;
    QPushButton *PauseButton;
    QPushButton *ResendButton;
    QLineEdit *FrameLine;
    QTextEdit *MessageText;
    QRadioButton *NormalSendRadio;
    QRadioButton *LackSendRadio;
    QLineEdit *LackFrameLine;
    QLabel *label_2;
    QSpinBox *WindowSIzeBox;
    QLabel *label_3;
    QPushButton *SendButton;
    QPushButton *ClearButton;
    QLabel *SpeedLabel;
    QComboBox *ServerIPLine;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(331, 722);
        QFont font;
        font.setPointSize(12);
        MainWindow->setFont(font);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        ServerRadio = new QRadioButton(centralwidget);
        ServerRadio->setObjectName(QString::fromUtf8("ServerRadio"));
        ServerRadio->setGeometry(QRect(20, 10, 141, 31));
        ServerRadio->setChecked(true);
        ClientRadio = new QRadioButton(centralwidget);
        ClientRadio->setObjectName(QString::fromUtf8("ClientRadio"));
        ClientRadio->setGeometry(QRect(20, 46, 141, 31));
        ClientIPLine = new QLineEdit(centralwidget);
        ClientIPLine->setObjectName(QString::fromUtf8("ClientIPLine"));
        ClientIPLine->setGeometry(QRect(170, 50, 141, 31));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 90, 54, 31));
        PortBox = new QSpinBox(centralwidget);
        PortBox->setObjectName(QString::fromUtf8("PortBox"));
        PortBox->setGeometry(QRect(90, 90, 71, 31));
        PortBox->setMaximum(100000);
        PortBox->setValue(8082);
        ConnectButton = new QPushButton(centralwidget);
        ConnectButton->setObjectName(QString::fromUtf8("ConnectButton"));
        ConnectButton->setGeometry(QRect(220, 170, 91, 31));
        PauseButton = new QPushButton(centralwidget);
        PauseButton->setObjectName(QString::fromUtf8("PauseButton"));
        PauseButton->setGeometry(QRect(20, 130, 61, 31));
        ResendButton = new QPushButton(centralwidget);
        ResendButton->setObjectName(QString::fromUtf8("ResendButton"));
        ResendButton->setGeometry(QRect(100, 130, 61, 31));
        FrameLine = new QLineEdit(centralwidget);
        FrameLine->setObjectName(QString::fromUtf8("FrameLine"));
        FrameLine->setGeometry(QRect(170, 130, 141, 31));
        MessageText = new QTextEdit(centralwidget);
        MessageText->setObjectName(QString::fromUtf8("MessageText"));
        MessageText->setGeometry(QRect(20, 290, 291, 371));
        NormalSendRadio = new QRadioButton(centralwidget);
        NormalSendRadio->setObjectName(QString::fromUtf8("NormalSendRadio"));
        NormalSendRadio->setGeometry(QRect(20, 170, 91, 31));
        NormalSendRadio->setChecked(false);
        LackSendRadio = new QRadioButton(centralwidget);
        LackSendRadio->setObjectName(QString::fromUtf8("LackSendRadio"));
        LackSendRadio->setGeometry(QRect(120, 170, 91, 31));
        LackFrameLine = new QLineEdit(centralwidget);
        LackFrameLine->setObjectName(QString::fromUtf8("LackFrameLine"));
        LackFrameLine->setGeometry(QRect(120, 210, 111, 31));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 210, 81, 31));
        WindowSIzeBox = new QSpinBox(centralwidget);
        WindowSIzeBox->setObjectName(QString::fromUtf8("WindowSIzeBox"));
        WindowSIzeBox->setGeometry(QRect(270, 90, 41, 31));
        WindowSIzeBox->setMaximum(5);
        WindowSIzeBox->setValue(2);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(170, 90, 101, 31));
        SendButton = new QPushButton(centralwidget);
        SendButton->setObjectName(QString::fromUtf8("SendButton"));
        SendButton->setGeometry(QRect(250, 210, 61, 31));
        ClearButton = new QPushButton(centralwidget);
        ClearButton->setObjectName(QString::fromUtf8("ClearButton"));
        ClearButton->setGeometry(QRect(20, 250, 91, 31));
        SpeedLabel = new QLabel(centralwidget);
        SpeedLabel->setObjectName(QString::fromUtf8("SpeedLabel"));
        SpeedLabel->setGeometry(QRect(140, 255, 171, 21));
        ServerIPLine = new QComboBox(centralwidget);
        ServerIPLine->setObjectName(QString::fromUtf8("ServerIPLine"));
        ServerIPLine->setGeometry(QRect(170, 11, 141, 31));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 331, 27));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        ServerRadio->setText(QApplication::translate("MainWindow", "server  \346\234\254\346\234\272IP\357\274\232", nullptr));
        ClientRadio->setText(QApplication::translate("MainWindow", "client  \345\257\271\346\226\271IP\357\274\232", nullptr));
        label->setText(QApplication::translate("MainWindow", "Port", nullptr));
        ConnectButton->setText(QApplication::translate("MainWindow", "\347\202\271\345\207\273\350\277\236\346\216\245", nullptr));
        PauseButton->setText(QApplication::translate("MainWindow", "\346\232\202\345\201\234", nullptr));
        ResendButton->setText(QApplication::translate("MainWindow", "\351\207\215\345\217\221", nullptr));
        FrameLine->setText(QApplication::translate("MainWindow", "\345\205\26110\345\270\247\346\225\260\346\215\256", nullptr));
        NormalSendRadio->setText(QApplication::translate("MainWindow", "\346\255\243\345\270\270\345\217\221\351\200\201", nullptr));
        LackSendRadio->setText(QApplication::translate("MainWindow", "\347\274\272\345\270\247\345\217\221\351\200\201", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "\346\214\207\345\256\232\347\274\272\345\244\261\345\270\247", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201\347\252\227\345\217\243\345\260\272\345\257\270", nullptr));
        SendButton->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        ClearButton->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272\347\252\227\345\217\243", nullptr));
        SpeedLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
