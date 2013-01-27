#include "data.h"

#include <QtGui>

QString Data::arg(QString argname) {
    int nodespos = qApp->arguments().indexOf(QRegExp("-"+argname+"*"));
    if (nodespos!=-1 && nodespos!=qApp->arguments().length()-1) {
        return qApp->arguments().at(nodespos+1);
    } else {
        return "";
    }
}

Data::Data(QString ssl, QString ssr, QString scons, QString gradientnamel, QString gradientnamer, float clipthr)
{
    surfl = NULL;
    surfr = NULL;
    surfset = NULL;
    surfsetr = NULL;

    center = new QVector3D(0,0,0);
    selected = NULL;

    firstSort = true;

    if (ssl!=""){
        qDebug() << "loading SurfaceSet: " << ssl << scons;
        surfset = new SurfaceSet(ssl,scons, gradientnamel);
        qDebug() << "SurfaceSet l loaded in data";
        surfl = surfset->surfs.at(surfset->cs);
        qDebug() << "surface set loaded";
    }

    if (ssr!=""){
        qDebug() << "loading SurfaceSet r: " << ssr << arg("sconsr");
        //TODO: Sort out common / separate connectivity...
        //make gradientname conditional
        surfsetr = new SurfaceSet(ssr,arg("scons"), gradientnamer);
        qDebug() << "SurfaceSet r loaded in data";
        surfr = surfsetr->surfs.at(surfsetr->cs);
        qDebug() << "surface set r loaded";
    }

   if (surfset&&surfsetr) {
       //assume common connectivity
       coll = new SurfaceCollection(scons, surfset, surfsetr, clipthr);
   } else {
       if (surfset){
           qDebug() << "only left surface";
          // surfset->createConnections(surfset->afnis.at(surfset->cs),new IndexedConnections(arg("scons")));
           coll = new SurfaceCollection(scons,surfset,NULL,clipthr);
           qDebug() << "done";
       }
   }
   if (surfset) {
       surfset->calcBoundingBox();
       surfset->conn = coll->conn;
       surfset->offset = 0;
   }
   if (surfsetr) {
       surfsetr->calcBoundingBox();
       surfsetr->conn = coll->conn;
       surfsetr->offset = surfset->afnis.at(0)->nodes.length();
   }
   qDebug() << "done with data initialization";
}


QVector3D Data::max(){
    if (surfset) return surfset->max;
    return QVector3D(100,100,100);
}

QVector3D Data::min(){
    if (surfset) return surfset->min;
    return QVector3D(-100,-100,-100);
}

//TODO: Remove stuffalpha...
void Data::paintGL(DisplayParameters* dp, bool shift, bool allNodes){

    qDebug() << "paintGL in data";

    //if (sl) qDebug() << "in data: " << sl->selected;

    if (surfset&&shift) glTranslatef(surfset->piv.x(),surfset->piv.y(),surfset->piv.z());

    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);

    //TODO: decide what works best...
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    if (surfset&&lon) surfset->paintGL(nodestyle, allNodes, connectivity);
    if (surfsetr&&ron) surfsetr->paintGL(nodestyle, allNodes, connectivity);

}

void Data::select(QVector3D v){
    float dist;
    if (surfset) {
        surfset->select(v);
        center = surfset->selected;
        selected = surfset;
        dist = (v-*center).length();
    }
    if (surfsetr) {
        surfsetr->select(v);
        QVector3D* rsel = surfsetr->selected;
        if ((v-*rsel).length() < dist){
            center = surfsetr->selected;
            selected = surfsetr;
        }
    }
}

void Data::setGlyphRadius(double glyphRadius){
    if (surfset) surfset->glyphRadius = glyphRadius;
    if (surfsetr) surfsetr->glyphRadius = glyphRadius;
}

void Data::setGlyphAlpha(double glyphAlpha){
    if (surfset) surfset->glyphAlpha = glyphAlpha;
    if (surfsetr) surfsetr->glyphAlpha = glyphAlpha;
}

void Data::setThreshold(double stuffAlpha){
    if (surfset) surfset->threshold = stuffAlpha;
    if (surfsetr) surfsetr->threshold = stuffAlpha;
}

void Data::setAlpha(double brainAlpha){
    if (surfl) surfl->setAlpha(brainAlpha);
    if (surfr) surfr->setAlpha(brainAlpha);
}

void Data::switchToSurface(int i){
    qDebug() << "switching l";
    if (surfset) {
        qDebug() << "switching surface l";
        surfset->switchSurface(i);
        surfl = surfset->surfs.at(i);
    }

    qDebug() << "switching r";
    if (surfsetr) {
        qDebug() << "switching surface r";
        surfsetr->switchSurface(i);
        surfr = surfsetr->surfs.at(i);
    }
    QVector3D* s = NULL;
    if (selected) s = (QVector3D*)&selected->scons.at(selected->cs)->nodes.at(selected->selectedIndex);
    if (s) center = s;
}

void Data::setNormalization(float n){
    if (surfset) surfset->norm = n;
    if (surfsetr) surfsetr->norm = n;
}

void Data::setSize(float s){
    if (surfset) surfset->size = s;
    if (surfsetr) surfsetr->size = s;
}

void Data::colorFrom(int i){
    if (surfset) surfset->colorsFrom = i;
    if (surfsetr) surfsetr->colorsFrom =i;
}

void Data::geoFrom(int i){
    if (surfset) surfset->geo = i;
    if (surfsetr) surfsetr->geo = i;
}

QVector3D* Data::selPos(){
    if (selected){
        return (QVector3D*)&selected->scons.at(selected->cs)->nodes.at(selected->selectedIndex);
    } else {
        return NULL;
    }
}

void Data::paintSelected(){

    QVector3D* s = selPos();

    glDisable(GL_DEPTH_TEST);
    glPointSize(5);
    glColor3f(0,1,0);
    glBegin(GL_POINTS);
    if (s) glVertex3f(s->x(),s->y(),s->z());
    glEnd();
    glEnable(GL_DEPTH_TEST);
}

void Data::setSelectedIndex(int i){

}
