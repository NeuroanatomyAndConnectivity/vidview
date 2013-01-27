#include "connectivity.h"
#include "qfile.h"
#include "qdebug.h"

#include "qmath.h"

Connectivity::Connectivity(QString filename)
{


    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) qDebug() << "binary connectivity unreadable: " << filename;

    //This assumes a square matrix of float32...
    n = qSqrt(f.size()/4);
    qDebug() << "connectivity matrix size: " << n;
    conn = new float*[n];
    for (int i = 0; i<n; i++){
        conn[i] = new float[n];
    }

    QDataStream ins(&f);
    //TODO: What's up with the byte order? Do we have to offer an option for that crap?
    ins.setByteOrder(QDataStream::LittleEndian);
    ins.setFloatingPointPrecision(QDataStream::SinglePrecision);
    qDebug() << "reading binary connectivity between " << n << " nodes...";

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            ins >> conn[i][j];
            //qDebug() << i << j << conn[i][j];
        }
    }
    f.close();
    qDebug() << "connectivity read";
}

IndexedConnections* Connectivity::getICs(float thresh){
    //TODO: Check that the upper triangle thing is correct...
    IndexedConnections* ics = new IndexedConnections();
    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){
            float v = conn[i][j];
            if ( (v>thresh) && (v<1)) {
                ics->add(i,j,v);

            }
        }
    }
    qDebug() << "ics.length in getICs: " << ics->length();
    return ics;
}
