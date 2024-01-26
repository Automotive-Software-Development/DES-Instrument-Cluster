#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <datareceiver.h>
#include<QQmlContext>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    DataReceiver obj;
    QObject::connect(&obj, SIGNAL(rpsChanged()), &obj, SLOT(rps()));
    QObject::connect(&obj, SIGNAL(batteryChanged()), &obj, SLOT(battery()));

    QQmlContext * rootContext = engine.rootContext();
    rootContext->setContextProperty("Instrument_Cluster", &obj);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
