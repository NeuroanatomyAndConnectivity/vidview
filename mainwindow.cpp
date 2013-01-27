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
    glw->data->surfset->loadOverlay(filename);
}

void MainWindow::on_actionRight_overlay_triggered()
{
    GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
    if (glw->data->surfsetr){
        QString filename = QFileDialog::getOpenFileName();
        qDebug() << "load overlay: " << filename;
        glw->data->surfsetr->loadOverlay(filename);
    } else {
        qDebug() << "no right hemisphere loaded...";
    }
}

void MainWindow::on_actionSave_screenshot_triggered()
{
    GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
    QString filename = QFileDialog::getSaveFileName();
    glw->screenshot(filename);
}

void MainWindow::closeEvent(QCloseEvent *e){
    qApp->quit();
}

void MainWindow::on_actionSave_screenshot_double_reolution_triggered()
{
    GLWidget* glw = centralWidget()->findChild<GLWidget*>("widget");
    QString filename = QFileDialog::getSaveFileName();
    glw->screenshot(filename,2);
}

