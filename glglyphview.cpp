#include "glglyphview.h"

GLGlyphView::GLGlyphView(QWidget *parent, const QGLWidget * shareWidget, Qt::WindowFlags f)
    :GLWidget::GLWidget(parent,shareWidget,f)
{

}

void GLGlyphView::paintGL(){
    GLWidget::setView();

    QVector3D* c = data->center;
    //glLoadIdentity();
    glScalef(30,30,30);
    glTranslatef(-c->x(),-c->y(),-c->z());

    GLWidget::render(false);
    glColor3f(0,1,1);
    glPointSize(3);
    glBegin(GL_POINTS);
    glVertex3f(c->x(),c->y(),c->z());
    glEnd();
}

void GLGlyphView::updateView(GLfloat* view){
    this->view = view;
    updateGL();
}
