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

    void on_actionSave_screenshot_double_resolution_triggered();

    void on_actionRight_overlay_triggered();

    void on_actionLeft_triggered();

    void on_actionRight_triggered();

    void on_actionSuperior_triggered();

    void on_actionInferior_triggered();

    void on_actionAnterior_triggered();

    void on_actionPosterior_triggered();

    void on_actionSave_view_triggered();

    void on_actionLoad_view_triggered();

    void on_actionChange_background_color_triggered();

    void on_actionSave_left_ROI_triggered();

    void on_actionSave_right_ROI_triggered();

    void on_actionLoad_left_ROI_triggered();

    void on_actionLoad_right_ROI_triggered();

    void on_actionClear_left_ROI_triggered();

    void on_actionClear_Right_ROI_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
