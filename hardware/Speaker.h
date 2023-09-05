//
// Created by maxime on 16/08/23.
//

#ifndef EMU_INTEL8080_SPEAKER_H
#define EMU_INTEL8080_SPEAKER_H

#include <QThread>
#include <AL/al.h>
#include <AL/alc.h>
#include "Bus.h"
#include "../utils/WaveFileLoader.h"

class Speaker : public QThread {
    Q_OBJECT
public:
    explicit Speaker(class Bus *bus);

protected:
    void run() override;

private:
    Bus *main_bus;
    ALuint source[3];
};


#endif //EMU_INTEL8080_SPEAKER_H
