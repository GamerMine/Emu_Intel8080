//
// Created by maxime on 27/07/23.
//

#ifndef EMU_INTEL8080_BUS_H
#define EMU_INTEL8080_BUS_H
#define MEMORY_SIZE 0x4000

#include <cstdint>
#include <QImage>

#include "Intel8080.h"
#include "Speaker.h"

class Intel8080;
class Bus : public QObject {
    Q_OBJECT

public:
    Bus();

    uint8_t fetchInstruction(uint16_t address);
    uint8_t fetchData(uint16_t address);
    void writeData(uint16_t address, uint8_t data);

    uint8_t mem[MEMORY_SIZE]{};
    Intel8080 *cpu;
    class Speaker *speak;

    // Shift register
    void pushToShiftRegister(uint8_t data);
    void setShiftRegisterOffset(uint8_t data);
    uint8_t getShiftRegisterContent();

    // Input ports
    uint8_t i_port_1;
    uint8_t i_port_2; //TODO: Second player not working
    bool debug_key;

    // Output ports
    uint8_t o_port_3;
    uint8_t o_port_5;

private:
    uint16_t shift_register;
    uint8_t shift_offset;
};


#endif //EMU_INTEL8080_BUS_H
