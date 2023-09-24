#ifndef EMU_INTEL8080_GAMECONFIGDIALOG_H
#define EMU_INTEL8080_GAMECONFIGDIALOG_H

#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include <QSettings>

#include "../hardware/Bus.h"
#include "../utils/Settings.h"

class GameConfigDialog : public QWidget {
    Q_OBJECT
public:
    GameConfigDialog(QWidget *parent, Bus *bus);

public slots:
    void btnCancelClicked();
    void btnApplyClicked();

private:
    Bus *main_bus;

    QComboBox *cb_nb_ships;
    QComboBox *cb_coin_info;
    QComboBox *cb_bonus_life;
    QPushButton *btn_cancel;
    QPushButton *btn_apply;
};


#endif //EMU_INTEL8080_GAMECONFIGDIALOG_H
