#include "glypheditor.h"
#include "ui_glypheditor.h"

#include "qfiledialog.h"

#include "qdebug.h"
#include "iostream"
#include "fstream"

using namespace std;

GlyphEditor::GlyphEditor(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    ui(new Ui::GlyphEditor)
{
    ui->setupUi(this);
}

void GlyphEditor::setData(Data* data, DisplayParameters* dp){
    this->data = data;
    GLGlyphView* gv = findChild<GLGlyphView*>("widget");
    gv->setData(data);
    gv->setDisplayParameters(dp);
    QComboBox* col = findChild<QComboBox*>("colorBox");
    QComboBox* disp = findChild<QComboBox*>("displayBox");
    QComboBox* geoBox = findChild<QComboBox*>("geoBox");
    QComboBox* minlBox = findChild<QComboBox*>("minlengthPickBox");
    for (int i = 0; i<data->surfset->afnis.length();i++){
        AFNISurface* afni = data->surfset->afnis.at(i);
        QString displayString = afni->filename;
        col->insertItem(i,displayString);
        disp->insertItem(i,displayString);
        geoBox->insertItem(i,displayString);
        minlBox->insertItem(i,displayString);
    }
}

GlyphEditor::~GlyphEditor()
{
    delete ui;
}

void GlyphEditor::initParams(){
    setThreshold(0.8);
    setRadius(10);
    setNormalization(0.5);
    setSize(2);
    setDisplay(0);
    setColor(0);
    setGeometry(0);
    setGlyphstyle(0);
    setPrimitives(0);
    setMinlength(0);
    setMinlSource(0);
}

void GlyphEditor::setVM(po::variables_map& vm, po::options_description& desc){
    this->desc = &desc;
    if (vm.count("threshold")) setThreshold(vm["threshold"].as<float>());
    if (vm.count("radius")) setRadius(vm["radius"].as<float>());
    if (vm.count("normalization")) setNormalization(vm["normalization"].as<float>());
    if (vm.count("primsize")) setSize(vm["primsize"].as<float>());
    if (vm.count("display")) setDisplay(vm["display"].as<int>());
    if (vm.count("color")) setColor(vm["color"].as<int>());
    if (vm.count("geometry")) setGeometry(vm["geometry"].as<int>());
    if (vm.count("glyphstyle")) setGlyphstyle(vm["glyphstyle"].as<int>());
    if (vm.count("primitives")) setPrimitives(vm["primitives"].as<int>());
    if (vm.count("minlength")) setMinlength(vm["minlength"].as<float>());
    if (vm.count("minlsource")) {
        setMinlSource(vm["minlsource"].as<int>());
        qDebug() << "***** minlsource: " << minlSource;
    }
}

void GlyphEditor::setThreshold(float value){
    threshold = value;
    QDoubleSpinBox* b = findChild<QDoubleSpinBox*>("thresholdBox");
    b->setValue(threshold);
    QSlider* s = findChild<QSlider*>("thresholdSlider");
    s->setValue(threshold*100);
    data->setThreshold(threshold);
    update();
}

void GlyphEditor::setMinlength(float value){
    minlength = value;
    QDoubleSpinBox* b = findChild<QDoubleSpinBox*>("minlengthBox");
    b->setValue(minlength);
    QSlider* s = findChild<QSlider*>("minlengthSlider");
    s->setValue(minlength);
    data->setMinlength(minlength);
    update();
}

void GlyphEditor::setNormalization(float value){
    normalization = value;
    QDoubleSpinBox* b = findChild<QDoubleSpinBox*>("normBox");
    b->setValue(normalization);
    QSlider* s = findChild<QSlider*>("normSlider");
    s->setValue(normalization*100);
    data->setNormalization(normalization);
    update();
}

void GlyphEditor::setRadius(float value){
    radius = value;
    QDoubleSpinBox* b = findChild<QDoubleSpinBox*>("radiusBox");
    b->setValue(radius);
    QSlider* s = findChild<QSlider*>("radiusSlider");
    s->setValue(radius*10);
    data->setGlyphRadius(radius);
    update();
}

void GlyphEditor::setSize(float value){
    size = value;
    QDoubleSpinBox* b = findChild<QDoubleSpinBox*>("sizeBox");
    b->setValue(size);
    QSlider* s = findChild<QSlider*>("sizeSlider");
    s->setValue(size*10);
    data->setSize(size);
    update();
}

void GlyphEditor::setDisplay(int d){
    display = d;
    QComboBox* dcb = findChild<QComboBox*>("displayBox");
    dcb->setCurrentIndex(display);
    qDebug() << "switching to surface: " << display;
    data->switchToSurface(display);
    update();
}

void GlyphEditor::setColor(int c){
    color = c;
    QComboBox* ccb = findChild<QComboBox*>("colorBox");
    ccb->setCurrentIndex(color);
    data->colorFrom(color);
    update();
}

void GlyphEditor::setGeometry(int g){
    geometry = g;
    QComboBox* gcb = findChild<QComboBox*>("geoBox");
    gcb->setCurrentIndex(geometry);
    qDebug() << "geometry from: " << geometry;
    data->geoFrom(geometry);
    update();
}

void GlyphEditor::setGlyphstyle(int g){
    glyphstyle = g;
    if (glyphstyle==0) {
        data->nodestyle = 6;
        if (data->surfset) data->surfset->billboarding = false;
        if (data->surfsetr) data->surfsetr->billboarding = false;
        QRadioButton* grb = findChild<QRadioButton*>("geometry");
        grb->setChecked(true);
    } else if (glyphstyle==1){
        data->nodestyle = 6;
        if (data->surfset) data->surfset->billboarding = true;
        if (data->surfsetr) data->surfsetr->billboarding = true;
        QRadioButton* prb = findChild<QRadioButton*>("projection");
        prb->setChecked(true);
    } else if (glyphstyle==2){
        data->nodestyle = 4;
        if (data->surfset) data->surfset->billboarding = false;
        if (data->surfsetr) data->surfsetr->billboarding = false;
        QRadioButton* pirb = findChild<QRadioButton*>("pies");
        pirb->setChecked(true);
    }
    update();
}

void GlyphEditor::setPrimitives(int p){
    primitives = p;
    if (p==0){
        QRadioButton* prb = findChild<QRadioButton*>("points");
        prb->setChecked(true);
        if (data->surfset) data->surfset->vectors = false;
        if (data->surfsetr) data->surfsetr->vectors = false;
    } else if (p==1){
        QRadioButton* vrb = findChild<QRadioButton*>("vectors");
        vrb->setChecked(true);
        if (data->surfset) data->surfset->vectors = true;
        if (data->surfsetr) data->surfsetr->vectors = true;
    }
    update();
}

void GlyphEditor::setMinlSource(int ms){
    minlSource = ms;
    QComboBox* mlb = findChild<QComboBox*>("minlengthPickBox");
    mlb->setCurrentIndex(minlSource);
    qDebug() << "minlegth from: " << minlSource;
    data->minlFrom(minlSource);
    update();
}

void GlyphEditor::update(){
    GLWidget* glw = findChild<GLWidget*>("widget");
    glw->updateGL();
    QCheckBox* c = findChild<QCheckBox*>("updateBox");
    if (c->checkState()==Qt::Checked) emit changed();
}

void GlyphEditor::on_thresholdSlider_valueChanged(int value)
{
    setThreshold(value/100.0);
}

void GlyphEditor::on_thresholdBox_valueChanged(double value)
{
    setThreshold(value);
}

void GlyphEditor::on_radiusSlider_valueChanged(int value)
{
    setRadius(value/10.0);
}

void GlyphEditor::on_radiusBox_valueChanged(double value)
{
    setRadius(value);
}

void GlyphEditor::on_normSlider_valueChanged(int value)
{
    setNormalization(value/100.0);
}

void GlyphEditor::on_normBox_valueChanged(double value)
{
    setNormalization(value);
}

void GlyphEditor::on_sizeSlider_valueChanged(int value)
{
    setSize(value/10.0);
}

void GlyphEditor::on_sizeBox_valueChanged(double value)
{
    setSize(value);
}

void GlyphEditor::on_minlengthSlider_valueChanged(int value)
{
    setMinlength(value);
}

void GlyphEditor::on_minlengthBox_valueChanged(double value)
{
    setMinlength(value);
}

void GlyphEditor::on_geometry_toggled(bool checked)
{
   if (checked) setGlyphstyle(0);
}

void GlyphEditor::on_projection_toggled(bool checked)
{
    if (checked) setGlyphstyle(1);
}

void GlyphEditor::on_pies_toggled(bool checked)
{
   if (checked) setGlyphstyle(2);
}

void GlyphEditor::on_points_toggled(bool checked)
{
   if (checked) setPrimitives(0);
   if (!checked) setPrimitives(1);
   update();
}

void GlyphEditor::on_colorBox_activated(int index)
{
    setColor(index);
}

void GlyphEditor::on_displayBox_activated(int index)
{
    setDisplay(index);
}

void GlyphEditor::on_geoBox_activated(int index)
{
    qDebug() << index;
    setGeometry(index);
}

void GlyphEditor::on_saveButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this);
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << "threshold = " << threshold << endl;
    out << "radius = " << radius << endl;
    out << "normalization = " << normalization << endl;
    out << "primsize = " << size << endl;
    out << "display = " << display << endl;
    out << "color = " << color << endl;
    out << "geometry = " << geometry << endl;
    out << "glyphstyle = " << glyphstyle << endl;
    out << "primitives = " << primitives << endl;
    out << "minlength = " << minlength << endl;
    out << "minlSource = " << minlSource << endl;

    file.close();
}

void GlyphEditor::on_loadButton_clicked()
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

void GlyphEditor::on_updateBox_toggled(bool checked)
{
    if (checked) emit update();
}


void GlyphEditor::on_minlengthPickBox_activated(int index)
{
    setMinlSource(index);
}
