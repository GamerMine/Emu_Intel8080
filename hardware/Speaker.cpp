//
// Created by maxime on 16/08/23.
//

#include "Speaker.h"

Speaker::Speaker(Bus *bus) {
    main_bus = bus;

    // Initialize the device (choosing the default device)
    ALCdevice *device = alcOpenDevice(nullptr);
    if (!device) qCritical("ERROR: Could not open audio device");

    // Creating a context a making it current
    ALCcontext *context = alcCreateContext(device, nullptr);
    if (!context) qCritical("ERROR: Could not create audio context");
    alcMakeContextCurrent(context);

    // Generating the sources
    alGenSources(3, &source[0]);
    for (unsigned int srcNb : source) {
        alSourcef(srcNb, AL_GAIN, 0.1f);
    }
}

[[noreturn]] void Speaker::run() {
    // Loading all the sound effect (Note that it may be slow depending on the computer it is running on)
    // Considering that the time before launching a game is enough
    ALuint flash = WaveFileLoader::getBufferForFile("sounds/flash.wav");
    ALuint fleetMovement1 = WaveFileLoader::getBufferForFile("sounds/fleet_movement_1.wav");
    ALuint fleetMovement2 = WaveFileLoader::getBufferForFile("sounds/fleet_movement_2.wav");
    ALuint fleetMovement3 = WaveFileLoader::getBufferForFile("sounds/fleet_movement_3.wav");
    ALuint fleetMovement4 = WaveFileLoader::getBufferForFile("sounds/fleet_movement_4.wav");
    ALuint invaderDie = WaveFileLoader::getBufferForFile("sounds/invader_die.wav");
    ALuint shot = WaveFileLoader::getBufferForFile("sounds/shot.wav");
    ALuint ufo = WaveFileLoader::getBufferForFile("sounds/ufo.wav");
    ALuint ufoHit = WaveFileLoader::getBufferForFile("sounds/ufo_hit.wav");

    // Initializing some useful variables to manage the sound effects
    ALint state[3] = {AL_STOPPED, AL_STOPPED, AL_STOPPED};
    ALint currentSound[3] = {-1, -1, -1};
    ALint lastSound[3] = {-1, -1, -1};
    ALboolean repeat[3] = {AL_FALSE, AL_FALSE, AL_FALSE};

    while (true) {
        // Reinitializing some variables to default values
        for (unsigned int srcNb : source) {
            currentSound[srcNb] = -1;
            repeat[srcNb] = AL_FALSE;
        }

        // Checking what sounds should be played
        if (main_bus->o_port_3 & 0b00000001) {currentSound[2] = (ALint)ufo; repeat[2] = AL_TRUE;}
        if (main_bus->o_port_3 & 0b00000010) {currentSound[0] = (ALint)shot;}
        if (main_bus->o_port_3 & 0b00000100) {currentSound[0] = (ALint)flash;}
        if (main_bus->o_port_3 & 0b00001000) {currentSound[0] = (ALint)invaderDie;}
        if (main_bus->o_port_5 & 0b00000001) {currentSound[1] = (ALint)fleetMovement1;}
        if (main_bus->o_port_5 & 0b00000010) {currentSound[1] = (ALint)fleetMovement2;}
        if (main_bus->o_port_5 & 0b00000100) {currentSound[1] = (ALint)fleetMovement3;}
        if (main_bus->o_port_5 & 0b00001000) {currentSound[1] = (ALint)fleetMovement4;}
        if (main_bus->o_port_5 & 0b00010000) {currentSound[1] = (ALint)ufoHit;}

        // Playing the sounds (+ some checks to make the sound behave normally)
        for (uint8_t outputPort = 0; outputPort < 3; outputPort++) {
            if (state[outputPort] != AL_PLAYING && currentSound[outputPort] != -1 &&
                (lastSound[outputPort] != currentSound[outputPort] || repeat[outputPort])) {
                alSourcei(source[outputPort], AL_BUFFER, currentSound[outputPort]);
                alSourcePlay(source[outputPort]);
                alGetSourcei(source[outputPort], AL_SOURCE_STATE, &state[outputPort]);
                lastSound[outputPort] = currentSound[outputPort];
            }

            // Setting the states of the sources
            if (state[outputPort] == AL_PLAYING) {
                alGetSourcei(source[outputPort], AL_SOURCE_STATE, &state[outputPort]);
            }
        }
        if (!(main_bus->o_port_3 & 0b00000010)) {lastSound[0] = -1;}
    }
}
