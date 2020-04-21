#include "video_provider.h"
#include "image_provider.h"
#include "image_item.h"
#include "audio_provider.h"

#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QTimer>
#include <QQuickView>
#include <QQmlContext>
#include <QObject>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    //video provide
    auto videoProvider = new VideoProvider{&app};
    engine.addImageProvider(QLatin1String("VideoProvider"), videoProvider);
    engine.rootContext()->setContextProperty("VideoProvider", videoProvider);

    //audio provider
    auto audioProvider = new AudioProvider{&app};
    engine.rootContext()->setContextProperty("AudioProvider", audioProvider);

    //image provider
    qmlRegisterType<ImageItem>("imageProvider", 1, 0, "ImageItem");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
