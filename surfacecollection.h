#ifndef SURFACECOLLECTION_H
#define SURFACECOLLECTION_H

#include "connectivity.h"
#include "surfaceset.h"

class SurfaceCollection
{
public:
    SurfaceCollection(QString consname, SurfaceSet* l, SurfaceSet* r, float clipthr);

    Connectivity* conn;
    IndexedConnections* icons;
    SurfaceSet* lset;
    SurfaceSet* rset;
};

#endif // SURFACECOLLECTION_H
