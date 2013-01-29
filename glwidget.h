#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "afnisurface.h"
#include "surface.h"
#include <QGLFramebufferObject>

#include "surfaceset.h"

#include "data.h"

#include "qprocess.h"

#include "displayparameters.h"

//#include "sumainterface.cpp"

#include "qmutex.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);
    ~GLWidget();

    void loadData(QString ssl, QString ssr, QString scons, QString overlayl, QString overlayr, float clipthr);
    void setData(GLWidget* glw);
    void setData(Data* data);
    void setDisplayParameters(DisplayParameters* dp);

    Data* data;

    GLfloat* view;

    void saveView(QString filename);
    void loadView(QString filename);
   //SUMAInterface* si;
    void orient(QString ori);

    float bg;

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent (QWheelEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    void setView();
    void render(bool shift, bool allNodes=false);

    QMatrix4x4* modelMatrix();
    QMatrix4x4* projectionMatrix();
    QVector3D world; //world coordinates of selected point
    QProcess* process; //System process for calling DriveSuma after selecting a point
private:
    QPoint lastPos;
    float ar;
    float brainAlpha, stuffAlpha, glyphAlpha, glyphRadius;

    DisplayParameters* dp;

    double scale;
    bool select(QMouseEvent *event);
    bool screen, cursor;

    mutable QMutex mutex;

    QString o; //for the command line orientation (iaplr, with s being the default)

public slots:
    void screenshot(QString name, double resolution=1);
    QString arg(QString argname);
    void updateParameters();
    void updateNode(int i);

signals:
    void dataChanged();
    void selected();
};

#endif
