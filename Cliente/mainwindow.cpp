#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui-> stackedWidget-> setCurrentIndex(0);
    inicialListaContactos();
    connect(ui->botonEnviar,SIGNAL(clicked()),this,SLOT(on_botonEnviar_Clicked()));
    connect(ui->botonBuscar,SIGNAL(clicked()),this,SLOT(on_botonBuscar_Clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}







// Método para configurar la tabla (columnas y cabeceras)
void MainWindow::configurarTabla()
{
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels({"ID", "Nombre", "Mensaje"});
    ui->tableWidget->setRowCount(0);

    // Habilitar selección de celdas y visibilidad
    //ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setShowGrid(true);

}

// Método para limpiar el campo de texto
void MainWindow::limpiarCampoTexto()
{
    ui->textEnviar->clear();
}

// Método para enviar mensaje y añadirlo a la tabla
void MainWindow::on_botonEnviar_Clicked()
{
    QString texto = ui->textEnviar->toPlainText().trimmed();

    if (texto.isEmpty()) {
        qDebug() << "El mensaje está vacío.";
        return;
    }

    qDebug() << "Texto ingresado:" << texto;

    // Verificar que la tabla está inicializada
    if (!ui->tableWidget) {
        qDebug() << "La tabla no está inicializada.";
        return;
    }

    // Añadir una nueva fila
    int rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowCount);
    qDebug() << "Fila añadida en la posición:" << rowCount;

    // Crear las celdas de la fila (FORZANDO SU CREACIÓN)
    ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem());
    ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem());
    ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem());

    // Verificar que las celdas están correctamente creadas
    if (!ui->tableWidget->item(rowCount, 0) ||
        !ui->tableWidget->item(rowCount, 1) ||
        !ui->tableWidget->item(rowCount, 2)) {
        qDebug() << "Error crítico: Las celdas no se crearon correctamente.";
        return;
    }

    time_t ahora= time(0);
    char* fecha= ctime(&ahora);

    // Insertar texto en las celdas
    ui->tableWidget->item(rowCount, 0)->setText(fecha);
    ui->tableWidget->item(rowCount, 1)->setText("Usuario");
    ui->tableWidget->item(rowCount, 2)->setText(texto);

    qDebug() << "Texto añadido correctamente a la tabla.";

    // Forzar la actualización visual
    ui->tableWidget->scrollToBottom();
    ui->tableWidget->viewport()->update();
    ui->tableWidget->repaint();

    // Limpiar el campo de texto después de enviar
    ui->textEnviar->clear();
}

// Método para buscar en la tabla
void MainWindow::on_botonBuscar_Clicked()
{
    QString textoBusqueda = ui->textEdit_2->toPlainText().trimmed();
    if (textoBusqueda.isEmpty()) {
        qDebug() << "No se ha ingresado texto para buscar.";
        return;
    }


}

// Método que realiza la búsqueda en la tabla
void MainWindow::buscarEnTabla(const QString &texto)
{
    bool encontrado = false;
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        for (int j = 0; j < ui->tableWidget->columnCount(); ++j) {
            QTableWidgetItem *item = ui->tableWidget->item(i, j);
            if (item && item->text().contains(texto, Qt::CaseInsensitive)) {
                item->setBackground(Qt::yellow);  // Resaltar la celda encontrada
                encontrado = true;
            } else {
                if (item) {
                    item->setBackground(Qt::white); // Restaurar color si no coincide
                }
            }
        }
    }

    if (encontrado) {
        qDebug() << "Se encontraron coincidencias para: " << texto;
    } else {
        qDebug() << "No se encontraron coincidencias para: " << texto;
    }
}

void MainWindow::on_iniciarSesionBtn_clicked()
{
    ui-> stackedWidget-> setCurrentIndex(1);
}

void MainWindow::on_registrarNuevoUsuarioBtn_clicked()
{
    ui-> stackedWidget-> setCurrentIndex(3);
}


void MainWindow::inicialListaContactos(){
    ui->listWidgetContactos->addItem("Madre");
    ui->listWidgetContactos->addItem("Pedro");
    ui->listWidgetContactos->addItem("Manuela");
}



void MainWindow::on_pushButtonAadirADD_clicked()
{
    ui-> stackedWidget-> setCurrentIndex(1);
}


void MainWindow::on_pushButtonAadir_clicked()
{
    ui-> stackedWidget-> setCurrentIndex(2);

}


void MainWindow::on_pushButtonEliminarContacto_clicked()
{
    delete ui->listWidgetContactos->currentItem();

}




//REGISTRO
void MainWindow::on_pushButton_ConfirmarRegistro_clicked()
{
    ui-> stackedWidget-> setCurrentIndex(1);
}

















