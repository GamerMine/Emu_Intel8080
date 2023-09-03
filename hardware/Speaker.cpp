//
// Created by maxime on 16/08/23.
//

#include "Speaker.h"

Speaker::Speaker(Bus *bus) {
    main_bus = bus;

    ALCdevice *device = alcOpenDevice(NULL);
    if (!device) qCritical("ERROR: Could not open audio device");

    ALCcontext *context = alcCreateContext(device, nullptr);
    if (!context) qCritical("ERROR: Could not create audio context");
    alcMakeContextCurrent(context);

    alGenSources(1, &source);
    alSourcef(source, AL_PITCH, 1);
    alSourcef(source, AL_GAIN, 0.5f);
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_LOOPING, AL_TRUE);
}

void Speaker::run() {
    ALuint buffer;
    alGenBuffers(1, &buffer);

    std::vector<uint8_t> data = WaveFileLoader::getPCMData("sounds/Lanmou_Soley.wav");
    uint32_t sampleRate = WaveFileLoader::getFormat("sounds/Lanmou_Soley.wav").sample_rate;

    alBufferData(buffer, AL_FORMAT_MONO8, data.data(), data.size(), sampleRate);

    alSourcei(source, AL_BUFFER, buffer);
    alSourcePlay(source);
}
