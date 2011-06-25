#include <QtGui/QApplication>
#include <QDeclarativeContext>

#include "qmlapplicationviewer.h"
#include "filebrowser.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    File file;
    QmlApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("file", &file);
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/QMLJSDebugger/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
