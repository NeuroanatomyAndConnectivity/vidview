#-------------------------------------------------
#
# Project created by QtCreator 2012-01-25T20:22:32
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = vidview
TEMPLATE = app
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp -lboost_program_options -L/usr/lib
INCLUDEPATH += /usr/include

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    surface.cpp \
    triangle.cpp \
    primitive.cpp \
    node.cpp \
    surfaceset.cpp \
    afnisurface.cpp \
    connection.cpp \
    sconnections.cpp \
    indexedconnections.cpp \
    data.cpp \
    connectivity.cpp \
    glypheditor.cpp \
    surfacecollection.cpp \
    displayparameters.cpp \
    glglyphview.cpp \

HEADERS  += mainwindow.h \
    glwidget.h \
    surface.h \
    triangle.h \
    primitive.h \
    node.h \
    surfaceset.h \
    afnisurface.h \
    connection.h \
    sconnections.h \
    indexedconnections.h \
    data.h \
    indexedconnections.h \
    connectivity.h \
    glypheditor.h \
    surfacecollection.h \
    displayparameters.h \
    glglyphview.h \

FORMS    += mainwindow.ui \
    glypheditor.ui \
    displayparameters.ui
