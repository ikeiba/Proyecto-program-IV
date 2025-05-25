#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_botonEnviar_Clicked();
    void on_botonBuscar_Clicked();
    void on_pushButton_Chatear_Clicked();

    void on_pushBtnAnadirUsu_Clicked();
    void on_pushButtonAnadirAGrupo_Clicked();

    void on_iniciarSesionBtn_clicked();
    void on_registrarNuevoUsuarioBtn_clicked();

    void on_pushButtonAadirADD_clicked();

    void on_pushButtonAadir_clicked();
    void on_pushButtonEliminarContacto_clicked();

    void on_pushButton_ConfirmarRegistro_clicked();
    void tareaCadaMinuto();
private:
    Ui::MainWindow *ui;
    void configurarTabla();       // Método para configurar la tabla
    void limpiarCampoTexto();     // Método para limpiar el campo de texto
    void buscarEnTabla(const QString &texto); // Método para buscar en la tabla
    void inicialListaContactos();
    void inicializarTimer();
};
#endif // MAINWINDOW_H
