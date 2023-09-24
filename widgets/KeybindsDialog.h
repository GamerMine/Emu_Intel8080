#ifndef EMU_INTEL8080_KEYBINDSDIALOG_H
#define EMU_INTEL8080_KEYBINDSDIALOG_H

#include <QButtonGroup>
#include <QGridLayout>
#include <QKeyEvent>
#include <QKeySequence>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QWidget>

#include "../hardware/Bus.h"
#include "../utils/Settings.h"

class KeybindsDialog : public QWidget {
    Q_OBJECT
public:
    KeybindsDialog(QWidget *parent, Bus *bus);

public slots:
    void assignKey();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Bus *main_bus;

    QButtonGroup *btn_group;
    QPushButton *btn_move_right;
    QPushButton *btn_move_left;
    QPushButton *btn_fire;
    QPushButton *btn_insert_credit;
    QPushButton *btn_start;

    QPushButton *clicked_button;

private:
    bool is_assigning;
};

#endif //EMU_INTEL8080_KEYBINDSDIALOG_H