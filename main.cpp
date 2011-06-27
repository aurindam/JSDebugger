#include <QtGui/QApplication>
#include <QDeclarativeContext>

#include "qmlapplicationviewer.h"
#include "filehandler.h"
#include "debugclient.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    FileHandler fileHander;
    DebugClient debugClient;
    QmlApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("fileHandler", &fileHander);
    viewer.rootContext()->setContextProperty("debugClient", &debugClient);
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/QMLJSDebugger/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
