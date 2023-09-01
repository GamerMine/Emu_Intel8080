//
// Created by maxime on 03/08/23.
//

#ifndef EMU_INTEL8080_SCREEN_H
#define EMU_INTEL8080_SCREEN_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QKeyEvent>
#include "bus.h"


QT_BEGIN_NAMESPACE
namespace Ui { class screen; }
QT_END_NAMESPACE

class screen : public QWidget {
Q_OBJECT

public:
    explicit screen(QWidget *parent = nullptr, bus *bus = nullptr);

    ~screen() override;

public slots:
    void imageReceived(QImage image);

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

private:
    bus *main_bus;

    Ui::screen *ui;
    QImage *image;
    QLabel *image_label;
    QBoxLayout *layout;
};


#endif //EMU_INTEL8080_SCREEN_H
