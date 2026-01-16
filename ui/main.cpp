#include <QApplication>
#include "mainwindow.h"
#include "config.h"

int main(int argc, char *argv[]) {

	ConfigManager::instance().load("config.toml");

    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    return app.exec();
}
