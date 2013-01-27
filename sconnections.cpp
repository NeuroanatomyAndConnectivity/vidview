#include "sconnections.h"

#include <QtDebug>

#include <QStringList>

#include <QGLWidget>

#include <QFile>
#include <QTextStream>
#include "qmath.h"

#include <afnisurface.h>
#include <QtCore/QCoreApplication>

QString SConnections::arg(QString argname) {
    int nodespos = qApp->arguments().indexOf(QRegExp("-"+argname+"*"));
    if (nodespos!=-1 && nodespos!=qApp->arguments().length()-1) {
        return qApp->arguments().at(nodespos+1);
    } else {
        return "";
    }
}

SConnections::SConnections()
{
    //TODO: probably should not be empty...
    min = QVector3D(-100,-100,-100);
    max = QVector3D(100,100,100);
    piv = (max-min)/2;
}

SConnections::SConnections(QList<QVector3D> nodes, IndexedConnections* ics, QList<QVector3D>* allNodes, int offset){
    params();
    //qDebug() << "SConnections: " << nodes << ics << nodes.length() << ics->length();
    //TODO: Pointers?
    this->nodes = nodes;
    this->icons = ics;
    calculateBounds();
    qDebug("before create Nodes");
    qDebug() << "icons length: " << icons->length() << allNodes->length();
    createNodes(allNodes, offset);
    qDebug("after create Nodes");
}

SConnections::SConnections(QString nname, QString ename)
{
    params();
    QFile n(nname);

    if (nname.endsWith(".asc")){
        qDebug() << "reading from "+nname;
        AFNISurface afnis(nname,"");
        nodes = afnis.nodes;
    } else {

    if (!n.open(QIODevice::ReadOnly)) qDebug("nodes unreadable");
    QTextStream ns(&n);
    QString nl;

    qDebug() << "reading from " << nname << " and " << ename;

    while(!ns.atEnd()) {
        nl = ns.readLine();

        QStringList vals = nl.split(" ", QString::SkipEmptyParts);
        QVector3D* anode;
        anode = new QVector3D(((QString)(vals.at(0))).toFloat(),
                              ((QString)(vals.at(1))).toFloat(),
                              ((QString)(vals.at(2))).toFloat());
        nodes << *anode;

    }
    n.close();

    }

    qDebug() << nodes.length() << " nodes read";
    calculateBounds();

    dn.clear();
    icons = new IndexedConnections(ename);
    createNodes();
}

void SConnections::createNodes(QList<QVector3D>* allNodes, int offset){

    qDebug() << "allNodes.length: " << allNodes->length();

    qDebug() << "create Nodes in SConnections";

    //initialized with NULL:
    QVector<Node*> *dnv = new QVector<Node*>(allNodes->length(),NULL);

    for (int i = 0; i<icons->length(); i++){
        int f = icons->fs.at(i);
        int t = icons->ts.at(i);
        float v = icons->vs.at(i);

        Connection* acon = new Connection(allNodes->at(f), allNodes->at(t),v);

        Node* n1 = new Node(acon->fn);

        if (dnv->at(f)==NULL) {
            n1->ncs << acon;
            n1->sncs << acon;
            dnv->replace(f,n1);
        } else {
            dnv->at(f)->ncs << acon;
            dnv->at(f)->sncs << acon;
        }

        acon = new Connection(allNodes->at(t), allNodes->at(f),v);

        Node* n2 = new Node(acon->fn);

        if (dnv->at(t)==NULL) {
            n2->ncs << acon;
            n2->sncs << acon;
            dnv->replace(t,n2);
        } else {
            dnv->at(t)->ncs << acon;
            dnv->at(t)->sncs << acon;
        }
    }
    qDebug() << "before putting nodes in dn";
    for (int i = 0; i<nodes.size(); i++){
        if (dnv->at(i+offset)!=NULL){
            dn << *dnv->at(i+offset);
        }
    }
    //qDebug() << "before sorting nodes";
    for (int i = 0; i<dn.length(); i++){
        dn[i].sortNCS();
    }
    delete dnv;
    qDebug() << "done with creating " << dn.length() << " nodes from " << icons->length() << " indexed connections";
}

void SConnections::params() {
    glyphAlpha = 1;
    glyphRadius = 1;
}

void SConnections::calculateBounds() {
    qDebug() << "calculating bounds; nodes.size: " << nodes.size();
    for (int i = 0; i < nodes.size(); i++) {
        QVector3D anode;
        anode = nodes.at(i);
        if (i==0){
            max = QVector3D(anode.x(),anode.y(),anode.z());
            min = QVector3D(anode.x(),anode.y(),anode.z());
        } else {
            max.setX(qMax(anode.x(),max.x()));
            max.setY(qMax(anode.y(),max.y()));
            max.setZ(qMax(anode.z(),max.z()));
            min.setX(qMin(anode.x(),min.x()));
            min.setY(qMin(anode.y(),min.y()));
            min.setZ(qMin(anode.z(),min.z()));
        }
    }
    piv = -(min+max)/2;
    qDebug() << "piv: " << piv.x() << piv.y() << piv.z();
    qDebug() << "max: " << max.x() << max.y() << max.z();
    qDebug() << "min: " << min.x() << min.y() << min.z();
}
