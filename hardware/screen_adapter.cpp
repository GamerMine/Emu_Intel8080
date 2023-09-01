//
// Created by maxime on 12/08/23.
//

#include "screen_adapter.h"
#include "screen.h"

screen *win;

screen_adapter::screen_adapter(bus *bus) {
    main_bus = bus;

    win = new screen(nullptr, bus);
    win->show();

    QObject::connect(this, &screen_adapter::sendImage, win, &screen::imageReceived);

    image = new QImage(256, 224, QImage::Format_RGB32);
}

[[noreturn]] void screen_adapter::run() {
    while (true) {
        usleep(16667);
        image->fill(QColor::fromRgb(0));
        for (int y = 0; y < 224; y++) {
            for (int x = 0; x < 32; x++) {
                for (int z = 0; z < 8; z++) {
                    image->setPixelColor(x * 8 + z, y, ((main_bus->mem[0x2400 + x + 32 * y] >> z) & 0x1) == 1 ? QColor::fromRgb(255, 255, 255) : QColor::fromRgb(0));
                }
            }
            if (y == 96) {
                emit setInterrupt(0x0008);
            }
        }

        emit setInterrupt(0x0010);

        emit sendImage(*image);
    }
}
