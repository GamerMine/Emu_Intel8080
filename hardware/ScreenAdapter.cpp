#include "ScreenAdapter.h"
#include "Screen.h"

Screen *win;

ScreenAdapter::ScreenAdapter(Bus *bus) {
    QSettings settings("config.conf", QSettings::IniFormat);

    main_bus = bus;

    win = new Screen(nullptr, bus);
    win->showMaximized();

    QObject::connect(this, &ScreenAdapter::sendImage, win, &Screen::imageReceived);

    image = new QImage(256, 224, QImage::Format_RGB32);
    current_color = QColor::fromRgb(0xFF, 0xFF, 0xFF);
    settings.beginGroup("Game Configuration");
    Screen::color_mode = settings.value(SETTING_ENABLE_COLOR, false).toBool();
}

[[noreturn]] void ScreenAdapter::run() {
    while (true) {
        #ifdef _WIN32
            Sleep(16);
        #else
            usleep(16667);
        #endif
        image->fill(QColor::fromRgb(0));
        for (int y = 0; y < 224; y++) {
            for (int x = 0; x < 32; x++) {
                if (x >= 24 && x <= 26 && Screen::color_mode) current_color = QColor::fromRgb(0xE6, 0x8E, 0x51);
                else if ((((x >= 0 && y >= 25 && y <= 102) && x <= 7) || (x >= 2 && x <= 7)) && Screen::color_mode) current_color = QColor::fromRgb(0x00, 0xB5, 0x9B);
                else current_color = QColor::fromRgb(0xFF, 0xFF, 0xFF);
                for (int z = 0; z < 8; z++) {
                    image->setPixelColor(x * 8 + z, y, ((main_bus->mem[0x2400 + x + 32 * y] >> z) & 0x1) == 1 ? current_color : QColor::fromRgb(0));
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
