#ifndef SCONNECTIONS_H
#define SCONNECTIONS_H

#include <QString>
#include <QList>
#include <QVector3D>
#include "primitive.h"

#include <qfile.h>
#include <qtextstream.h>
#include "node.h"
#include "QVector"
#include "indexedconnections.h"

class SConnections
{
public:
    SConnections();
    SConnections(QString nname, QString ename);
    SConnections(QList<QVector3D> nodes, IndexedConnections* ics, QList<QVector3D>* allNodes = NULL, int offset=0);
    void params();

    QVector3D piv;
    QVector3D max, min;
    QList<QVector3D> nodes;
    IndexedConnections* icons;
    void createNodes(QList<QVector3D>* allNodes = NULL, int offset=0);
    //TODO: This should be pointers...
    QList<Node> dn;
    void calculateBounds();
    QList<int> label1;
    QList<int> label2;
    void loadLabels();

    QString arg(QString argname);

    double glyphAlpha, glyphRadius;

};

#endif // SCONNECTIONS_H
