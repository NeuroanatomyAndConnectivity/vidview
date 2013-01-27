#include "triangle.h"
#include "glwidget.h"

#include <QtDebug>

Triangle::Triangle(const QVector3D* p1, const QVector3D* p2, const QVector3D* p3, const QVector3D* n1, const QVector3D* n2, const QVector3D* n3) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->n1 = n1;
    this->n2 = n2;
    this->n3 = n3;
    this->c1 = new QVector3D(0.5,0.5,0.5);
    this->c2 = new QVector3D(0.5,0.5,0.5);
    this->c3 = new QVector3D(0.5,0.5,0.5);
    alpha = 0.5;
}

void Triangle::setColor(QVector3D *c1,QVector3D *c2,QVector3D *c3){
    this->c1 = c1;
    this->c2 = c2;
    this->c3 = c3;
}

void Triangle::setAlpha(double a){
    this->alpha = a;
    //qDebug() << "alpha: " << this->alpha;
}

Triangle::Triangle(const QVector3D* p1, const QVector3D* p2, const QVector3D* p3){
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    //TODO: calculate normals in surface with neighbor information
    //TODO: check orientation
    QVector3D n = QVector3D::crossProduct(*p1-*p2,*p1-*p3);
    n1 = new QVector3D(n.x(),n.y(),n.z());
    n2 = new QVector3D(n.x(),n.y(),n.z());
    n3 = new QVector3D(n.x(),n.y(),n.z());
    alpha = 0.1;
    this->c1 = new QVector3D(1,1,1);
    this->c2 = new QVector3D(1,1,1);
    this->c3 = new QVector3D(1,1,1);
}

void Triangle::paintGL(){
    //alpha = 0.1;
    //qDebug() << this->alpha;
    //glBegin(GL_TRIANGLES);
    glColor4d(c1->x(),c1->y(),c1->z(),alpha);
    glNormal3d(n1->x(),n1->y(),n1->z());
    glVertex3d(p1->x(),p1->y(),p1->z());

    glColor4d(c2->x(),c2->y(),c2->z(),alpha);
    glNormal3d(n2->x(),n2->y(),n2->z());
    glVertex3d(p2->x(),p2->y(),p2->z());

    glColor4d(c3->x(),c3->y(),c3->z(),alpha);
    glNormal3d(n3->x(),n3->y(),n3->z());
    glVertex3d(p3->x(),p3->y(),p3->z());
    //glEnd();
}
