#ifndef DISPLAYPARAMETERS_H
#define DISPLAYPARAMETERS_H

#include <QWidget>

#include "boost/program_options.hpp"

namespace po = boost::program_options;

namespace Ui {
    class DisplayParameters;
}

class DisplayParameters : public QWidget
{
    Q_OBJECT

public:
    explicit DisplayParameters(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~DisplayParameters();

    float left,right,glyphs;
    void setVM(po::variables_map& vm, po::options_description& desc);
    po::options_description* desc;

signals:
    void changed();

private slots:
    void on_rightSlider_valueChanged(int value);

    void on_rightBox_valueChanged(double );

    void on_leftSlider_valueChanged(int value);

    void on_leftBox_valueChanged(double );

    void on_glyphSlider_valueChanged(int value);

    void on_glyphBox_valueChanged(double );

    void on_saveButton_clicked();

    void on_loadButton_clicked(bool checked);

    void on_cursorBox_toggled(bool checked);

    void on_leftBox_2_toggled(bool checked);

    void on_rightBox_2_toggled(bool checked);

    void on_connectivityBox_toggled(bool checked);

private:
    Ui::DisplayParameters *ui;
    void setLeft(float l);
    void setRight(float r);
    void setGlyphs(float g);
};

#endif // DISPLAYPARAMETERS_H
