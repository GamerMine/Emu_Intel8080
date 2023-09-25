#include "KeybindsDialog.h"

KeybindsDialog::KeybindsDialog(QWidget *parent, Bus *bus) : QWidget(parent, Qt::Window) {
    QSettings settings("config.conf", QSettings::IniFormat);
    settings.beginGroup("Keybinds");
    main_bus = bus;
    is_assigning = false;

    //printf("%s", seq.toString().toStdString().c_str());
    auto *lbl_move_right    = new QLabel("Move Right: ");
    auto *lbl_move_left     = new QLabel("Move Left: ");
    auto *lbl_fire          = new QLabel("Fire: ");
    auto *lbl_insert_credit = new QLabel("Insert Credit: ");
    auto *lbl_start         = new QLabel("Start Game: ");
    auto *lyt_grid          = new QGridLayout();

    btn_group           = new QButtonGroup();
    btn_move_right      = new QPushButton(QKeySequence(settings.value(SETTING_KEY_RIGHT, Qt::Key_Right).toInt()).toString());
    btn_move_left       = new QPushButton(QKeySequence(settings.value(SETTING_KEY_LEFT, Qt::Key_Left).toInt()).toString());
    btn_fire            = new QPushButton(QKeySequence(settings.value(SETTING_FIRE, Qt::Key_Space).toInt()).toString());
    btn_insert_credit   = new QPushButton(QKeySequence(settings.value(SETTING_ADD_CREDIT, Qt::Key_Enter).toInt()).toString());
    btn_start           = new QPushButton(QKeySequence(settings.value(SETTING_START, Qt::Key_Ampersand).toInt()).toString());
    btn_move_right->setProperty("id", SETTING_KEY_RIGHT);
    btn_move_left->setProperty("id", SETTING_KEY_LEFT);
    btn_fire->setProperty("id", SETTING_FIRE);
    btn_insert_credit->setProperty("id", SETTING_ADD_CREDIT);
    btn_start->setProperty("id", SETTING_START);
    btn_group->addButton(btn_move_right);
    btn_group->addButton(btn_move_left);
    btn_group->addButton(btn_fire);
    btn_group->addButton(btn_insert_credit);
    btn_group->addButton(btn_start);

    lyt_grid->addWidget(lbl_move_right, 0, 0);
    lyt_grid->addWidget(btn_move_right, 0, 1);
    lyt_grid->addWidget(lbl_move_left, 1, 0);
    lyt_grid->addWidget(btn_move_left, 1, 1);
    lyt_grid->addWidget(lbl_fire, 2, 0);
    lyt_grid->addWidget(btn_fire, 2, 1);
    lyt_grid->addWidget(lbl_insert_credit, 3, 0);
    lyt_grid->addWidget(btn_insert_credit, 3, 1);
    lyt_grid->addWidget(lbl_start, 4, 0);
    lyt_grid->addWidget(btn_start, 4, 1);

    lyt_grid->setRowMinimumHeight(0, 30);

    this->setMinimumSize(300, 200);
    this->setLayout(lyt_grid);
    this->setWindowTitle("Keybinds");
    this->show();

    connect(btn_move_right, &QPushButton::clicked, this, &KeybindsDialog::assignKey);
    connect(btn_move_left, &QPushButton::clicked, this, &KeybindsDialog::assignKey);
    connect(btn_fire, &QPushButton::clicked, this, &KeybindsDialog::assignKey);
    connect(btn_insert_credit, &QPushButton::clicked, this, &KeybindsDialog::assignKey);
    connect(btn_start, &QPushButton::clicked, this, &KeybindsDialog::assignKey);
}

void KeybindsDialog::assignKey() {
    clicked_button = (QPushButton*)sender();
    if (!clicked_button) return;

    clicked_button->setText("Press a key...");
    for (QAbstractButton *btn : btn_group->buttons()) {
        btn->setDisabled(true);
    }

    is_assigning = true;
}

void KeybindsDialog::keyPressEvent(QKeyEvent *event) {
    if (is_assigning) {
        QSettings settings("config.conf", QSettings::IniFormat);
        settings.beginGroup("Keybinds");
        clicked_button->setText(QKeySequence(event->key()).toString());

        settings.setValue(clicked_button->property("id").toString(), event->key());

        for (QAbstractButton *btn : btn_group->buttons()) {
            btn->setDisabled(false);
        }

        is_assigning = false;
        clicked_button = nullptr;
    }
}
