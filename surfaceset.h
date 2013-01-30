#ifndef SURFACESET_H
#define SURFACESET_H

#include "surface.h"
#include "afnisurface.h"
#include "sconnections.h"
#include "connectivity.h"

#include "QMatrix4x4"
#include "QVector2D"

class SurfaceSet
{
public:
    SurfaceSet(QString filename, QString consname=NULL, QString labelname=NULL);

    QList<AFNISurface*> afnis;
    QList<Surface*> surfs;
    QList<SConnections*> scons;

    int cs; //current surface
    int colorsFrom; //current color
    int geo; //current glyph geometry
    int minSpace; //space in which length threshold is calculated
    float norm; //normalization of glyphs
    double size; //glSize for primitive drawing

    void switchSurface();
    void switchSurface(int i);

    void addSurface(QString filename, QString labelname, QVector3D shift=QVector3D(0,0,0));
    void createConnections(AFNISurface* a, IndexedConnections* icons, QList<QVector3D>* allNodes = NULL, int offset = 0);
    void calcBoundingBox();
    Connectivity* conn;

    void paintGL(int ns, bool allNodes=false, bool connect=false, bool glyphsVisible=true);
    void paintNodes(int ns);
    void paintBufferedNodes(int ns);
    void paintConnectivity();
    double glyphAlpha, glyphRadius, threshold, minlength;
    void setGlyphAlpha(double a);
    void setGlyphRadius(double r);
    QMatrix4x4* viewMatrix();
    void calcInvRot();
    QVector3D invRotX;
    QVector3D invRotY;
    QVector3D invRotZ;
    bool clear_depth;
    bool vectors;
    bool billboarding;

    QSet<int> *roi;
    void saveROI(QString filename);
    void loadROI(QString filename);
    void paintROI();
    int offset;

    QVector3D piv;
    QVector3D max, min;

    GLuint displayList;
    bool updateDisplayList;

    void select(QVector3D v);
    QVector3D* selected;
    int selectedIndex;
    void loadOverlay(QString filename);
};

#endif // SURFACESET_H
