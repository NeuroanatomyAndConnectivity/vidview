#include "surface.h"
#include <qfile.h>
#include <qtextstream.h>
#include <QtDebug>
#include <QStringList>

#include <QGLWidget>

Surface::Surface(QList<Triangle*> tris){
    this->tris = tris;
    qDebug() << "!!!!!!!!!!!!!!!Surface from Triangle List with: " << tris.length() << " triangles";
}

void Surface::paintGL()
{
    sortTriangles();

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < tris.length(); i++) {
        ((Triangle*)tris.at(i))->paintGL();
    }
    glEnd();
}

bool tapComp(Triangle* tp1, Triangle* tp2){
    return (*tp1) < (*tp2);
}

void Surface::sortTriangles() {
    //Matrix vom Viewport
    GLfloat mat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX,mat);
    for (int i = 0; i<16;i++){
        //qDebug() << mat[i];
    }
    //durch die dreiecke, tiefe berechnen
    for (int i = 0; i<tris.length(); i++) {
        Triangle* t = tris.at(i);
        double z1 = t->p1->x()*mat[2]+t->p1->y()*mat[6]+t->p1->z()*mat[10]+mat[14];
        double z2 = t->p2->x()*mat[2]+t->p2->y()*mat[6]+t->p2->z()*mat[10]+mat[14];
        double z3 = t->p3->x()*mat[2]+t->p3->y()*mat[6]+t->p3->z()*mat[10]+mat[14];
        t->depth = (z1+z2+z3)/3.0;
        //qDebug() << t->depth;
    }
    qSort(tris.begin(),tris.end(),tapComp);
}

void Surface::setAlpha(double a){
    qDebug() << "Surface alpha: " << a << " tris.length()" << tris.length();
    for (int i = 0; i < tris.length(); i++) {
        //qDebug() << "setAlpha: " << a << i;
        tris.at(i)->setAlpha(a);
    }
    qDebug() << "setAlpha done";
}
