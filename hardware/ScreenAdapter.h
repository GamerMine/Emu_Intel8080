//
// Created by maxime on 12/08/23.
//

#ifndef EMU_INTEL8080_SCREENADAPTER_H
#define EMU_INTEL8080_SCREENADAPTER_H

#include <QThread>
#include "Bus.h"

#ifdef _WIN32
    #include "windows.h"
#endif

class ScreenAdapter : public QThread {
    Q_OBJECT
public:
    explicit ScreenAdapter(Bus *bus);

private:
    Bus *main_bus;
    QImage *image;
    QColor current_color;

protected:
    [[noreturn]] void run() override;

signals:
    void sendImage(QImage image);
    void setInterrupt(uint16_t value);
};


#endif //EMU_INTEL8080_SCREENADAPTER_H
