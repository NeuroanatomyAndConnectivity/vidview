#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *e);

private slots:
    void on_actionOverlay_triggered();

    void on_actionSave_screenshot_triggered();

    void on_actionSave_screenshot_double_reolution_triggered();

    void on_actionRight_overlay_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
