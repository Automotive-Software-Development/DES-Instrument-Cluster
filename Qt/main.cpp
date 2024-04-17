#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <datareceiver.h>
#include <socketclient.h>
#include<QQmlContext>
#include <QQuickWindow>

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
    QObject::connect(&obj, SIGNAL(gearChanged()), &obj, SLOT(gear()));
    QObject::connect(&obj, SIGNAL(indicatorChanged()), &obj, SLOT(indicator()));

    QQmlContext * rootContext = engine.rootContext();
    rootContext->setContextProperty("Instrument_Cluster", &obj);

    SocketClient socketObj;
    QQmlContext * rootContext1 = engine.rootContext();
    rootContext1->setContextProperty("socketClient", &socketObj);

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

    if (!engine.rootObjects().isEmpty()) {
        auto window = qobject_cast<QQuickWindow*>(engine.rootObjects().first());
        if (window) {
            window->showFullScreen();
        }
    }

    return app.exec();
}
