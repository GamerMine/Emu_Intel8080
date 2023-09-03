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

    alGenSources(2, &source[0]);
    alSourcef(source[0], AL_GAIN, 0.1f);
    alSourcef(source[1], AL_GAIN, 0.1f);
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

    ALint state[2] = {AL_STOPPED, AL_STOPPED};
    ALint currentSound[2] = {-1, -1};
    ALint lastSound[2] = {-1, -1};
    uint8_t outputPort = 0;
    ALboolean repeat[2] = {AL_FALSE, AL_FALSE};

    while (true) {
        currentSound[0] = -1;
        currentSound[1] = -1;
        repeat[0] = AL_FALSE;
        repeat[1] = AL_FALSE;
        if (main_bus->o_port_3 & 0b00000001) {
            currentSound[0] = ufo;
            repeat[0] = AL_TRUE;
        }
        if (main_bus->o_port_3 & 0b00000010) {currentSound[0] = shot;outputPort = 0;}
        if (main_bus->o_port_3 & 0b00000100) {currentSound[0] = flash;outputPort = 0;}
        if (main_bus->o_port_3 & 0b00001000) {currentSound[0] = invaderDie;outputPort = 0;}
        if (main_bus->o_port_5 & 0b00000001) {currentSound[1] = fleetMovement1;outputPort = 1;}
        if (main_bus->o_port_5 & 0b00000010) {currentSound[1] = fleetMovement2;outputPort = 1;}
        if (main_bus->o_port_5 & 0b00000100) {currentSound[1] = fleetMovement3;outputPort = 1;}
        if (main_bus->o_port_5 & 0b00001000) {currentSound[1] = fleetMovement4;outputPort = 1;}
        if (main_bus->o_port_5 & 0b00010000) {currentSound[1] = ufoHit;outputPort = 1;}

        if (state[outputPort] != AL_PLAYING && currentSound[outputPort] != -1 && (lastSound[outputPort] != currentSound[outputPort] || repeat[outputPort])) {
            alSourcei(source[outputPort], AL_BUFFER, currentSound[outputPort]);
            alSourcePlay(source[outputPort]);
            alGetSourcei(source[outputPort], AL_SOURCE_STATE, &state[outputPort]);
            lastSound[outputPort] = currentSound[outputPort];
        }
        if (state[outputPort] == AL_PLAYING) {
            alGetSourcei(source[outputPort], AL_SOURCE_STATE, &state[outputPort]);
        }
    }
}
