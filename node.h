#ifndef NODE_H
#define NODE_H

#include <primitive.h>
#include <connection.h>

class Node
{
public:
    Node(QVector3D p);
    QVector3D p;

    //List, because a node can be connected to the same cluster multiple times...
    QList<int> clustergroups;

    //List of edges connected to this node for references between diff. surfaces: Original ordering
    QList<Connection*> ncs;
    //sorted Edges
    QList<Connection*> sncs;

    QVector3D normal;

    void indexCons();
    void sortNCS();

    bool operator ==(Node const& n);
    void buildRot(float m[16]);
    void calcMatrix();
    QString arg(QString argname);
};

#endif // NODE_H
