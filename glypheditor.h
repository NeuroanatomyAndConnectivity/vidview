#ifndef GLYPHEDITOR_H
#define GLYPHEDITOR_H

#include <QWidget>
#include "data.h"

namespace Ui {
    class GlyphEditor;
}

class GlyphEditor : public QWidget
{
    Q_OBJECT

public:
    explicit GlyphEditor(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~GlyphEditor();

    float threshold, radius, normalization, size, minlength;
    int display, color, geometry, glyphstyle, primitives, minlSource;

    void setThreshold(float value);
    void setRadius(float value);
    void setNormalization(float value);
    void setSize(float value);
    void setDisplay(int d);
    void setColor(int c);
    void setGeometry(int g);
    void setGlyphstyle(int g);
    void setPrimitives(int p);
    void setMinlength(float value);
    void setMinlSource(int ms);
    void initParams();
    void setData(Data* data, DisplayParameters* dp);
    Data* data;
    void update();

    void setVM(po::variables_map& vm, po::options_description& desc);
    po::options_description* desc;

private slots:
    void on_thresholdSlider_valueChanged(int value);

    void on_thresholdBox_valueChanged(double );

    void on_radiusSlider_valueChanged(int value);

    void on_radiusBox_valueChanged(double );

    void on_normSlider_valueChanged(int value);

    void on_normBox_valueChanged(double );

    void on_sizeSlider_valueChanged(int value);

    void on_sizeBox_valueChanged(double );

    void on_geometry_toggled(bool checked);

    void on_pies_toggled(bool checked);

    void on_projection_toggled(bool checked);

    void on_points_toggled(bool checked);

    void on_colorBox_activated(int index);

    void on_displayBox_activated(int index);

    void on_geoBox_activated(int index);

    void on_saveButton_clicked();

    void on_loadButton_clicked();

    void on_updateBox_toggled(bool checked);

    void on_minlengthSlider_valueChanged(int value);

    void on_minlengthBox_valueChanged(double arg1);

    void on_minlengthPickBox_activated(int index);

signals:
    void changed();

private:
    Ui::GlyphEditor *ui;
};

#endif // GLYPHEDITOR_H
