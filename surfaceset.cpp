#include "surfaceset.h"

#include "connection.h"

#include <qfile.h>
#include "qfileinfo.h"
#include <qtextstream.h>

#include "qdir.h"

#include <QtDebug>
#include "qmath.h"

SurfaceSet::SurfaceSet(QString filename, QString consname, QString labelname)
{
    qDebug() << "loading surface set: " << filename << " connections: " << consname << " labelname: " << labelname;

    QFile n(filename);
    if (!n.open(QIODevice::ReadOnly)) qDebug("set file unreadable");
    QTextStream ns(&n);
    QString nl;
    //TODO: Windows will have a problem with this
    QString trunk = QFileInfo(filename).path();
    while(!ns.atEnd()){
        nl = ns.readLine();
        if (!nl.startsWith("#")){
            QStringList sl = nl.split(" ");
            QString fullname = trunk+QDir::separator()+sl.at(0);
            float x = 0;
            float y = 0;
            float z = 0;
            if (sl.length()>1) x = sl.at(1).toFloat();
            if (sl.length()>2) y = sl.at(2).toFloat();
            if (sl.length()>3) z = sl.at(3).toFloat();
            QVector3D s(x,y,z);
            qDebug() << "adding Surface: " << fullname << " shift: " << s;
            addSurface(fullname,labelname, s);
        }
    }

    qDebug() << "surfaces added";

    cs = 0;
    colorsFrom = 0;
    geo = 0;
    norm = 0.5;
    glyphAlpha = 1;
    glyphRadius = 1;
    threshold = 1;
    clear_depth = false;
    vectors = false;
    billboarding = false;
    sproject = false;
    size = 2.0;

    roi = new QSet<int>;

    displayList = NULL;
    updateDisplayList = false;

    selected = new QVector3D(0,0,0);
    selectedIndex = 0;

    qDebug() << "SurfaceSet done...";
}

void SurfaceSet::addSurface(QString filename, QString labelname, QVector3D shift){
    //load surface data
    AFNISurface* a = new AFNISurface(filename, labelname, shift);
    afnis << a;
    //create surfaces for display
    surfs << new Surface(a->tris);
}

void SurfaceSet::createConnections(AFNISurface* a, IndexedConnections* icons, QList<QVector3D>* allNodes, int offset){
    //create colored connections, and transfer normals to nodes for fast display
    SConnections* scs = new SConnections(a->nodes,icons,allNodes, offset);
    a->calcNodeNormals(scs->nodes);
    a->putNormals(scs);
    scons << scs;
    qDebug() << "surface added to set";
}

void SurfaceSet::calcBoundingBox(){
    piv = scons.at(cs)->piv;
    max = scons.at(cs)->max;
    min = scons.at(cs)->min;
}

void SurfaceSet::switchSurface(){

    cs = (cs+1)%surfs.length();
    qDebug() << "surface switched";
}

void SurfaceSet::switchSurface(int i){
    cs = i;
    qDebug() << "surface switched to: " << cs;
}

void SurfaceSet::paintGL(int ns, bool allNodes, bool connect, bool glyphsVisible){
    qDebug() << "Surfaceset.paintGL()";
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST); //TODO: centralize that shit somewhere...
    surfs.at(cs)->paintGL();
    if (clear_depth) {
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    if (connect) paintConnectivity();
    paintROI();
    glDisable(GL_LIGHTING);
    if (glyphsVisible) paintBufferedNodes(ns);
    glEnable(GL_LIGHTING);
}

void SurfaceSet::paintBufferedNodes(int ns){
    /*if (displayList == NULL) displayList = glGenLists(1);
    if (updateDisplayList) {
        glNewList(displayList,GL_COMPILE_AND_EXECUTE);
        updateDisplayList = false;
        paintNodes(ns);
        glEndList();
    } else {
        glCallList(displayList);
    }*/
    paintNodes(ns);
}

void SurfaceSet::paintConnectivity(){
    glColor3f(1,0,0);
    qDebug() << "selectedIndex: " << selectedIndex;
    if (conn){
        qDebug() << afnis.at(0)->nodes.length();
        for (int i = 0; i < afnis.at(0)->nodes.length(); i++){
            float r = 20*conn->conn[i][selectedIndex];
            if (r>0) {
                glPointSize(r);
                //qDebug() << i << r;
                glBegin(GL_POINTS);
                QVector3D p = afnis.at(cs)->nodes.at(i);
                glVertex3f(p.x(),p.y(),p.z());
                glEnd();
            }
        }
    } else {
        qDebug() << "no connectivity information available";
    }
}

void SurfaceSet::paintROI(){
    glColor3f(0,1,0);
    glPointSize(10);
    glBegin(GL_POINTS);
    for (int i = 0; i < afnis.at(0)->nodes.length(); i++){
            QVector3D p = afnis.at(cs)->nodes.at(i);
            if (roi->contains(i)) glVertex3f(p.x(),p.y(),p.z());
    }
    glEnd();
}

void SurfaceSet::paintNodes(int ns){

    calcInvRot();

    SConnections* ccs = scons.at(cs);

    glPointSize(qMax(size,0.1));  //does not like 0 for pointsize...
    glLineWidth(size);

    for (int i = 0; i < ccs->dn.length(); i++){
        Node* p = (Node*)(&ccs->dn.at(i));
        QVector3D nnormal = p->normal.normalized();
        QMatrix4x4* view = viewMatrix();
        QVector3D mapped = view->mapVector(nnormal);
        QVector3D mappedp = view->map(p->p);

        bool visible = mapped.z() > 0;
        //visible = true;
        //additionally: compare depth buffer after surface draw?
        //TODO: poor guys clipping, should take ar into account...
        double clip = 1;
        visible &= (mappedp.x()>-clip);
        visible &= (mappedp.x()<clip);
        visible &= (mappedp.y()>-clip);
        visible &= (mappedp.y()<clip);

        if (visible) {

        int cOver = p->connectionsOver(threshold); //How many connections have a value above the threshold?
        int nth = 0; //the how-manieth connection for the pie chart...

        QVector3D zshift = glyphRadius*invRotZ;
        if (ns==4) {
            //pie charts
            glShadeModel(GL_FLAT);
            glBegin(GL_TRIANGLE_FAN);
            glVertex3f(p->p.x()+zshift.x(),p->p.y()+zshift.y(),p->p.z()+zshift.z());
        }

        QVector3D pieClosePoint;
        float cr,cg,cb;

        for (int j=0; j<p->ncs.length();j++){
            Connection* e = p->ncs.at(j);
            QVector3D diff; //scaled vector from current point to point on the other side:
            //in the createPrims method, edges are connected to the nodes with fn == n.p:
            Connection* diffc = ((Node*)(&scons.at(geo)->dn.at(i)))->ncs.at(j);
            diff = diffc->tn-diffc->fn;
            diff = diff*glyphRadius/100.0;

            bool draw = true;

            Node* colorNode = (Node*)(&scons.at(colorsFrom)->dn.at(i));
            Connection* c = colorNode->ncs.at(j);
            if (c->v < threshold) draw=false;
            glColor4f(c->r,c->g,c->b,glyphAlpha);

            //if (nor.length()>50){

                QVector3D p_shifted;

                p_shifted = p->p + diff;


                if (billboarding && (ns==6)) {

                    //TODO: check, this used to be es...
                    p_shifted = diffc->tn;

                    QVector2D xy(p_shifted.x(),p_shifted.y());
                    xy /= 100;
                    xy.normalize();
                    double l = p_shifted.z()/2.0+0.5;
                    xy *= l*glyphRadius/100;
                    //l = qPow(l,2);
                    p_shifted = p->p;
                    p_shifted += xy.x()*invRotX;
                    p_shifted += xy.y()*invRotY;
                }

                Connection* pieEdge;
                float rad;
                if (ns==4) {
                    //pie charts

                    //TODO: redundant:
                    pieEdge = colorNode->sncs.at(j);

                    if (pieEdge->v > threshold){

                        glColor4f(pieEdge->r,pieEdge->g,pieEdge->b,glyphAlpha);
                        //float t = (j/(float)p->ncs.length())*2*M_PI;
                        float t = (nth/(float)cOver)*2*M_PI;
                        nth++;

                       /* if (norm==1){
                            //rad = 0.1*glyphRadius*qSqrt(p->ncs.length());
                            rad = 0.1*glyphRadius*qSqrt(cOver);
                        } else if (norm==2) {
                            rad = glyphRadius;
                        } else if (norm==3) {
                            //rad = glyphRadius*(20+qSqrt(p->ncs.length()))/40.0;
                            rad = glyphRadius*(20+qSqrt(cOver))/40.0;
                        }*/

                        rad = norm*glyphRadius + (1-norm)*glyphRadius*qSqrt(cOver)/10.0;

                        p_shifted = p->p;
                        p_shifted += rad*qSin(t)*invRotX;
                        p_shifted += rad*qCos(t)*invRotY;
                    }
                }

                if (sproject){
                    double dprod = QVector3D::dotProduct(diff,nnormal);
                        p_shifted = p_shifted - (nnormal*dprod);
                }

                //TODO: There is no more reason that this stuff couldn't go further up? Also: The flow here is seriously off...
                if (!vectors){
                    glBegin(GL_POINTS);
                } else if (ns!=4){
                    glBegin(GL_LINES);
                    if (draw) glVertex3d(p->p.x()+zshift.x(),p->p.y()+zshift.y(),p->p.z()+zshift.z());
                }
                if (ns==4) {
                    draw = (pieEdge->v > threshold);
                    if ((nth==1)&draw) {
                        cr = pieEdge->r;
                        cg = pieEdge->g;
                        cb = pieEdge->b;
                        pieClosePoint = QVector3D(p_shifted.x()+zshift.x(),p_shifted.y()+zshift.y(),p_shifted.z()+zshift.z());
                    }
                }
                if (draw) glVertex3d(p_shifted.x()+zshift.x(),p_shifted.y()+zshift.y(),p_shifted.z()+zshift.z());
                if (ns!=4) glEnd();
            //}
        }
        //TODO: deal with two/one point issue...
        if (ns==4) {
            glColor4f(cr,cg,cb,glyphAlpha);
            glVertex3f(pieClosePoint.x(),pieClosePoint.y(),pieClosePoint.z());
            glEnd();
        }
        }
    }
}

void SurfaceSet::setGlyphAlpha(double a){
    glyphAlpha=a;
    for (int i=0; i<scons.length(); i++){
        scons.at(i)->glyphAlpha=a;
    }
}

void SurfaceSet::setGlyphRadius(double r){
    glyphRadius=r;
    for (int i=0; i<scons.length(); i++){
        scons.at(i)->glyphRadius=r;
    }
}

QMatrix4x4* SurfaceSet::viewMatrix(){
    float mat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    QMatrix4x4* viewMat = new QMatrix4x4(mat[0],mat[4],mat[8],mat[12],mat[1],mat[5],mat[9],mat[13],mat[2],mat[6],mat[10],mat[14],mat[3],mat[7],mat[11],mat[15]);
    return viewMat;

}

void SurfaceSet::calcInvRot(){
    float mat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    //This one is tricky: the matrix below is the GL matrix (different row/column convention than QT) with the 3x3 part transposed.
    //This inverts the rotation and does weird stuff to the scale...
    QMatrix4x4* invRotMat = new QMatrix4x4(mat[0],mat[1],mat[2],mat[12],mat[4],mat[5],mat[6],mat[13],mat[8],mat[9],mat[10],mat[14],mat[3],mat[7],mat[11],mat[15]);
    //QMatrix4x4* invRotMat = new QMatrix4x4(mat[0],mat[1],mat[2],mat[3],mat[4],mat[5],mat[6],mat[7],mat[8],mat[9],mat[10],mat[11],mat[12],mat[13],mat[14],mat[15]);
    const QVector3D xVec(1,0,0);
    const QVector3D yVec(0,1,0);
    const QVector3D zVec(0,0,1);
    //mapVector ignores translation and such...
    invRotX = invRotMat->mapVector(xVec);
    invRotY = invRotMat->mapVector(yVec);
    invRotZ = invRotMat->mapVector(zVec);
    //normalization ignores the scale
    invRotX.normalize();
    invRotY.normalize();
    invRotZ.normalize();
}

void SurfaceSet::select(QVector3D v){
    selected = NULL;
    double mindist=10000000;
    qDebug() << "select in surfset: " << v;
    QList<QVector3D> *n = &(scons.at(cs)->nodes);
    for (int i = 0; i<n->length(); i++){
        double d = (n->at(i)-v).length();
        if (i==0) mindist = d;
        if (d<mindist) {
            mindist = d;
            qDebug() << mindist;
            selected = new QVector3D(n->at(i));
            selectedIndex = i;
        }
    }
}

void SurfaceSet::loadOverlay(QString filename){
    for (int i = 0; i<afnis.length(); i++){
        AFNISurface* afni = afnis.at(i);
        afni->loadOverlay(filename);
    }
}

void SurfaceSet::saveROI(QString filename){
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    foreach(const int &i, *roi){
        out << i << endl;
    }
    file.close();
}

void SurfaceSet::loadROI(QString filename){
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    roi->clear();
    while (!in.atEnd()){
        QString nl = in.readLine();
        if (!(nl.startsWith("#")) && !(nl=="")){
            qDebug() << nl;
            QStringList vals = nl.split(" ", QString::SkipEmptyParts);
            roi->insert(vals[0].toInt());
        }
    }
    file.close();
}
