//
// Created by maxime on 03/08/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_screen.h" resolved

#include "screen.h"
#include "ui_screen.h"


screen::screen(QWidget *parent, bus *bus) : QWidget(parent), ui(new Ui::screen) {
    ui->setupUi(this);

    main_bus = bus;

    layout = new QBoxLayout(QBoxLayout::TopToBottom);
    image_label = new QLabel();
    image = new QImage(256, 224, QImage::Format_RGB32);
    image->fill(QColor::fromRgb(0));

    image_label->setPixmap(QPixmap::fromImage(image->scaled(256*3, 224*3)));

    layout->addWidget(image_label);

    this->setLayout(layout);
}

screen::~screen() {
    delete ui;
}

void screen::imageReceived(QImage image) {
    auto rotated = image.transformed(QTransform().rotate(-90.0));
    auto scaled = rotated.scaled(224*3.8, 256*3.8);
    image_label->setPixmap(QPixmap::fromImage(scaled));
}

void screen::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
        case Qt::Key_Ampersand: main_bus->i_port_1 = main_bus->i_port_1 | 0b00000100; break; // One player button
        case Qt::Key_Enter:     main_bus->i_port_1 = main_bus->i_port_1 | 0b00000001; break; // Credit
        case Qt::Key_Right:     main_bus->i_port_1 = main_bus->i_port_1 | 0b01000000; break; // P1 Right
        case Qt::Key_Left:      main_bus->i_port_1 = main_bus->i_port_1 | 0b00100000; break; // P1 Left
        case Qt::Key_Space:     main_bus->i_port_1 = main_bus->i_port_1 | 0b00010000; break; // P1 Shot
    }
}

void screen::keyReleaseEvent(QKeyEvent *e) {
    switch (e->key()) {
        case Qt::Key_Ampersand: main_bus->i_port_1 = main_bus->i_port_1 & 0b11111011; break; // One player button
        case Qt::Key_Enter:     main_bus->i_port_1 = main_bus->i_port_1 & 0b11111110; break; // Credit
        case Qt::Key_Right:     main_bus->i_port_1 = main_bus->i_port_1 & 0b10111111; break; // P1 Right
        case Qt::Key_Left:      main_bus->i_port_1 = main_bus->i_port_1 & 0b11011111; break; // P1 Left
        case Qt::Key_Space:     main_bus->i_port_1 = main_bus->i_port_1 & 0b11101111; break; // P1 Shot
    }
}
