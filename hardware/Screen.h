//
// Created by maxime on 03/08/23.
//

#ifndef EMU_INTEL8080_SCREEN_H
#define EMU_INTEL8080_SCREEN_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QMenu>
#include <QMenuBar>

#include "Bus.h"
#include "../widgets/GameConfigDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Screen; }
QT_END_NAMESPACE

class Screen : public QWidget {
Q_OBJECT

public:
    explicit Screen(QWidget *parent = nullptr, Bus *bus = nullptr);
    ~Screen() override;

    static inline bool color_mode = false;

public slots:
    void imageReceived(QImage image);
    void enableColorTriggered();
    void editGameConfigTriggered();

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

private:
    Bus *main_bus;

    Ui::Screen *ui;
    QImage *image;
    QLabel *image_label;
    QBoxLayout *layout;
    QMenuBar *menu_bar;
    QMenu *setting_menu;
    QAction *add_color;
    QAction *edit_dips;
};


#endif //EMU_INTEL8080_SCREEN_H
