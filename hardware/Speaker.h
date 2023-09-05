//
// Created by maxime on 16/08/23.
//

#ifndef EMU_INTEL8080_SPEAKER_H
#define EMU_INTEL8080_SPEAKER_H

#define NB_SOURCES 3

#include <QThread>
#include <AL/al.h>
#include <AL/alc.h>
#include "Bus.h"
#include "../utils/WaveFileLoader.h"
/*
 * This class is used for managing the sounds in the emulator
 * It is using 3 sources as 3 sounds can be played at the same time (shot, ufo and fleet movements for instance)
 * This is an async class since it has to catch any bits on the output 3 and 5 of the CPU
 */
class Speaker : public QThread {
    Q_OBJECT
public:
    explicit Speaker(class Bus *bus);

protected:
    [[noreturn]] void run() override;

private:
    Bus *main_bus;
    ALuint source[NB_SOURCES]{};
};


#endif //EMU_INTEL8080_SPEAKER_H
