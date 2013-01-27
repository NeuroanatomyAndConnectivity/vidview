#include "indexedconnections.h"

#include "qfile.h"
#include "qdebug.h"
#include "qstringlist.h"

IndexedConnections::IndexedConnections(){

}

IndexedConnections::IndexedConnections(QString filename){
    loadTxt(filename);
}

void IndexedConnections::add(int f, int t, float v){
    fs << f;
    ts << t;
    vs << v;
}

int IndexedConnections::length(){
    return vs.length();
}

void IndexedConnections::loadTxt(QString filename){
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) qDebug() << "unable to open file: " << filename;
    qDebug() << "reading from: " << filename;
    QTextStream stream(&f);
    QString line;
    while(!stream.atEnd()) {
        line = stream.readLine();

        QStringList vals = line.split(" ",QString::SkipEmptyParts);
        //TODO: check if this works / is necessary for scientific notation ints
        int f = ((QString)(vals.at(0))).toFloat();
        int t = ((QString)(vals.at(1))).toFloat();
        //if no values are in the file, value is set to 1 (binary graph)
        float v;
        if (vals.length()>=3) {
            v = ((QString)(vals.at(2))).toFloat();
        } else {
            v = 1;
        }
       add(f,t,v);
    }
    f.close();
}

void IndexedConnections::saveBin(QString filename){

}

void IndexedConnections::loadBin(QString filename){

}
