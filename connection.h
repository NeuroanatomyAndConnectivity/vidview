#ifndef CONNECTION_H
#define CONNECTION_H

#include <QList>
#include <QVector3D>

#include <QGLWidget>

class Connection
{
public:
    Connection(QVector3D fn, QVector3D tn, float v);
    QVector3D fn, tn;
    float v;

    float r,g,b;

    int origInd; //original Index (for access to same connection in different space)

    double length();

};

#endif // CONNECTION_H
