#include "displayparameters.h"
#include "ui_displayparameters.h"

#include "qdebug.h"
#include "qfiledialog.h"
#include "iostream"
#include "fstream"

using namespace std;

DisplayParameters::DisplayParameters(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    ui(new Ui::DisplayParameters)
{
    ui->setupUi(this);
    left = 1;
    right = 1;
    glyphs = 1;
}

void DisplayParameters::setVM(po::variables_map& vm, po::options_description& desc){
    this->desc = &desc;
    if (vm.count("leftalpha")) setLeft(vm["leftalpha"].as<float>());
    if (vm.count("rightalpha")) setRight(vm["rightalpha"].as<float>());
    if (vm.count("glyphs")) setGlyphs(vm["glyphs"].as<float>());
}

DisplayParameters::~DisplayParameters()
{
    delete ui;
}

void DisplayParameters::setLeft(float l){
    left = l;
    QDoubleSpinBox* b = findChild<QDoubleSpinBox*>("leftBox");
    b->setValue(left);
    QSlider* s = findChild<QSlider*>("leftSlider");
    s->setValue(left*100);
    emit changed();
}

void DisplayParameters::setRight(float r){
    right = r;
    QDoubleSpinBox* b = findChild<QDoubleSpinBox*>("rightBox");
    b->setValue(right);
    QSlider* s = findChild<QSlider*>("rightSlider");
    s->setValue(right*100);
    emit changed();
}

void DisplayParameters::setGlyphs(float g){
    glyphs = g;
    QDoubleSpinBox* b = findChild<QDoubleSpinBox*>("glyphBox");
    b->setValue(glyphs);
    QSlider* s = findChild<QSlider*>("glyphSlider");
    s->setValue(glyphs*100);
    emit changed();
}

void DisplayParameters::on_rightSlider_valueChanged(int value)
{
    setRight(value/100.0);
}

void DisplayParameters::on_rightBox_valueChanged(double value)
{
    setRight(value);
}

void DisplayParameters::on_leftSlider_valueChanged(int value)
{
    setLeft(value/100.0);
}

void DisplayParameters::on_leftBox_valueChanged(double value)
{
    setLeft(value);
}

void DisplayParameters::on_glyphSlider_valueChanged(int value)
{
    setGlyphs(value/100.0);
}

void DisplayParameters::on_glyphBox_valueChanged(double value)
{
    setGlyphs(value);
}

void DisplayParameters::on_saveButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this);
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << "leftalpha = " << left << endl;
    out << "rightalpha = " << right << endl;
    out << "glyphs = " << glyphs << endl;
    file.close();
}

void DisplayParameters::on_loadButton_clicked(bool checked)
{
    po::variables_map vm;
    QString filename = QFileDialog::getOpenFileName();
    string stdstr = filename.toStdString();
    ifstream ifs(stdstr.c_str());
    if (!ifs) {
        cout << "can not open config file: " << stdstr << "\n";
    } else {
        store(parse_config_file(ifs, *desc), vm);
        notify(vm);
    }
    setVM(vm,*desc);
}

void DisplayParameters::on_cursorBox_toggled(bool checked)
{
    emit changed();
}

void DisplayParameters::on_leftBox_2_toggled(bool checked)
{
    emit changed();
}

void DisplayParameters::on_rightBox_2_toggled(bool checked)
{
    emit changed();
}

void DisplayParameters::on_connectivityBox_toggled(bool checked)
{
    emit changed();
}
