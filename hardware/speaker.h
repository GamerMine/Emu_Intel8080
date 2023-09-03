//
// Created by maxime on 16/08/23.
//

#ifndef EMU_INTEL8080_SPEAKER_H
#define EMU_INTEL8080_SPEAKER_H

#include <QThread>
#include <AL/al.h>
#include <AL/alc.h>
#include "AudioFile.h"
#include "bus.h"
#include "../utils/WaveFileLoader.h"

class speaker : public QThread {
    Q_OBJECT
public:
    explicit speaker(class bus *bus);

protected:
    void run() override;

private:
    bus *main_bus;
    ALuint source;
};


#endif //EMU_INTEL8080_SPEAKER_H
