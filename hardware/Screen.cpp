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

    image_height = this->screen()->size().height() - 10;
    image_width = 7 * image_height / 8;

    main_layout = new BorderLayout(0);
    image_label = new QLabel();
    menu_bar = new QMenuBar();
    setting_menu = new QMenu("Settings");
    add_color = new QAction("Enable colors");
    edit_dips = new QAction("Game configuration");
    edit_keybinds = new QAction("Keybinds");
    image = new QImage(image_width, image_height, QImage::Format_RGB32);
    image->fill(QColor::fromRgb(0));

    add_color->setCheckable(true);
    setting_menu->addAction(add_color);
    setting_menu->addAction(edit_dips);
    setting_menu->addAction(edit_keybinds);

    menu_bar->addMenu(setting_menu);

    image_label->setPixmap(QPixmap::fromImage(image->scaled(image_width, image_height)));
    image_label->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
    image_label->setAlignment(Qt::AlignCenter);

    main_layout->addWidget(menu_bar, BorderLayout::North);
    main_layout->addWidget(image_label, BorderLayout::Center);

    this->setLayout(main_layout);
    this->setWindowTitle("Space Invaders (Intel 8080 Emulator)");
    this->setWindowIcon(QIcon("resources/images/icon.png"));

    connect(add_color, &QAction::triggered, this, &Screen::enableColorTriggered);
    connect(edit_dips, &QAction::triggered, this, &Screen::editGameConfigTriggered);
    connect(edit_keybinds, &QAction::triggered, this, &Screen::editKeybindsTriggered);
}

Screen::~Screen() {
    delete ui;
}

void Screen::imageReceived(const QImage& img) {
    auto rotated = img.transformed(QTransform().rotate(-90.0));
    auto scaled = rotated.scaled(image_width, image_height);
    image_label->setPixmap(QPixmap::fromImage(scaled));
}

void Screen::enableColorTriggered() {
    QSettings settings("config.conf", QSettings::IniFormat);
    settings.beginGroup("Game Configuration");
    settings.setValue(SETTING_ENABLE_COLOR, add_color->isChecked());

    if (add_color->isChecked()) {
        add_color->setText("Disable colors");
        Screen::color_mode = true;
    } else {
        add_color->setText("Enable colors");
        Screen::color_mode = false;
    }
}

void Screen::editGameConfigTriggered() {
    auto *dialog = new GameConfigDialog(this, main_bus);
    (void) dialog;
}

void Screen::editKeybindsTriggered() {
    auto *dialog = new KeybindsDialog(this, main_bus);
    (void) dialog;
}

void Screen::toggleFullscreen() {
    if (isFullScreen()) {
        menu_bar->show();
        setStyleSheet("");
        showMaximized();
    } else {
        menu_bar->hide();
        setStyleSheet("background-color: black;");
        showFullScreen();
    }
}

/*
 * ------------------
 * | Events section |
 * ------------------
 */

void Screen::keyPressEvent(QKeyEvent *e) {
    QSettings settings("config.conf", QSettings::IniFormat);
    settings.beginGroup("Keybinds");

    if (e->key() == settings.value(SETTING_START, Qt::Key_Ampersand).toInt())       main_bus->i_port_1 = main_bus->i_port_1 | 0b00000100;
    else if (e->key() == settings.value(SETTING_ADD_CREDIT, Qt::Key_Enter).toInt()) main_bus->i_port_1 = main_bus->i_port_1 | 0b00000001;
    else if (e->key() == settings.value(SETTING_KEY_RIGHT, Qt::Key_Right).toInt())  main_bus->i_port_1 = main_bus->i_port_1 | 0b01000000;
    else if (e->key() == settings.value(SETTING_KEY_LEFT, Qt::Key_Left).toInt())    main_bus->i_port_1 = main_bus->i_port_1 | 0b00100000;
    else if (e->key() == settings.value(SETTING_FIRE, Qt::Key_Space).toInt())       main_bus->i_port_1 = main_bus->i_port_1 | 0b00010000;

    switch (e->key()) {
        case Qt::Key_F11:       toggleFullscreen();                                   break; // Fullscreen toggle
    }
}

void Screen::keyReleaseEvent(QKeyEvent *e) {
    QSettings settings("config.conf", QSettings::IniFormat);
    settings.beginGroup("Keybinds");

    if (e->key() == settings.value(SETTING_START, Qt::Key_Ampersand).toInt())       main_bus->i_port_1 = main_bus->i_port_1 & 0b11111011;
    else if (e->key() == settings.value(SETTING_ADD_CREDIT, Qt::Key_Enter).toInt()) main_bus->i_port_1 = main_bus->i_port_1 & 0b11111110;
    else if (e->key() == settings.value(SETTING_KEY_RIGHT, Qt::Key_Right).toInt())  main_bus->i_port_1 = main_bus->i_port_1 & 0b10111111;
    else if (e->key() == settings.value(SETTING_KEY_LEFT, Qt::Key_Left).toInt())    main_bus->i_port_1 = main_bus->i_port_1 & 0b11011111;
    else if (e->key() == settings.value(SETTING_FIRE, Qt::Key_Space).toInt())       main_bus->i_port_1 = main_bus->i_port_1 & 0b11101111;
}

void Screen::resizeEvent(QResizeEvent *e) {
    if (e->size().width() + 100 < e->size().height()) {
        image_width = e->size().width();
        image_height = 8 * image_width / 7;
    } else {
        image_height = e->size().height() - 10;
        image_width = 7 * image_height / 8;
    }
}
