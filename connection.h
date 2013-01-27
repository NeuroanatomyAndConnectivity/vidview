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

    double length();

};

#endif // CONNECTION_H
