//
// Created by maxime on 27/07/23.
//

#include "bus.h"
#include "screen.h"
#include "screen_adapter.h"

const char *roms[4] = {"roms/invaders.h", "roms/invaders.g", "roms/invaders.f", "roms/invaders.e"};

screen_adapter *adapter;

bus::bus() {
    cpu = new intel_8080(this);
    adapter = new screen_adapter(this);
    speak = new speaker(this);

    // ROM
    uint16_t i = 0;
    for (const char *rom : roms) {
        FILE *file = fopen(rom, "r");

        if (file == nullptr) {
            qCritical("File not found: %s", rom);
            return;
        }

        int byte;
        while((byte = getc(file)) != EOF) {
            mem[i++] = (uint8_t) byte;
        }

        fclose(file);
    }

    // Shift register
    shift_register = 0x0000;
    shift_offset = 0x00;

    // I/O
    i_port_1 = 0x00;
    i_port_2 = 0x00;
    debug_key = 0;

    adapter->start();
    speak->start();
    cpu->start();

    QObject::connect(adapter, &screen_adapter::setInterrupt, cpu, &intel_8080::setInterrupt);
}

uint8_t bus::fetchInstruction(uint16_t address) {
    return mem[address];
}

uint8_t bus::fetchData(uint16_t address) {
    return mem[address];
}

void bus::writeData(uint16_t address, uint8_t data) {
    if (address >= 0x2000 && address <= 0x3FFF) {
        mem[address] = data;
    }
}

void bus::pushToShiftRegister(uint8_t data) {
    shift_register = shift_register >> 8;
    shift_register = shift_register | (data << 8);
}

void bus::setShiftRegisterOffset(uint8_t data) {
    shift_offset = data;
}

uint8_t bus::getShiftRegisterContent() {
    uint16_t result = (shift_register << shift_offset) >> (8 - shift_offset);

    return result & 0x00FF;
}
