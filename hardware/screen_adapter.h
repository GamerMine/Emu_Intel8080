//
// Created by maxime on 12/08/23.
//

#ifndef EMU_INTEL8080_SCREEN_ADAPTER_H
#define EMU_INTEL8080_SCREEN_ADAPTER_H

#include <QThread>
#include "bus.h"

class screen_adapter : public QThread {
    Q_OBJECT
public:
    explicit screen_adapter(bus *bus);

private:
    bus *main_bus;
    QImage *image;

protected:
    [[noreturn]] void run() override;

signals:
    void sendImage(QImage image);
    void setInterrupt(uint16_t value);
};


#endif //EMU_INTEL8080_SCREEN_ADAPTER_H
