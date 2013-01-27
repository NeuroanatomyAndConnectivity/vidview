#ifndef AFNISURFACE_H
#define AFNISURFACE_H

#include "sconnections.h"
#include "qlist.h"
#include "triangle.h"

class AFNISurface
{
public:
    AFNISurface(QString filename, QString gradientname, QVector3D shift=QVector3D(0,0,0));
    QList<int> label1;
    QList<int> label2;
    QList<Triangle*> tris;
    void paintGL();
    QList<QVector3D> nodes;
    QList<QVector3D> normals;
    QList<QVector3D> nodenormals;
    void calcNodeNormals(QList<QVector3D> n);
    void putNormals(SConnections* c);

    QString filename;
    void loadOverlay(QString filename);
};

#endif // AFNISURFACE_H
