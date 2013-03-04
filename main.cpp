#include <QtGui/QApplication>
#include "mainwindow.h"

#include "QDebug"
#include "glwidget.h"
#include "glypheditor.h"
#include "displayparameters.h"
#include "glglyphview.h"

#include "qmath.h"

#include "boost/program_options.hpp"

#include "iostream"
#include "fstream"

#include "qfiledialog.h"

namespace po = boost::program_options;
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    string surfsetl;
    string surfsetr;
    string scons;
    string config;
    string overlayl;
    string overlayr;
    string view;
    string screenshotname;

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("surfsetl", po::value<string>(&surfsetl), "Set of surfaces for the left hemisphere")
            ("surfsetr", po::value<string>(&surfsetr), "Set of surfaces for the right hemisphere")
            ("overlayl", po::value<string>(&overlayl), "Set of surfaces for the left hemisphere")
            ("overlayr", po::value<string>(&overlayr), "Set of surfaces for the right hemisphere")
            ("scons", po::value<string>(&scons), "Connectivity (.bin) or indexed connections (.txt)")
            ("config", po::value<string>(&config), "Config file containing these here options")
            ("view", po::value<string>(&view), "Load file with view matrix")
            ("leftalpha", po::value<float>(), "alpha value for left hemisphere")
            ("rightalpha", po::value<float>(), "alpha value for right hemisphere")
            ("glyphs", po::value<float>(), "alpha value for glyphs")
            ("threshold", po::value<float>(), "threshold for edges to be displayed")
            ("clipthreshold", po::value<float>(), "threshold determining how many connections are included from a full (.bin) connectivity matrix")
            ("radius", po::value<float>(), "glyph radius")
            ("normalization", po::value<float>(), "normalization between 0 (no normalization) and 1 (completely normalized)")
            ("primsize", po::value<float>(), "size of display primitives (pointsize or linewidth, in pixels)")
            ("display", po::value<int>(), "number of surface in surface set that is displayed")
            ("color", po::value<int>(), "number of surface in surface set that determines the coloration of the glyphs")
            ("geometry", po::value<int>(), "number of surface in surface set that determines the geometric distribution of the glyph primitives")
            ("glyphstyle", po::value<int>(), "0 = geometry, 1 = projection, 2 = pie charts")
            ("primitives", po::value<int>(), "0 = points, 1 = vectors")
            ("minlength", po::value<float>(), "length threshold for removal of short edges")
            ("sresolution", po::value<float>(), "resolution multiplication factor for screenshots")
            ("minlsource", po::value<int>(), "number of surface in surface set used for the calculation of length for short edge removal")
            ("screenshot", po::value<string>(&screenshotname), "make screenshot and exit")
            ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        //TODO: Any way around using std::cout?
        std::cout << desc;
        return 1;
    }

    //QString qconfig(config.c_str());
    //string stdstr = qconfig.toStdString();
    ifstream ifs(config.c_str());
    if (!ifs) {
        cout << "can not open config file: " << config << "\n";
    } else {
        store(parse_config_file(ifs, desc), vm);
        notify(vm);
    }

    MainWindow w(NULL);
    w.setWindowTitle("Glyph Viewer");
    GLWidget* glw = w.centralWidget()->findChild<GLWidget*>("widget");
    QString qsurfsetl(surfsetl.c_str());
    QString qsurfsetr(surfsetr.c_str());
    QString qscons(scons.c_str());
    QString qoverlayl(overlayl.c_str());
    QString qoverlayr(overlayr.c_str());
    QString qview(view.c_str());
    QString qscreenshotname(screenshotname.c_str());
    qDebug() << "loading data: " << qsurfsetl << qsurfsetr << qscons << qoverlayl << qoverlayr;
    float clipthr = 0.7;
    if (vm.count("clipthreshold")) clipthr = vm["clipthreshold"].as<float>();
    glw->loadData(qsurfsetl,qsurfsetr,qscons, qoverlayl, qoverlayr, clipthr);
    qDebug() <<   "data loaded...";

    DisplayParameters* dp = new DisplayParameters(NULL, Qt::Window);
    dp->setWindowTitle("Display Options");
    dp->setVM(vm, desc);
    glw->setDisplayParameters(dp);

    GlyphEditor* ge = new GlyphEditor(NULL,Qt::Window);
    ge->setWindowTitle("Glyph Options");
    ge->setData(glw->data,dp);
    ge->initParams();
    ge->setVM(vm, desc);

    QObject::connect(ge,SIGNAL(changed()),glw,SLOT(updateParameters()));
    QObject::connect(glw,SIGNAL(selected()),ge->findChild<GLGlyphView*>("widget"),SLOT(updateGL()));
    ge->show();
    dp->show();
    w.show();

    if (qview!="") {
        qDebug() << "loading view: " << qview;
        glw->loadView(qview);
    }

    if (vm.count("screenshot")){
        if (vm.count("sresolution")){
            glw->screenshot(qscreenshotname,vm["sresolution"].as<float>());
        } else {
            glw->screenshot(qscreenshotname);
        }
        return 0;
    }

    return a.exec();

}
