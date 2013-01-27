#ifndef INDEXEDCONNECTIONS_H
#define INDEXEDCONNECTIONS_H

#include "qlist.h"
#include "qstring.h"

class IndexedConnections
{
public:
    IndexedConnections();
    IndexedConnections(QString filename);
    QList<int> fs;
    QList<int> ts;
    QList<float> vs;
    int length();
    void add(int f, int t, float v);
    void loadTxt(QString filename);
    void saveBin(QString filename);
    void loadBin(QString filename);
};

#endif // INDEXEDCONNECTIONS_H
