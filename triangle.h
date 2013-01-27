#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "qvector3d.h"
#include "primitive.h"

class Triangle : public Primitive
{
public:
    Triangle(const QVector3D* p1, const QVector3D* p2, const QVector3D* p3, const QVector3D* n1, const QVector3D* n2, const QVector3D* n3);
    Triangle(const QVector3D* p1, const QVector3D* p2, const QVector3D* p3);
    const QVector3D *p1,*p2,*p3;
    const QVector3D *n1,*n2,*n3;
    const QVector3D *c1,*c2,*c3;
    int i1,i2,i3; //indices of the points in the node list
    void setColor(QVector3D *c1,QVector3D *c2,QVector3D *c3);
    void paintGL();
    void setAlpha(double a);
    float v1,v2,v3;
    double alpha;
};

#endif // TRIANGLE_H
