//
// Created by maxime on 09/09/23.
//

#include "GameConfigDialog.h"

GameConfigDialog::GameConfigDialog(QWidget *parent, Bus *bus) : QWidget(parent, Qt::Window) {
    main_bus = bus;

    auto *lbl_nb_ships    = new QLabel("Number of life :");
    auto *lbl_coin_info   = new QLabel("Show coin info :");
    auto *lbl_bonus_life  = new QLabel("Bonus life at  :");
    auto *lyt_box         = new QGridLayout();

    cb_nb_ships     = new QComboBox();
    cb_coin_info    = new QComboBox();
    cb_bonus_life   = new QComboBox();

    btn_cancel  = new QPushButton(QIcon("./resources/images/cancel-circle.png"), "Cancel");
    btn_apply   = new QPushButton(QIcon("./resources/images/check-circle.png"),  "Apply");

    cb_nb_ships->addItems   (QStringList {"3 ships", "4 ships", "5 ships", "6 ships"});
    cb_coin_info->addItems  (QStringList {"Show", "Hide"});
    cb_bonus_life->addItems (QStringList {"1500 points", "1000 points"});

    switch (main_bus->i_port_2 & 0b00000011) {
        case 0b01: cb_nb_ships->setCurrentIndex(1); break;
        case 0b10: cb_nb_ships->setCurrentIndex(2); break;
        case 0b11: cb_nb_ships->setCurrentIndex(3); break;
    }
    if (main_bus->i_port_2 & 0b00001000) cb_bonus_life->setCurrentIndex(1);
    if (main_bus->i_port_2 & 0b10000000) cb_coin_info->setCurrentIndex(1);

    lyt_box->addWidget(lbl_nb_ships, 0, 0);
    lyt_box->addWidget(cb_nb_ships, 0, 1);
    lyt_box->addWidget(lbl_coin_info, 1, 0);
    lyt_box->addWidget(cb_coin_info, 1, 1);
    lyt_box->addWidget(lbl_bonus_life, 2, 0);
    lyt_box->addWidget(cb_bonus_life, 2, 1);
    lyt_box->addWidget(btn_cancel, 3, 0);
    lyt_box->addWidget(btn_apply, 3, 1);

    lyt_box->setRowMinimumHeight(3, 50);

    this->setLayout(lyt_box);
    this->show();

    connect(btn_cancel, &QPushButton::released, this, &GameConfigDialog::btnCancelClicked);
    connect(btn_apply, &QPushButton::released, this, &GameConfigDialog::btnApplyClicked);
}

void GameConfigDialog::btnApplyClicked() {
    main_bus->i_port_2 &= 0b01110100;
    switch (cb_nb_ships->currentIndex()) {
        case 1: main_bus->i_port_2 |= 0b00000001; break;
        case 2: main_bus->i_port_2 |= 0b00000010; break;
        case 3: main_bus->i_port_2 |= 0b00000011; break;
    }
    switch (cb_coin_info->currentIndex()) {
        case 1: main_bus->i_port_2 |= 0b10000000; break;
    }
    switch (cb_bonus_life->currentIndex()) {
        case 1: main_bus->i_port_2 |= 0b00001000; break;
    }

    main_bus->reset();
    this->close();
}

void GameConfigDialog::btnCancelClicked() {
    this->close();
}