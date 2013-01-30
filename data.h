#ifndef DATA_H
#define DATA_H

#include "surface.h"
#include "surfaceset.h"
#include "surfacecollection.h"
#include "connectivity.h"

#include "displayparameters.h"

class Data
{
public:
    Data(QString ssl, QString ssr, QString scons, QString gradientnamel, QString gradientnamer, float clipthr);

    Surface *surfr, *surfl;
    SurfaceSet *surfset, *surfsetr;
    bool firstSort;
    int nodestyle;

    bool ron, lon, connectivity, glyphsVisible;

    SurfaceSet* selected;
    void paintSelected();
    QVector3D* selPos();

    void switchToSurface(int i);
    void colorFrom(int i);
    void geoFrom(int i);
    void setNormalization(float n);
    void setSize(float s);
    void minlFrom(int minlSource);

    QVector3D* center;
    SurfaceCollection* coll;

    QString arg(QString argname);

    QVector3D max();
    QVector3D min();
    void paintGL(DisplayParameters* dp,bool shift=true, bool allNodes=false);

    void select(QVector3D v, Qt::KeyboardModifiers modifiers);

    void setGlyphRadius(double glyphRadius);
    void setGlyphAlpha(double glyphAlpha);
    void setThreshold(double stuffAlpha);
    void setAlpha(double brainAlpha);
    void setMinlength(double minlength);

    int selectedIndex;
    void setSelectedIndex(int i);
};

#endif // DATA_H
