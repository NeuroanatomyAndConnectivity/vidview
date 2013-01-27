#ifndef GLGLYPHVIEW_H
#define GLGLYPHVIEW_H

#include "glwidget.h"

class GLGlyphView : public GLWidget
{
public:
    GLGlyphView(QWidget *parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);
protected:
    void paintGL();
public slots:
    void updateView(GLfloat* view);
};

#endif // GLGLYPHVIEW_H
