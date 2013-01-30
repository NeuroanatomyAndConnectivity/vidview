#include "node.h"

#include "glwidget.h"

#include <QtDebug>

#include "qmath.h"

#include <QtCore/QCoreApplication>

Node::Node(QVector3D p)
{
    this->p = p;
}

QString Node::arg(QString argname) {
    int nodespos = qApp->arguments().indexOf(QRegExp("-"+argname+"*"));
    if (nodespos!=-1 && nodespos!=qApp->arguments().length()-1) {
        return qApp->arguments().at(nodespos+1);
    } else {
        return "";
    }
}

bool edgeCompare(Connection* e1, Connection* e2){
    QColor c1(e1->r*255,e1->g*255,e1->b*255);
    QColor c2(e2->r*255,e2->g*255,e2->b*255);
    //qDebug() << e1->r << " " << c1.red() << " " << c2.red();
    return c1.hueF() > c2.hueF();
    //return c1.valueF() > c2.valueF();
}

void Node::indexCons(){
    for (int i = 0; i<ncs.length(); i++){
        ncs.at(i)->origInd = i;
        sncs.at(i)->origInd = i;
    }
}

void Node::sortNCS(){
    qSort( sncs.begin(), sncs.end(), edgeCompare );
}


bool Node::operator ==(Node const& n){
    return (n.p == p);
}

void Node::buildRot(float m[16]) {
    float temp;

    temp = m[1]; m[1] = m[4]; m[4] = temp;
    temp = m[2]; m[2] = m[8]; m[8] = temp;
    temp = m[6]; m[6] = m[9]; m[9] = temp;

}

void Node::calcMatrix() {
    float mat[16];

    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    buildRot(mat);
    glMultMatrixf(mat);
}

