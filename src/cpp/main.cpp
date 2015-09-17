#include <QApplication>
#include <QtDebug>
#include <QtQml>
#include "ui/mainwindow.h"
#include "version.h"
#include "settings.h"
#include "utils.h"
#include "console.h"
#include "sproutdb.h"
#include "sproutc.h"
#include "project.h"

QSharedPointer<Settings> settings;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Greenery");
    QCoreApplication::setApplicationVersion(Version::full());

    QCommandLineParser parser;
    parser.setApplicationDescription("IDE for Sprout language");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("source", QCoreApplication::translate("main", "Sprout file to edit"));

    parser.addOptions({
        {{"q", "qml"}, QCoreApplication::translate("main", "QML Window")},
    });

    parser.process(app);
    bool isQml = parser.isSet("qml");

    ::settings = QSharedPointer<Settings>(new Settings());
    QPointer<QQmlApplicationEngine> engine;
    QPointer<MainWindow> mainWindow;

    if (isQml) {
        qmlRegisterType<Console>("Greenery.Lib", 1, 0, "Console");
        qmlRegisterType<SproutDb>("Greenery.Lib", 1, 0, "SproutDb");

        Utils* utils = new Utils();
        Project* project = new Project();
        Version* version = new Version();

        engine = QPointer<QQmlApplicationEngine>(new QQmlApplicationEngine());

        engine->rootContext()->setContextProperty("PROJECT", project);
        engine->rootContext()->setContextProperty("UTILS", utils);
        engine->rootContext()->setContextProperty("SETTINGS", ::settings.data());
        engine->rootContext()->setContextProperty("VERSION", version);
        engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    } else {
        mainWindow = QPointer<MainWindow>(new MainWindow());
        mainWindow->show();
    }

    int exitCode = app.exec();

    if (!mainWindow.isNull()) {
        delete mainWindow;
    }

    if (!engine.isNull()) {
        delete engine;
    }

    return exitCode;
}
