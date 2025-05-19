/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *page_2;
    QLabel *labelNombreUsuario;
    QLabel *labelContrasea;
    QLineEdit *lineEditContrasea;
    QLineEdit *lineEditUsuario;
    QLabel *labelInicioSesion;
    QPushButton *iniciarSesionBtn;
    QWidget *page;
    QTextEdit *textEdit_2;
    QPushButton *botonBuscar;
    QLabel *label;
    QLabel *label_2;
    QTableWidget *tableWidget;
    QTextEdit *textEnviar;
    QPushButton *botonEnviar;
    QListWidget *listWidgetContactos;
    QPushButton *pushButton_2;
    QPushButton *pushButtonAadir;
    QPushButton *pushButtonEliminarContacto;
    QWidget *page_3;
    QLabel *labelNombreADD;
    QLineEdit *lineEditNombreADD;
    QLabel *labelEmailADD;
    QLineEdit *lineEditEmailADD;
    QLabel *labelTelefonoADD;
    QLineEdit *lineEditTelefonoADD;
    QPushButton *pushButtonAadirADD;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 750);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(0, 0, 1000, 721));
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        labelNombreUsuario = new QLabel(page_2);
        labelNombreUsuario->setObjectName("labelNombreUsuario");
        labelNombreUsuario->setGeometry(QRect(370, 210, 201, 41));
        labelContrasea = new QLabel(page_2);
        labelContrasea->setObjectName("labelContrasea");
        labelContrasea->setGeometry(QRect(370, 300, 161, 31));
        lineEditContrasea = new QLineEdit(page_2);
        lineEditContrasea->setObjectName("lineEditContrasea");
        lineEditContrasea->setGeometry(QRect(370, 340, 251, 28));
        lineEditUsuario = new QLineEdit(page_2);
        lineEditUsuario->setObjectName("lineEditUsuario");
        lineEditUsuario->setGeometry(QRect(370, 260, 251, 28));
        labelInicioSesion = new QLabel(page_2);
        labelInicioSesion->setObjectName("labelInicioSesion");
        labelInicioSesion->setGeometry(QRect(430, 140, 141, 41));
        iniciarSesionBtn = new QPushButton(page_2);
        iniciarSesionBtn->setObjectName("iniciarSesionBtn");
        iniciarSesionBtn->setGeometry(QRect(372, 400, 251, 29));
        stackedWidget->addWidget(page_2);
        page = new QWidget();
        page->setObjectName("page");
        textEdit_2 = new QTextEdit(page);
        textEdit_2->setObjectName("textEdit_2");
        textEdit_2->setGeometry(QRect(30, 60, 271, 51));
        botonBuscar = new QPushButton(page);
        botonBuscar->setObjectName("botonBuscar");
        botonBuscar->setGeometry(QRect(310, 60, 101, 51));
        label = new QLabel(page);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 20, 271, 41));
        label_2 = new QLabel(page);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(470, 20, 271, 41));
        tableWidget = new QTableWidget(page);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(460, 60, 511, 541));
        tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        tableWidget->setColumnCount(3);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(false);
        tableWidget->horizontalHeader()->setDefaultSectionSize(165);
        textEnviar = new QTextEdit(page);
        textEnviar->setObjectName("textEnviar");
        textEnviar->setGeometry(QRect(460, 610, 401, 51));
        botonEnviar = new QPushButton(page);
        botonEnviar->setObjectName("botonEnviar");
        botonEnviar->setGeometry(QRect(870, 610, 101, 51));
        listWidgetContactos = new QListWidget(page);
        listWidgetContactos->setObjectName("listWidgetContactos");
        listWidgetContactos->setGeometry(QRect(30, 120, 381, 481));
        QFont font;
        font.setPointSize(18);
        listWidgetContactos->setFont(font);
        pushButton_2 = new QPushButton(page);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(290, 610, 121, 51));
        pushButtonAadir = new QPushButton(page);
        pushButtonAadir->setObjectName("pushButtonAadir");
        pushButtonAadir->setGeometry(QRect(30, 610, 121, 51));
        pushButtonEliminarContacto = new QPushButton(page);
        pushButtonEliminarContacto->setObjectName("pushButtonEliminarContacto");
        pushButtonEliminarContacto->setGeometry(QRect(160, 610, 121, 51));
        stackedWidget->addWidget(page);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        labelNombreADD = new QLabel(page_3);
        labelNombreADD->setObjectName("labelNombreADD");
        labelNombreADD->setGeometry(QRect(60, 50, 181, 31));
        lineEditNombreADD = new QLineEdit(page_3);
        lineEditNombreADD->setObjectName("lineEditNombreADD");
        lineEditNombreADD->setGeometry(QRect(60, 90, 281, 28));
        labelEmailADD = new QLabel(page_3);
        labelEmailADD->setObjectName("labelEmailADD");
        labelEmailADD->setGeometry(QRect(60, 130, 181, 31));
        lineEditEmailADD = new QLineEdit(page_3);
        lineEditEmailADD->setObjectName("lineEditEmailADD");
        lineEditEmailADD->setGeometry(QRect(60, 170, 281, 28));
        labelTelefonoADD = new QLabel(page_3);
        labelTelefonoADD->setObjectName("labelTelefonoADD");
        labelTelefonoADD->setGeometry(QRect(60, 210, 181, 31));
        lineEditTelefonoADD = new QLineEdit(page_3);
        lineEditTelefonoADD->setObjectName("lineEditTelefonoADD");
        lineEditTelefonoADD->setGeometry(QRect(60, 250, 281, 28));
        pushButtonAadirADD = new QPushButton(page_3);
        pushButtonAadirADD->setObjectName("pushButtonAadirADD");
        pushButtonAadirADD->setGeometry(QRect(70, 320, 231, 29));
        stackedWidget->addWidget(page_3);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1000, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Deusto Messenger", nullptr));
        labelNombreUsuario->setText(QCoreApplication::translate("MainWindow", "Nombre de Usuario", nullptr));
        labelContrasea->setText(QCoreApplication::translate("MainWindow", "Contrase\303\261a", nullptr));
        labelInicioSesion->setText(QCoreApplication::translate("MainWindow", "Inicio de sesion", nullptr));
        iniciarSesionBtn->setText(QCoreApplication::translate("MainWindow", "Iniciar sesion", nullptr));
        botonBuscar->setText(QCoreApplication::translate("MainWindow", "Buscar", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Contactos", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Chat", nullptr));
        textEnviar->setPlaceholderText(QCoreApplication::translate("MainWindow", "Mensaje", nullptr));
        botonEnviar->setText(QCoreApplication::translate("MainWindow", "Enviar", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Chatear", nullptr));
        pushButtonAadir->setText(QCoreApplication::translate("MainWindow", "A\303\261adir", nullptr));
        pushButtonEliminarContacto->setText(QCoreApplication::translate("MainWindow", "Eliminar", nullptr));
        labelNombreADD->setText(QCoreApplication::translate("MainWindow", "Nombre", nullptr));
        labelEmailADD->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        labelTelefonoADD->setText(QCoreApplication::translate("MainWindow", "Telefono", nullptr));
        pushButtonAadirADD->setText(QCoreApplication::translate("MainWindow", "A\303\261adir", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
