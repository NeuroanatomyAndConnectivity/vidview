#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtDebug>
#include "qfiledialog.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) :
    QMainWindow(parent, flags),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOverlay_triggered()
{
    QString filename = QFileDialog::getOpenFileName();
    qDebug() << "load overlay: " << filename;
    GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
    if (filename != "") glw->data->surfset->loadOverlay(filename);
}

void MainWindow::on_actionRight_overlay_triggered()
{
    GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
    if (glw->data->surfsetr){
        QString filename = QFileDialog::getOpenFileName();
        qDebug() << "load overlay: " << filename;
        if (filename != "") glw->data->surfsetr->loadOverlay(filename);
    } else {
        qDebug() << "no right hemisphere loaded...";
    }
}

void MainWindow::on_actionSave_screenshot_triggered()
{
    GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
    QString filename = QFileDialog::getSaveFileName();
    if (filename != "") glw->screenshot(filename);
}

void MainWindow::closeEvent(QCloseEvent *e){
    qApp->quit();
}

void MainWindow::on_actionSave_screenshot_double_resolution_triggered()
{
    GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
    QString filename = QFileDialog::getSaveFileName();
    if (filename != "") glw->screenshot(filename,2);
}


void MainWindow::on_actionLeft_triggered()
{
    GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
    glw->orient("l");
    glw->updateGL();
}

void MainWindow::on_actionRight_triggered()
{
    GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
    glw->orient("r");
    glw->updateGL();
}

void MainWindow::on_actionSuperior_triggered()
{
    GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
    glw->orient("s");
    glw->updateGL();
}

void MainWindow::on_actionInferior_triggered()
{
    GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
    glw->orient("i");
    glw->updateGL();
}

void MainWindow::on_actionAnterior_triggered()
{
    GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
    glw->orient("a");
    glw->updateGL();
}

void MainWindow::on_actionPosterior_triggered()
{
    GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
    glw->orient("p");
    glw->updateGL();
}

void MainWindow::on_actionSave_view_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this);
    if (filename!=""){
        GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
        glw->saveView(filename);
    }
}

void MainWindow::on_actionLoad_view_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this);
    if (filename!=""){
        GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
        glw->loadView(filename);
    }
}

void MainWindow::on_actionChange_background_color_triggered()
{
    GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
    if (glw->bg==1) {
        glw->bg=0;
    } else {
        glw->bg=1;
    }
}

void MainWindow::on_actionSave_left_ROI_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this);
    if (filename!=""){
        GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
        if (glw->data->surfset) glw->data->surfset->saveROI(filename);
    }
}
void MainWindow::on_actionSave_right_ROI_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this);
    if (filename!=""){
        GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
        if (glw->data->surfsetr) glw->data->surfsetr->saveROI(filename);
    }
}

void MainWindow::on_actionLoad_left_ROI_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this);
    if (filename!=""){
        GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
        if (glw->data->surfset) glw->data->surfset->loadROI(filename);
    }
}

void MainWindow::on_actionLoad_right_ROI_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this);
    if (filename!=""){
        GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
        if (glw->data->surfsetr) glw->data->surfsetr->loadROI(filename);
    }
}

void MainWindow::on_actionClear_left_ROI_triggered()
{
    GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
    if (glw->data->surfset) glw->data->surfset->roi->clear();
}

void MainWindow::on_actionClear_Right_ROI_triggered()
{
    GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
    if (glw->data->surfsetr) glw->data->surfsetr->roi->clear();
}

