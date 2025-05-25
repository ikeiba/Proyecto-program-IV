#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctime>
#include <sstream>
#include "sockets/socket.h"

extern Usuario** usuarios;
extern int numUsuarios;

extern Usuario* cliente;

Grupo* activo;
Grupo* aAñadir;

extern Grupo** grupos;
extern int numGrupos;

extern Mensaje** mensajes;
extern int numMensajes;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui-> stackedWidget-> setCurrentIndex(0);
    connect(ui->botonEnviar,SIGNAL(clicked()),this,SLOT(on_botonEnviar_Clicked()));
    connect(ui->pushButtonChatear,SIGNAL(clicked()),this,SLOT(on_pushButton_Chatear_Clicked()));
    connect(ui->pushBtnAnadirUsu,SIGNAL(clicked()),this,SLOT(on_pushBtnAnadirUsu_Clicked()));
    connect(ui->pushButtonAnadirAGrupo,SIGNAL(clicked()),this,SLOT(on_pushButtonAnadirAGrupo_Clicked()));
    connect(ui->pushButtonRefresh,SIGNAL(clicked()),this,SLOT(on_pushButtonRefresh_Clicked()));

    ui->label_ErrorInicioSes->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}




// Método para configurar la tabla (columnas y cabeceras)
void MainWindow::configurarTabla()
{

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels({"Fecha", "Emisor", "Mensaje"});
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
    struct tm* local = localtime(&ahora);  // Convertir a estructura tm

    char fecha[11];  // YYYY-MM-DD (10 + 1 para null terminator)
    char hora[6];    // HH:MM (5 + 1 para null terminator)

    // Formatear la fecha
    strftime(fecha, sizeof(fecha), "%Y-%m-%d", local);

    // Formatear la hora
    strftime(hora, sizeof(hora), "%H:%M", local);

    // Insertar texto en las celdas
    ui->tableWidget->item(rowCount, 0)->setText(fecha);
    ui->tableWidget->item(rowCount, 1)->setText(cliente->getNombre());
    ui->tableWidget->item(rowCount, 2)->setText(texto);

    qDebug() << "Texto añadido correctamente a la tabla.";
    QByteArray textoByte = texto.toLocal8Bit();  // convierte QString a QByteArray con codificación local
    const char* textoNormal = textoByte.data(); 

    enviarMensaje(fecha, hora, textoNormal, cliente->getId(), activo->getId());
    // Forzar la actualización visual
    ui->tableWidget->scrollToBottom();
    ui->tableWidget->viewport()->update();
    ui->tableWidget->repaint();

    // Limpiar el campo de texto después de enviar
    ui->textEnviar->clear();
}


// Método para cargar mensajes
void MainWindow::on_pushButton_Chatear_Clicked()
{
    if (!ui->tableWidget) {
        qDebug() << "La tabla no está inicializada.";
        return;
    }

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0); // También limpiar las filas
    QString tituloChat;
    QListWidgetItem* chatSeleccionado = ui->listWidgetContactos->currentItem();

    if (!chatSeleccionado) {
        qDebug() << "Ningún chat seleccionado.";
        return;
    }

    tituloChat = chatSeleccionado->text();
    qDebug() << "Texto seleccionado:" << tituloChat;

    Grupo* grupoSeleccionado = nullptr;

    for (int i = 0; i < numGrupos; i++) {
        QString nombreGrupo = QString::fromUtf8(grupos[i]->getNombre());

        if (nombreGrupo.contains(tituloChat, Qt::CaseInsensitive)) {
            grupoSeleccionado = grupos[i];
            qDebug() << "Grupo seleccionado:" << nombreGrupo;
            break;
        }
    }

    if (!grupoSeleccionado) {
        qDebug() << "No se encontró ningún grupo que coincida con el título.";
        return;
    }
    activo=grupoSeleccionado;
    int rowCount = 0;
   for (int i = 0; i < numMensajes; i++) {
    QString nombreMensajeGrupo = QString::fromLatin1(mensajes[i]->getGrupo()->getNombre());
    QString nombreSeleccionado = QString::fromLatin1(grupoSeleccionado->getNombre());

    qDebug() << "Grupo del mensaje:" << mensajes[i]->getContenido();
    qDebug() << "Nombre del grupo del mensaje:" << QString::fromUtf8(mensajes[i]->getGrupo()->getNombre());
    qDebug() << "Grupo seleccionado:" << grupoSeleccionado;
    qDebug() << "Nombre grupo seleccionado:" << QString::fromUtf8(grupoSeleccionado->getNombre());

    if (nombreMensajeGrupo == nombreSeleccionado) {

        Mensaje mensajeActual = *mensajes[i];

        ui->tableWidget->insertRow(rowCount);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem());
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem());
        ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem());

        ui->tableWidget->item(rowCount, 0)->setText(mensajeActual.getFecha());
        ui->tableWidget->item(rowCount, 1)->setText(mensajeActual.getEmisor()->getNombre());
        ui->tableWidget->item(rowCount, 2)->setText(mensajeActual.getContenido());

        rowCount++;
        }
    }

    if (rowCount == 0) {
        qDebug() << "No se encontraron mensajes para este grupo.";
    }

    ui->tableWidget->scrollToBottom();
    ui->textEnviar->clear();
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
    QString textoNombre = ui->textEdit_UsuarioInicioSes->toPlainText().trimmed();
    QString textoContra = ui->textEdit_ContraInicioSes->toPlainText().trimmed();
    if (textoNombre.isEmpty()) {
        qDebug() << "No se ha ingresado Usuario para buscar.";
        return;
    } else if (textoContra.isEmpty()) {
        qDebug() << "No se ha ingresado Contrasena para buscar.";
        return;
    }
    QByteArray byteArray = textoNombre.toUtf8();
    const char* txtNom = byteArray.data();
    QByteArray byteArray2 = textoContra.toUtf8();
    const char* txtCont = byteArray2.data();

    if (inicioSesion(txtNom, txtCont) == 1)
    {
        qDebug() << "Llegooooo";
        getGeneral(txtNom);
        qDebug() << "Llegooooo2";

        inicialListaContactos();
        qDebug() << "Llegooooo3";

        ui-> stackedWidget-> setCurrentIndex(1);
    } else if (inicioSesion(txtNom, txtCont) == 0)
    {
        ui->label_ErrorInicioSes->setVisible(true);
        qDebug() << "Usuario o contrasena incorrectos";
        return; 
    } else {
        ui->label_ErrorInicioSes->setVisible(true);
        qDebug() << "Ocurrio un error al iniciar sesion";
        return; 
    }
    //inicializarTimer();
}

void MainWindow::on_registrarNuevoUsuarioBtn_clicked()
{
        ui-> stackedWidget-> setCurrentIndex(3);
}


void MainWindow::inicialListaContactos(){
    ui->listWidgetContactos->clear();
    for (int i = 0; i < numGrupos; i++)
    {
        ui->listWidgetContactos->addItem(grupos[i]->getNombre());
    }
}



void MainWindow::on_pushButtonAadirADD_clicked()
{
    QString textoGrupo = ui->textEditNombreGrupo->toPlainText().trimmed();
    QString textoDescripcion = ui->textEditDescripcionGrupo->toPlainText().trimmed();

    if (textoGrupo.isEmpty()) {
        ui-> stackedWidget-> setCurrentIndex(1);
        return;
    }

    QByteArray textoByte = textoGrupo.toLocal8Bit();  // convierte QString a QByteArray con codificación local
    char* textoNormalNombre = textoByte.data(); 


    time_t ahora= time(0);
    struct tm* local = localtime(&ahora);  // Convertir a estructura tm

    char fecha[11];  // YYYY-MM-DD (10 + 1 para null terminator)
    char hora[6];    // HH:MM (5 + 1 para null terminator)

    // Formatear la fecha
    strftime(fecha, sizeof(fecha), "%Y-%m-%d", local);

    if (textoDescripcion.isEmpty()) {

        crearGrupo(textoNormalNombre, fecha, cliente->getId(), "");
        qDebug() << "Grupo insertado.";

    }else{

        QByteArray textoByteDesc = textoDescripcion.toLocal8Bit();  // convierte QString a QByteArray con codificación local
        char* textoNormalDesc = textoByte.data(); 

        crearGrupo(textoNormalNombre, fecha, cliente->getId(), textoNormalDesc);
        qDebug() << "Grupo insertado.";

    }
    //getGeneral(cliente->getEmail());
    actualizarDatos();
    inicialListaContactos();
    ui-> stackedWidget-> setCurrentIndex(1);

}


void MainWindow::on_pushButtonAadir_clicked()//Realmente es el de crear pero cambiar nombres da mucho lio
{
    ui-> stackedWidget-> setCurrentIndex(2);

}


void MainWindow::on_pushButtonEliminarContacto_clicked()
{

    QString tituloChat;
    QListWidgetItem* chatSeleccionado = ui->listWidgetContactos->currentItem();

    if (!chatSeleccionado) {
        qDebug() << "Ningún chat seleccionado.";
        return;
    }

    tituloChat = chatSeleccionado->text();
    qDebug() << "Texto seleccionado:" << tituloChat;

    Grupo* grupoSeleccionado = nullptr;

    for (int i = 0; i < numGrupos; i++) {
        QString nombreGrupo = QString::fromUtf8(grupos[i]->getNombre());

        if (nombreGrupo.contains(tituloChat, Qt::CaseInsensitive)) {
            grupoSeleccionado = grupos[i];
            qDebug() << "Grupo seleccionado:" << nombreGrupo;
            break;
        }
    }

    if (!grupoSeleccionado) {
        qDebug() << "No se encontró ningún grupo que coincida con el título.";
        return;
    }

    abandonarGrupo(cliente->getId(), grupoSeleccionado->getId());
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0); // También limpiar las filas
    delete ui->listWidgetContactos->currentItem();
}




//REGISTRO
void MainWindow::on_pushButton_ConfirmarRegistro_clicked()
{
    QString textoContraReg = ui->textEdit_ContraRegistro->toPlainText().trimmed();
    QString textoEmailReg = ui->textEdit_EmailRegistro->toPlainText().trimmed();
    QString textoFechaReg = ui->textEdit_FechaRegistro->toPlainText().trimmed();
    QString textoNombreReg = ui->textEdit_NombreRegistro->toPlainText().trimmed();
    QString textoTlfReg = ui->textEdit_TlfRegistro->toPlainText().trimmed();

    if (    textoContraReg.isEmpty() || textoEmailReg.isEmpty()  || 
            textoFechaReg.isEmpty()  || textoNombreReg.isEmpty() || 
            textoTlfReg.isEmpty()) {
        qDebug() << "No se ha ingresado Usuario para buscar.";
        return;
    }

    QByteArray byteArray3 = textoNombreReg.toUtf8();
    const char* txtNomReg = byteArray3.data();

    QByteArray byteArray4 = textoContraReg.toUtf8();
    const char* txtContReg = byteArray4.data();

    QByteArray byteArray5 = textoEmailReg.toUtf8();
    const char* txtEmailReg = byteArray5.data();

    QByteArray byteArray6 = textoFechaReg.toUtf8();
    const char* txtFechaReg = byteArray6.data();

    QByteArray byteArray7 = textoTlfReg.toUtf8();
    const char* txtTlfReg = byteArray7.data();

    if (registrarse(txtNomReg, txtEmailReg, txtTlfReg, txtFechaReg, txtContReg) == 1)
    {
        ui-> stackedWidget-> setCurrentIndex(0);
        return;

    } else if (registrarse(txtNomReg, txtEmailReg, txtTlfReg, txtFechaReg, txtContReg) == -1)
    {
        qDebug() << "Ocurrio un error al registrar el usuario";
        return;
    }
    
}


void MainWindow::on_pushBtnAnadirUsu_Clicked(){

    QListWidgetItem* chatSeleccionado = ui->listWidgetContactos->currentItem();
    if (!chatSeleccionado) {
        qDebug() << "Ningún chat seleccionado.";
        return;
    }
    QString tituloChat;
    tituloChat = chatSeleccionado->text();
    qDebug() << "Texto seleccionado:" << tituloChat;


    Grupo* grupoSeleccionado = nullptr;

    for (int i = 0; i < numGrupos; i++) {
        QString nombreGrupo = QString::fromUtf8(grupos[i]->getNombre());

        if (nombreGrupo.contains(tituloChat, Qt::CaseInsensitive)) {
            grupoSeleccionado = grupos[i];
            qDebug() << "Grupo seleccionado:" << nombreGrupo;
            break;
        }
    }

    if (!grupoSeleccionado) {
        qDebug() << "No se encontró ningún grupo que coincida con el título.";
        return;
    }

    aAñadir=grupoSeleccionado;
    ui-> stackedWidget-> setCurrentIndex(4);



}

void MainWindow::on_pushButtonAnadirAGrupo_Clicked(){

    QString texto = ui->textEditEmailUsu->toPlainText().trimmed();

    if (texto.isEmpty()) {
        ui-> stackedWidget-> setCurrentIndex(1);
        return;
    }

    QByteArray textoByte = texto.toLocal8Bit();  // convierte QString a QByteArray con codificación local
    char* textoNormal = textoByte.data(); 

    aniadirUsuarioAGrupo(textoNormal,aAñadir->getId());
    ui-> stackedWidget-> setCurrentIndex(1);

}



void MainWindow::on_pushButtonRefresh_Clicked(){
    actualizarDatos();
    inicialListaContactos();
}




#include <QTimer>

void MainWindow::inicializarTimer() {
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::tareaCadaMinuto);
    timer->start(15 * 1000);  // 60 segundos = 60000 ms
}

void MainWindow::tareaCadaMinuto() {
    qDebug() << "Ejecutando tarea cada minuto";
    actualizarDatos();
    inicialListaContactos();
}