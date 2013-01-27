#include "surfacecollection.h"

SurfaceCollection::SurfaceCollection(QString consname,  SurfaceSet* l, SurfaceSet* r, float clipthr)
{
    qDebug() << "loading connectivity for surface collection: " << consname;
    conn = new Connectivity(consname);
    lset = l;
    rset = r;

    if (consname.endsWith(".txt")) {
        icons = new IndexedConnections(consname);
    } else if (consname.endsWith(".bin")) {
        icons = conn->getICs(clipthr);
    }


    for (int i=0; i<lset->afnis.length(); i++){
        qDebug() << "creating l connections for surface # " << i;
        QList<QVector3D>* allNodes = new QList<QVector3D>(lset->afnis.at(i)->nodes);
        if (rset) allNodes->append(rset->afnis.at(i)->nodes);
        lset->createConnections((AFNISurface*)lset->afnis.at(i),icons,allNodes);
        if (rset) rset->createConnections((AFNISurface*)rset->afnis.at(i),icons,allNodes,lset->afnis.at(i)->nodes.length());
    }
}

