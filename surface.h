#ifndef SURFACE_H
#define SURFACE_H

#include <QList>
#include "triangle.h"
#include <QString>

class Surface
{
public:
    Surface(QList<Triangle*> tris);
    int nv,nt;
    double *verts;
    double *norms;
    int *inds;
    QList<Triangle*> tris;
    void paintGL();
    void sortTriangles();
    void setAlpha(double a);
};

#endif // SURFACE_H
