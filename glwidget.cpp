#include <QtGui>
#include <QtOpenGL>

#include <QtDebug>

#include "glwidget.h"
#include <QMatrix4x4>

#include "afnisurface.h"

QString GLWidget::arg(QString argname) {
    int nodespos = qApp->arguments().indexOf(QRegExp("-"+argname+"*"));
    if (nodespos!=-1 && nodespos!=qApp->arguments().length()-1) {
        return qApp->arguments().at(nodespos+1);
    } else {
        return "";
    }
}

GLWidget::GLWidget(QWidget *parent, const QGLWidget * shareWidget, Qt::WindowFlags f)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent, shareWidget, f)
{
    data = NULL;

    brainAlpha = 0.3;
    stuffAlpha = 0.1;

    screen = false;

    o = arg("orientation");

    view = new GLfloat[16];

    //background gray value
    bg = 1;

    process = new QProcess();

    qDebug("widget created");
}

void GLWidget::loadData(QString ssl, QString ssr, QString scons, QString overlayl, QString overlayr, float clipthr){
    data = new Data(ssl, ssr, scons, overlayl, overlayr, clipthr);

    QVector3D size(1,1,1);
    size = data->max() - data->min();

    qDebug() << "size: " << size.x() << size.y();
    float largest = qMax(size.x(),size.y());
    scale = (1/largest)*0.95;

    qDebug() << "widget data loaded";
}

void GLWidget::setData(GLWidget* glw){
    setData(glw->data);
}

void GLWidget::setData(Data* data){
    this->data = data;
    QVector3D size(1,1,1);
    size = data->max() - data->min();

    qDebug() << "size: " << size.x() << size.y();
    float largest = qMax(size.x(),size.y());
    scale = (1/largest)*0.95;
}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
    qDebug() << "initialize GL start";

    GLfloat fSizes[2];
    glGetFloatv(GL_POINT_SIZE_RANGE,fSizes);
    GLfloat step;
    glGetFloatv(GL_POINT_SIZE_GRANULARITY,&step);
    qDebug() << "Pointsize range: " << fSizes[0] << fSizes[1];
    qDebug() << "Pointsize granularity: " << step;

    glEnable(GL_RESCALE_NORMAL);

    //glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);
    glDisable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

    //glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    static GLfloat global_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    static GLfloat specular[] = {0.5f, 0.5f, 0.5f , 1.0f};
    glMateriali(GL_FRONT, GL_SHININESS, 1);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    static GLfloat lightPosition[4] = { 10000, 10000, 50000, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    if (!screen) glGetFloatv(GL_MODELVIEW_MATRIX, view);

    qDebug() << "initialize GL done";
}

void GLWidget::setView(){
    glLoadIdentity();
    glScaled(scale,scale,scale);

    glMultMatrixf(view);

    //This is only for initialization...
    if (o=="i"){
        glRotatef(180,0,1,0);
    }

    if (o=="a"){
        glRotatef(90,0,1,0);
        glRotatef(-90,1,0,0);
        glRotatef(90,0,0,1);
    }

    if (o=="p"){
        glRotatef(90,0,1,0);
        glRotatef(-90,1,0,0);
        glRotatef(-90,0,0,1);
    }

    if (o=="l"){
        glRotatef(90,0,1,0);
        glRotatef(-90,1,0,0);
    }

    if (o=="r"){
        glRotatef(-90,0,1,0);
        glRotatef(-90,1,0,0);
    }
}

void GLWidget::paintGL()
{
    qDebug() << "begin paintGL in GLWidget";

    setView();
    render(true);
}

void GLWidget::render(bool shift, bool allNodes){

    glColorMask(1,1,1,1);
    glClearColor(bg,bg,bg,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    data->paintGL(dp,shift,allNodes);

    //TODO: This seems necessary to get keyboard events to this widget:
    //if (!picking) setFocus();

    QVector3D* s = data->selPos();

    qDebug() << "selPos:" << s;

    if (cursor&&(s!=NULL)){
        double l = 10;
        glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(s->x()+l,s->y(),s->z());
        glVertex3f(s->x()-l,s->y(),s->z());
        glColor3f(0,1,0);
        glVertex3f(s->x(),s->y()+l,s->z());
        glVertex3f(s->x(),s->y()-l,s->z());
        glColor3f(0,0,1);
        glVertex3f(s->x(),s->y(),s->z()+l);
        glVertex3f(s->x(),s->y(),s->z()-l);
        glEnd();

        glDisable(GL_DEPTH_TEST);
        glPointSize(5);
        glColor3f(1,0,0);
        glBegin(GL_POINTS);
        glVertex3f(s->x(),s->y(),s->z());
        glEnd();
    }
}

void GLWidget::resizeGL(int width, int height)
{
    qDebug() << "begin GL resize, width: " << width << " height: " << height;

    glViewport(0, 0, width, height);
    ar = (float)width/(float)height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (ar>1){
        glOrtho(ar*-0.5, ar*+0.5, -0.5, +0.5, -1000, 1000);
    } else {
        glOrtho(-0.5, +0.5, -0.5/ar, +0.5/ar, -1000, 1000);
    }
    //glOrtho(-0.5,+0.5,-0.5,+0.5,-0.5,0.5);
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();

    select(event);

    //updateGL();
}

bool GLWidget::select(QMouseEvent *event){
    if (event->modifiers() & Qt::ControlModifier){
        qDebug() << "selecting...";

        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        GLfloat z;
        GLint x = event->x();
        GLint y = viewport[3] - event->y();
        glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, (void*)&z);
        //qDebug() << "glReadPixels x: " << x << " y: " << y << " z: " << z;
        float xs = 2*x/(float)viewport[2]-1;
        float ys = 2*y/(float)viewport[3]-1;
        float zs = 2*z-1;
        QVector3D sp(xs,ys,zs);
        //qDebug() << sp;
        QMatrix4x4 inv = modelMatrix()->inverted();
        QMatrix4x4 inv2 = projectionMatrix()->inverted();
        QVector3D repro = inv2.map(sp);
        //qDebug() << sp;
        world = inv.map(repro);
        data->select(world, event->modifiers()&Qt::AltModifier);

        int s = data->selected->selectedIndex;

        emit selected();
        return true;
    } else {
        return false;
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!select(event)){

        glLoadIdentity();

        int dx = event->x() - lastPos.x();
        int dy = event->y() - lastPos.y();

        if (event->buttons() & Qt::LeftButton) {
            QMatrix4x4 mat(view[0],view[4],view[8],view[12],view[1],view[5],view[9],view[13],view[2],view[6],view[10],view[14],view[3],view[7],view[11],view[15]);
            QVector3D orig(0, 0, 0);
            QVector3D m = mat.map(orig);
            glTranslatef(m.x(), m.y(), m.z());
            glRotatef(qSqrt(dx*dx+dy*dy)/2.0, dy, dx, 0);
            glTranslatef(-m.x(), -m.y(), -m.z());
        } else if (event->buttons() & Qt::RightButton) {
            glTranslatef(dx/(float)width()*ar/scale, -dy/(float)height()/scale, 0);
        }
        lastPos = event->pos();
        glPushMatrix();
        glMultMatrixf(view);
        glGetFloatv(GL_MODELVIEW_MATRIX, view);
        glPopMatrix();
    }

    updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event){
    //TODO: sorting of all stuff in one sweep?
    //surfr->sortTriangles();
    //surfl->sortTriangles();
    //cons->sortCons();
    updateGL();
}

void GLWidget::wheelEvent (QWheelEvent *event)
{
    int d = event->delta();
    //qDebug() << "scale" << scale;
    scale *= 1.0-d/1200.0;
    updateGL();
}

void GLWidget::screenshot(QString name, double resolution){
    screen = true;

    //TODO: check: supp. does not work on windows / blocks on laptop / does not work non-interactively...
    QPixmap map = this->renderPixmap(this->width()*resolution,this->height()*resolution,true);

    map.save(name);
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    qDebug() << "key:" << event->key();
    if (event->key() == Qt::Key_C) {
        if (bg == 1) {
            bg = 0;
        } else {
            bg =1;
        }
    }

    if (event->key() == Qt::Key_1) data->nodestyle = 1;
    if (event->key() == Qt::Key_2) data->nodestyle = 2;
    if (event->key() == Qt::Key_3) data->nodestyle = 3;
    if (event->key() == Qt::Key_4) data->nodestyle = 4;
    if (event->key() == Qt::Key_5) data->nodestyle = 5;
    if (event->key() == Qt::Key_6) data->nodestyle = 6;
    qDebug() << data->nodestyle;
    if (event->key() == Qt::Key_T) screenshot("test.png");
    if (event->key() == Qt::Key_F) {
        if (this->window()->isFullScreen()) {
            this->window()->showNormal();
        } else {
            this->window()->showFullScreen();
        }
    }
    if (event->key() == Qt::Key_I) {
        glLoadIdentity();
        glRotatef(180,0,1,0);
        glGetFloatv(GL_MODELVIEW_MATRIX, view);
        o = "";
    }

    if (event->key() == Qt::Key_A) {
        glLoadIdentity();
        glRotatef(90,0,1,0);
        glRotatef(-90,1,0,0);
        glRotatef(90,0,0,1);
        glGetFloatv(GL_MODELVIEW_MATRIX, view);
        o = "";
    }

    if (event->key() == Qt::Key_P) {
        glLoadIdentity();
        glRotatef(90,0,1,0);
        glRotatef(-90,1,0,0);
        glRotatef(-90,0,0,1);
        glGetFloatv(GL_MODELVIEW_MATRIX, view);
        o = "";
    }

    if (event->key() == Qt::Key_L) {
        glLoadIdentity();
        glRotatef(90,0,1,0);
        glRotatef(-90,1,0,0);
        glGetFloatv(GL_MODELVIEW_MATRIX, view);
        o = "";
    }

    if (event->key() == Qt::Key_R) {
        glLoadIdentity();
        glRotatef(-90,0,1,0);
        glRotatef(-90,1,0,0);
        glGetFloatv(GL_MODELVIEW_MATRIX, view);
        o = "";
    }

    if (event->key() == Qt::Key_S) {
        glLoadIdentity();
        glGetFloatv(GL_MODELVIEW_MATRIX, view);
        o = "";
    }


    if (event->key() == Qt::Key_B){
        if (data->surfset) {
            qDebug() << "switching surface";
            data->surfset->switchSurface(2);
            data->surfl = data->surfset->surfs.at(data->surfset->cs);
        }
        if (data->surfsetr) {
            qDebug() << "switching surface r";
            data->surfsetr->switchSurface(2);
            data->surfr = data->surfsetr->surfs.at(data->surfsetr->cs);
        }

    }
    if (event->key() == Qt::Key_G){
        if (data->surfset) {
            data->surfset->clear_depth = !data->surfset->clear_depth;
        }
    }
    if (event->key() == Qt::Key_H){
        if (data->surfset) {
            data->surfset->vectors = !data->surfset->vectors;
            qDebug() << "vectors: " << data->surfset->vectors;
        }
    }
    if (event->key() == Qt::Key_J){
        if (data->surfset!=NULL) {
            data->surfset->billboarding = !data->surfset->billboarding;
            qDebug() << "billboarding: " << data->surfset->billboarding;
        }
    }
    if (event->key() == Qt::Key_K){
        if (data->surfset) {
            data->surfset->sproject = !data->surfset->sproject;
            qDebug() << "sproject: " << data->surfset->sproject;
        }
    }
    if (event->key() == Qt::Key_X){
        if (data->surfset) {
            data->surfset->colorsFrom = (data->surfset->colorsFrom+1)%data->surfset->afnis.length();
        }
    }
    if (event->key() == Qt::Key_Y){
        if (data->surfset) {
            data->surfset->norm++;
            if (data->surfset->norm==4) {
                data->surfset->norm = 1;
            }
        }
    }
    if (event->key() == Qt::Key_V){
        if (data->surfset) {
            data->surfset->geo = (data->surfset->geo+1)%data->surfset->afnis.length();
            qDebug() << data->surfset->geo;
        }
    }
    if (event->key() == Qt::Key_Z){
        if (data->surfset) {
            data->surfset->updateDisplayList = true;
        }
    }
    emit dataChanged();
    updateGL();
}

QMatrix4x4* GLWidget::modelMatrix(){
    float mat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    return new QMatrix4x4(mat[0],mat[4],mat[8],mat[12],mat[1],mat[5],mat[9],mat[13],mat[2],mat[6],mat[10],mat[14],mat[3],mat[7],mat[11],mat[15]);
}

QMatrix4x4* GLWidget::projectionMatrix(){
    float mat[16];
    for (int i = 0; i<16; i++){
        //qDebug() << mat[i];
    }
    glGetFloatv(GL_PROJECTION_MATRIX, mat);
    return new QMatrix4x4(mat[0],mat[4],mat[8],mat[12],mat[1],mat[5],mat[9],mat[13],mat[2],mat[6],mat[10],mat[14],mat[3],mat[7],mat[11],mat[15]);
}

void GLWidget::setDisplayParameters(DisplayParameters* dp){
    this->dp = dp;
    connect(dp,SIGNAL(changed()),this,SLOT(updateParameters()));
    updateParameters();
}

void GLWidget::updateParameters(){
    if (data->surfl) data->surfl->setAlpha(dp->left);
    if (data->surfr) data->surfr->setAlpha(dp->right);
    data->setGlyphAlpha(dp->glyphs);
    qDebug() << "parameters: " << dp->left << dp->right << dp->glyphs;
    QCheckBox* b = dp->findChild<QCheckBox*>("cursorBox");
    cursor = b->isChecked();
    b = dp->findChild<QCheckBox*>("leftBox_2");
    data->lon = b->isChecked();
    qDebug() << "lon: " << data->lon;
    b = dp->findChild<QCheckBox*>("rightBox_2");
    data->ron = b->isChecked();
    qDebug() << "ron: " << data->ron;
    b = dp->findChild<QCheckBox*>("connectivityBox");
    data->connectivity = b->isChecked();
    b = dp->findChild<QCheckBox*>("glyphVisibilityBox");
    data->glyphsVisible = b->isChecked();
    emit dataChanged();
    updateGL();
}

void GLWidget::updateNode(int i){
    QMutexLocker locker(&mutex);
    qDebug() << "updateNode: " << i;
}

void GLWidget::saveView(QString filename){
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    for (int i = 0; i<16; i++){
        out << view[i] << endl;
    }
    out << scale;
    file.close();
}

void GLWidget::loadView(QString filename){
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    for (int i = 0; i<16; i++){
        in >> view[i];
    }
    in >> scale;
    file.close();
}

void GLWidget::orient(QString ori){
    if (ori=="i") {
        glLoadIdentity();
        glRotatef(180,0,1,0);
        glGetFloatv(GL_MODELVIEW_MATRIX, view);
        o = "";
    }

    if (ori=="a") {
        glLoadIdentity();
        glRotatef(90,0,1,0);
        glRotatef(-90,1,0,0);
        glRotatef(90,0,0,1);
        glGetFloatv(GL_MODELVIEW_MATRIX, view);
        o = "";
    }

    if (ori=="p") {
        glLoadIdentity();
        glRotatef(90,0,1,0);
        glRotatef(-90,1,0,0);
        glRotatef(-90,0,0,1);
        glGetFloatv(GL_MODELVIEW_MATRIX, view);
        o = "";
    }

    if (ori=="l") {
        glLoadIdentity();
        glRotatef(90,0,1,0);
        glRotatef(-90,1,0,0);
        glGetFloatv(GL_MODELVIEW_MATRIX, view);
        o = "";
    }

    if (ori=="r") {
        glLoadIdentity();
        glRotatef(-90,0,1,0);
        glRotatef(-90,1,0,0);
        glGetFloatv(GL_MODELVIEW_MATRIX, view);
        o = "";
    }

    if (ori=="s") {
        glLoadIdentity();
        glGetFloatv(GL_MODELVIEW_MATRIX, view);
        o = "";
    }

}
