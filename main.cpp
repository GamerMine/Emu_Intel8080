#include <QApplication>

#include "hardware/Bus.h"
#include "hardware/Screen.h"

int main(int argc, char *argv[]) {
    QApplication::addLibraryPath("lib");
    QApplication app(argc, argv);

    Bus bus;

    return app.exec();
}
