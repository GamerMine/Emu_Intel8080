//
// Created by maxime on 16/08/23.
//

#include "speaker.h"

speaker::speaker(bus *bus) {
    main_bus = bus;

    ALCdevice *device = alcOpenDevice(NULL);
    if (!device) qCritical("ERROR: Could not open audio device");

    ALCcontext *context = alcCreateContext(device, nullptr);
    if (!context) qCritical("ERROR: Could not create audio context");
    alcMakeContextCurrent(context);

    alGenSources(1, &source);
    alSourcef(source, AL_PITCH, 1);
    alSourcef(source, AL_GAIN, 0.1f);
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_LOOPING, AL_FALSE);
}

void speaker::run() {
    ALuint buffer;
    alGenBuffers(1, &buffer);

    AudioFile<int> audioFile;
    audioFile.load("sounds/Lanmou Soley.wav");

    /*alBufferData(buffer, AL_FORMAT_STEREO16, audioFile.raw_samples.data(), audioFile.raw_samples.size(), audioFile.getSampleRate());

    alSourcei(source, AL_BUFFER, buffer);
    alSourcePlay(source);*/
}
