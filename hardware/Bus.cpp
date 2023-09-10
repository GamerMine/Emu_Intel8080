//
// Created by maxime on 27/07/23.
//

#include "Bus.h"
#include "Screen.h"
#include "ScreenAdapter.h"

const char *roms[4] = {"resources/roms/invaders.h", "resources/roms/invaders.g", "resources/roms/invaders.f", "resources/roms/invaders.e"};

ScreenAdapter *adapter;

Bus::Bus() {
    cpu = new Intel8080(this);
    adapter = new ScreenAdapter(this);
    speak = new Speaker(this);

    // ROM
    uint16_t i = 0;
    for (const char *rom : roms) {
        std::ifstream file(rom, std::ios::binary);

        if (!file.good()) {
            qCritical("File not found: %s", rom);
            return;
        }

        char byte;

        while (!file.eof()) {
            file.read(&byte, 1);
            mem[i++] = byte;
        }
        i--;

        file.close();
    }

    // Shift register
    shift_register = 0x0000;
    shift_offset = 0x00;

    // I/O
    i_port_1 = 0x00;
    i_port_2 = 0x00;
    o_port_3 = 0x00;
    o_port_5 = 0x00;
    debug_key = 0;

    adapter->start();
    speak->start();
    cpu->start();

    QObject::connect(adapter, &ScreenAdapter::setInterrupt, cpu, &Intel8080::setInterrupt);
}

uint8_t Bus::fetchInstruction(uint16_t address) {
    return mem[address];
}

uint8_t Bus::fetchData(uint16_t address) {
    return mem[address];
}

void Bus::writeData(uint16_t address, uint8_t data) {
    if (address >= 0x2000 && address <= 0x3FFF) {
        mem[address] = data;
    }
}

void Bus::pushToShiftRegister(uint8_t data) {
    shift_register = shift_register >> 8;
    shift_register = shift_register | (data << 8);
}

void Bus::setShiftRegisterOffset(uint8_t data) {
    shift_offset = data;
}

uint8_t Bus::getShiftRegisterContent() {
    uint16_t result = (shift_register << shift_offset) >> (8 - shift_offset);

    return result & 0x00FF;
}

void Bus::reset() {
    uint16_t i = 0x2000;
    while (i < 0x4000) {
        mem[i++] = 0x00;
    }

    cpu->reset();
}
