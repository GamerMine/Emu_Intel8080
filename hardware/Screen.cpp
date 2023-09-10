//
// Created by maxime on 03/08/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_screen.h" resolved

#include "Screen.h"
#include "ui_Screen.h"

Screen::Screen(QWidget *parent, Bus *bus) : QWidget(parent), ui(new Ui::Screen) {
    ui->setupUi(this);

    main_bus = bus;
    color_mode = false;

    layout = new QBoxLayout(QBoxLayout::TopToBottom);
    image_label = new QLabel();
    menu_bar = new QMenuBar();
    setting_menu = new QMenu("Settings");
    add_color = new QAction("Enable colors");
    edit_dips = new QAction(tr("Game configuration"));
    image = new QImage(256, 224, QImage::Format_RGB32);
    image->fill(QColor::fromRgb(0));

    add_color->setCheckable(true);
    setting_menu->addAction(add_color);
    setting_menu->addAction(edit_dips);

    menu_bar->addMenu(setting_menu);

    image_label->setPixmap(QPixmap::fromImage(image->scaled(224*3.8, 256*3.8)));

    layout->addWidget(menu_bar);
    layout->addWidget(image_label);
    layout->setContentsMargins(0, 0, 0, 0);

    this->setLayout(layout);
    this->setFixedSize(224*3.8, 256*3.8);

    connect(add_color, &QAction::triggered, this, &Screen::enableColorTriggered);
    connect(edit_dips, &QAction::triggered, this, &Screen::editGameConfigTriggered);
}

Screen::~Screen() {
    delete ui;
}

void Screen::imageReceived(QImage image) {
    auto rotated = image.transformed(QTransform().rotate(-90.0));
    auto scaled = rotated.scaled(224*3.8, 256*3.8);
    image_label->setPixmap(QPixmap::fromImage(scaled));
}

void Screen::enableColorTriggered() {
    if (add_color->isChecked()) {
        add_color->setText("Disable colors");
        Screen::color_mode = true;
    } else {
        add_color->setText("Enable colors");
        Screen::color_mode = false;
    }
}

void Screen::editGameConfigTriggered() {
    GameConfigDialog *dialog = new GameConfigDialog(this, main_bus);
    (void) dialog;
}

void Screen::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
        case Qt::Key_Ampersand: main_bus->i_port_1 = main_bus->i_port_1 | 0b00000100; break; // One player button
        case Qt::Key_Enter:     main_bus->i_port_1 = main_bus->i_port_1 | 0b00000001; break; // Credit
        case Qt::Key_Right:     main_bus->i_port_1 = main_bus->i_port_1 | 0b01000000; break; // P1 Right
        case Qt::Key_Left:      main_bus->i_port_1 = main_bus->i_port_1 | 0b00100000; break; // P1 Left
        case Qt::Key_Space:     main_bus->i_port_1 = main_bus->i_port_1 | 0b00010000; break; // P1 Shot
    }
}

void Screen::keyReleaseEvent(QKeyEvent *e) {
    switch (e->key()) {
        case Qt::Key_Ampersand: main_bus->i_port_1 = main_bus->i_port_1 & 0b11111011; break; // One player button
        case Qt::Key_Enter:     main_bus->i_port_1 = main_bus->i_port_1 & 0b11111110; break; // Credit
        case Qt::Key_Right:     main_bus->i_port_1 = main_bus->i_port_1 & 0b10111111; break; // P1 Right
        case Qt::Key_Left:      main_bus->i_port_1 = main_bus->i_port_1 & 0b11011111; break; // P1 Left
        case Qt::Key_Space:     main_bus->i_port_1 = main_bus->i_port_1 & 0b11101111; break; // P1 Shot
    }
}
