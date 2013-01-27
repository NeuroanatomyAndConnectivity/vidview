#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

#include "qstring.h"
#include "indexedconnections.h"

class Connectivity
{
public:
    Connectivity(QString filename);
    IndexedConnections* getICs(float thresh);
    float** conn;
    int n;
};

#endif // CONNECTIVITY_H
