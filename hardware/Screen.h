//
// Created by maxime on 03/08/23.
//

#ifndef EMU_INTEL8080_SCREEN_H
#define EMU_INTEL8080_SCREEN_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QKeyEvent>
#include "Bus.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Screen; }
QT_END_NAMESPACE

class Screen : public QWidget {
Q_OBJECT

public:
    explicit Screen(QWidget *parent = nullptr, Bus *bus = nullptr);

    ~Screen() override;

public slots:
    void imageReceived(QImage image);

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

private:
    Bus *main_bus;

    Ui::Screen *ui;
    QImage *image;
    QLabel *image_label;
    QBoxLayout *layout;
};


#endif //EMU_INTEL8080_SCREEN_H
