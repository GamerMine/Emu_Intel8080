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
    alSourcef(source, AL_GAIN, 0.1f);
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_LOOPING, AL_FALSE);
}

void Speaker::run() {
    ALuint flash = WaveFileLoader::getBufferForFile("sounds/flash.wav");
    ALuint fleetMovement1 = WaveFileLoader::getBufferForFile("sounds/fleet_movement_1.wav");
    ALuint fleetMovement2 = WaveFileLoader::getBufferForFile("sounds/fleet_movement_2.wav");
    ALuint fleetMovement3 = WaveFileLoader::getBufferForFile("sounds/fleet_movement_3.wav");
    ALuint fleetMovement4 = WaveFileLoader::getBufferForFile("sounds/fleet_movement_4.wav");
    ALuint invaderDie = WaveFileLoader::getBufferForFile("sounds/invader_die.wav");
    ALuint shot = WaveFileLoader::getBufferForFile("sounds/shot.wav");
    ALuint ufo = WaveFileLoader::getBufferForFile("sounds/ufo.wav");
    ALuint ufoHit = WaveFileLoader::getBufferForFile("sounds/ufo_hit.wav");

    ALint state;
    ALint currentSound = -1;
    ALint lastSound = -1;
    ALboolean repeat = AL_FALSE;

    while (true) {
        currentSound = -1;
        repeat = AL_FALSE;
        if (main_bus->o_port_3 & 0b00000001) {
            currentSound = ufo;
            repeat = AL_TRUE;
        }
        if (main_bus->o_port_3 & 0b00000010) currentSound = shot;
        if (main_bus->o_port_3 & 0b00000100) currentSound = flash;
        if (main_bus->o_port_3 & 0b00001000) currentSound = invaderDie;
        if (main_bus->o_port_5 & 0b00000001) currentSound = fleetMovement1;
        if (main_bus->o_port_5 & 0b00000010) currentSound = fleetMovement2;
        if (main_bus->o_port_5 & 0b00000100) currentSound = fleetMovement3;
        if (main_bus->o_port_5 & 0b00001000) currentSound = fleetMovement4;
        if (main_bus->o_port_5 & 0b00010000) currentSound = ufoHit;

        if (state != AL_PLAYING && currentSound != -1 && (lastSound != currentSound || repeat)) {
            alSourcei(source, AL_BUFFER, currentSound);
            alSourcePlay(source);
            alGetSourcei(source, AL_SOURCE_STATE, &state);
            lastSound = currentSound;
        }
        if (state == AL_PLAYING) {
            alGetSourcei(source, AL_SOURCE_STATE, &state);
        }
    }
}
