#include "connection.h"

#include "qmath.h"

#include <QtDebug>

Connection::Connection(QVector3D fn, QVector3D tn, float v)//TODO: points by reference?
{
    this->fn = fn;
    this->tn = tn;
    this->v = v;

    //this makes the color default to the orientation-codes DTI-stuff...
    QVector3D diff = fn-tn;
    diff.normalize();

    r=qAbs(diff.x());
    g=qAbs(diff.y());
    b=qAbs(diff.z());
}

double Connection::length(){
    return (fn-tn).length();
}
