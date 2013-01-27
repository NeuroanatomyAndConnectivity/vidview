#include "afnisurface.h"

#include <qfile.h>
#include <qtextstream.h>

#include <QtDebug>

AFNISurface::AFNISurface(QString filename, QString gradientname, QVector3D shift)
{
    this->filename = filename;
    QFile n(filename);
    if (!n.open(QIODevice::ReadOnly)) qDebug("nodes unreadable");
    QTextStream ns(&n);

    //TODO: something clever with filename
    QString normalsfile = filename.remove("asc");
    normalsfile += "normals.asc";
    qDebug() << "normals expected in file: " << normalsfile;
    QFile no(normalsfile);
    bool normalsExist = true;
    if (!no.open(QIODevice::ReadOnly)) {
        qDebug("normals unreadable");
        normalsExist = false;
    }

    QTextStream nos(&no);

    QString nl;
    ns.readLine();
    nl = ns.readLine();
    int nn = nl.split(" ").at(0).toInt();
    int nt = nl.split(" ").at(1).toInt();

    if (normalsExist){
        nos.readLine();
        nos.readLine();
    }

    //NODES
    qDebug() << "reading " << nn << " nodes...";
    for (int i=0; i<nn; i++) {
        nl = ns.readLine();

        QStringList vals = nl.split(" ", QString::SkipEmptyParts);
        QVector3D* anode;
        //x,y,z
        anode = new QVector3D((((QString)(vals.at(0))).toFloat()),
                              (((QString)(vals.at(1))).toFloat()),
                              ((QString)(vals.at(2))).toFloat());
        *anode += shift;
        //qDebug() << anode->x() << anode->y() << anode->z();
        nodes << *anode;

        if (normalsExist){
            nl = nos.readLine();

            vals = nl.split(" ", QString::SkipEmptyParts);
            QVector3D* normal;
            //x,y,z
            normal = new QVector3D(((QString)(vals.at(0))).toFloat(),
                                   ((QString)(vals.at(1))).toFloat(),
                                   ((QString)(vals.at(2))).toFloat());
            normals << *normal;
        }
    }
    qDebug() << nodes.size() << " nodes read";

    //TRIANGLES
    qDebug() << "reading " << nt << " triangles...";
    for (int i=0; i<nt; i++) {
        nl = ns.readLine();


        float r[3];
        float g[3];
        float b[3];
        QStringList vals = nl.split(" ", QString::SkipEmptyParts);
        Triangle* atri;
        if (!normalsExist) {
            atri = new Triangle(&nodes.at((vals.at(0)).toInt()),
                                          &nodes.at((vals.at(1)).toInt()),
                                          &nodes.at((vals.at(2)).toInt()));
        } else {
            atri = new Triangle(&nodes.at((vals.at(0)).toInt()),
                                          &nodes.at((vals.at(1)).toInt()),
                                          &nodes.at((vals.at(2)).toInt()),
                                          &normals.at((vals.at(0)).toInt()),
                                          &normals.at((vals.at(1)).toInt()),
                                          &normals.at((vals.at(2)).toInt())
                                          );
        }
        atri->i1 = vals.at(0).toInt();
        atri->i2 = vals.at(1).toInt();
        atri->i3 = vals.at(2).toInt();
        tris << atri;
    }

    //GRADIENTS
    qDebug() << "overlay filename: " << gradientname;
    if (!(gradientname=="")) loadOverlay(gradientname);

    n.close();
    qDebug() << tris.size() << " triangles read";
}

void AFNISurface::loadOverlay(QString filename){
    bool is_color = false;
    float* rv = new float[nodes.size()];
    float* gv = new float[nodes.size()];
    float* bv = new float[nodes.size()];
    QFile gr(filename);
    if (!gr.open(QIODevice::ReadOnly)) qDebug("gradients unreadable");
    QTextStream gs(&gr);
    QString nl;

    for (int i = 0; i<nodes.size(); i++){
        nl = gs.readLine();
        QStringList vals = nl.split(" ", QString::SkipEmptyParts);
        //qDebug() << vals.length();
        rv[i] = (vals[0]).toFloat();
        if (vals.length()==3){
            is_color = true;
            gv[i] = (vals[1]).toFloat();
            bv[i] = (vals[2]).toFloat();
        }
    }

    qDebug() << "values read";
    qDebug() << "triangle length: " << tris.length();
    qDebug() << "is color: " << is_color;

    for (int i=0; i<tris.length(); i++) {
        Triangle* atri = tris.at(i);
        float r[3];
        float g[3];
        float b[3];
        r[0] = rv[atri->i1];
        r[1] = rv[atri->i2];
        r[2] = rv[atri->i3];
        if (is_color){
            g[0] = gv[atri->i1];
            g[1] = gv[atri->i2];
            g[2] = gv[atri->i3];

            b[0] = bv[atri->i1];
            b[1] = bv[atri->i2];
            b[2] = bv[atri->i3];
        }

        if (is_color){
            tris.at(i)->setColor(new QVector3D(r[0],g[0],b[0]),new QVector3D(r[1],g[1],b[1]),new QVector3D(r[2],g[2],b[2]));
        } else {
            //black and white
            float a = -0.5;
            float c = 0.5;
            tris.at(i)->setColor(new QVector3D(a*r[0]+c,a*r[0]+c,a*r[0]+c),new QVector3D(a*r[1]+c,a*r[1]+c,a*r[1]+c),new QVector3D(a*r[2]+c,a*r[2]+c,a*r[2]+c));
        }
    }
    qDebug() << "gradient loaded";
}

void AFNISurface::calcNodeNormals(QList<QVector3D> n){

    //given a (possibly shortened from the original surface) list of nodes, create an array with the normals...
    //go through all connection nodes
    //qDebug() << "calcNodeNormals, n.length: " << n.length();
    for (int i=0; i<n.length(); i++){
        //find the normals from the surfaces
        QVector3D nnormal = normals.at(nodes.indexOf(n.at(i)));
        //qDebug() << "calcNodeNormals: " << nnormal;
        nodenormals << nnormal;
    }
}

void AFNISurface::putNormals(SConnections* c){
    for (int i=0; i<c->dn.length(); i++){
        Node* n = &c->dn[i];
        n->normal = normals.at(c->nodes.indexOf(n->p));
    }
}
