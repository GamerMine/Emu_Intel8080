//
// Created by maxime on 27/07/23.
//

#include <QDebug>
#include "intel_8080.h"

intel_8080::intel_8080(class bus *bus) {
    this->main_bus = bus;

    A = 0x00;
    B = 0x00;
    C = 0x00;
    D = 0x00;
    E = 0x00;
    H = 0x00;
    L = 0x00;

    SP = 0x0000;
    PC = 0x0000;

    hlt = false;
    enable_interrupts = false;
    interrupt = 0x00;
}

void intel_8080::run() {
    while (!hlt) {
        opcodes[fetchInstruction()]();
        checkInterrupts();
    }
}

uint8_t intel_8080::fetchInstruction() {
    if (main_bus->debug_key) {
        qDebug("INSTR: PC = %X\tINSTR = %s", PC, lookup[main_bus->fetchInstruction(PC)].c_str());
    }
    return main_bus->fetchInstruction(PC++);
}

uint8_t intel_8080::fetchData(uint16_t address) {
    if (main_bus->debug_key) {
        qDebug("FETCH: ADDR = %X\tDATA = %X", address, main_bus->fetchData(address));
    }
    return main_bus->fetchData(address);
}

void intel_8080::writeData(uint16_t address, uint8_t data) {
    if (main_bus->debug_key) {
        qDebug("WRITE: ADDR = %X\tDATA = %X", address, data);
    }
    main_bus->writeData(address, data);
}

void intel_8080::reset() {
    A = 0x00;
    B = 0x00;
    C = 0x00;
    D = 0x00;
    E = 0x00;
    H = 0x00;
    L = 0x00;

    SP = 0x0000;
    PC = 0x0000;

    hlt = false;
    enable_interrupts = false;
    interrupt = 0x00;
}

void intel_8080::setInterrupt(uint16_t value) {
    if (enable_interrupts) {
        interrupt = value;
    }
}

void intel_8080::checkInterrupts() {
    if (enable_interrupts && interrupt != 0x0000) {
        if (interrupt == 0x0008) {
            opcodes[0xCF]();
        } else if (interrupt == 0x0010) {
            opcodes[0xD7]();
        }
        interrupt = 0x00;
        //opcodes[0xF3]();
    }
}

void intel_8080::pushToShiftRegister(uint8_t data) {
    main_bus->pushToShiftRegister(data);
}

void intel_8080::setShiftRegisterOffset(uint8_t data) {
    main_bus->setShiftRegisterOffset(data);
}

uint8_t intel_8080::getShiftRegisterContent() {
    return main_bus->getShiftRegisterContent();
}

uint8_t intel_8080::NIMP() {
    qDebug("Not implemented");
    return 1;
}

uint8_t intel_8080::NOP() {
    return 1;
}

uint8_t intel_8080::LXI(uint8_t *rh, uint8_t *rl) {
    if (rh == nullptr && rl == nullptr) {
        SP = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        *rh = fetchData(PC + 1);
        *rl = fetchData(PC);
    }
    PC += 2;
    return 3;
}

uint8_t intel_8080::STAX(uint8_t rh, uint8_t rl) {
    writeData(rh << 8 | rl, A);
    return 2;
}

uint8_t intel_8080::INX(uint8_t *rh, uint8_t *rl) {
    if (rh == nullptr && rl == nullptr) {
        SP++;
    } else {
        uint16_t pair = *rh << 8 | *rl;
        pair++;

        *rh = pair >> 8;
        *rl = pair & 0x00FF;
    }

    return 1;
}

uint8_t intel_8080::INR(uint8_t *reg) {
    uint8_t cycle = 1;
    uint8_t tmp_result;
    if (reg == nullptr) {
        uint8_t data = fetchData(H << 8 | L);

        tmp_result = data + 1;

        flags.aux_carry = ((data & 0x0F) + 1) > 0x0F;

        writeData(H << 8 | L, tmp_result);

        cycle = 3;
    } else {
        tmp_result = *reg + 1;

        flags.aux_carry = ((*reg & 0x0F) + 1) > 0x0F;

        *reg = tmp_result;
    }

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;

    return cycle;
}

uint8_t intel_8080::DCR(uint8_t *reg) {
    uint8_t cycle = 1;
    uint8_t tmp_result;
    if (reg == nullptr) {
        uint8_t data = fetchData(H << 8 | L);

        tmp_result = data - 1;

        flags.aux_carry = ((data & 0x0F) - 1) > 0xFF;

        writeData(H << 8 | L, tmp_result);

        cycle = 3;
    } else {
        tmp_result = *reg - 1;

        flags.aux_carry = ((*reg & 0x0F) - 1) > 0xFF;

        *reg = *reg - 1;
    }

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;

    return cycle;
}

uint8_t intel_8080::MVI(uint8_t *reg) {
    if (reg == nullptr) {
        writeData((H << 8 | L), fetchData(PC++));
        return 3;
    }
    *reg = fetchData(PC++);
    return 2;
}

uint8_t intel_8080::RLC() {
    bool high_bit = A >> 7;

    flags.carry = high_bit;

    A = (A << 1) + high_bit;

    return 1;
}

uint8_t intel_8080::DAD(const uint8_t *rh, const uint8_t *rl) {
    uint16_t reg;
    if (rh == nullptr && rl == nullptr) {
        reg = SP;
    } else {
        reg = *rh << 8 | *rl;
    }
    uint16_t hl = H << 8 | L;

    flags.carry = (hl + reg) > 0xFFFF;

    hl = hl + reg;

    H = hl >> 8;
    L = hl & 0x00FF;

    return 3;
}

uint8_t intel_8080::LDAX(uint8_t rh, uint8_t rl) {
    A = fetchData(rh << 8 | rl);
    return 2;
}

uint8_t intel_8080::LDA() {
    A = fetchData(fetchData(PC + 1) << 8 | fetchData(PC));
    PC += 2;
    return 4;
}

uint8_t intel_8080::DCX(uint8_t *rh, uint8_t *rl) {
    if (rh == nullptr && rl == nullptr) {
        SP--;
    } else {
        uint16_t pair = *rh << 8 | *rl;
        pair--;

        *rh = pair >> 8;
        *rl = pair & 0x00FF;
    }

    return 1;
}

uint8_t intel_8080::RRC() {
    bool low_bit = A & 0x01;

    flags.carry = low_bit;

    A = (A >> 1) | low_bit << 7;

    return 1;
}

uint8_t intel_8080::RAL() {
    bool high_bit = A >> 7;

    A = (A << 1) + flags.carry;

    flags.carry = high_bit;

    return 1;
}

uint8_t intel_8080::RAR() {
    bool low_bit = A & 0x01;

    A = (A >> 1) + (flags.carry == 1 ? 0x80 : 0x00);

    flags.carry = low_bit;

    return 1;
}

uint8_t intel_8080::SHLD() {
    uint16_t address = fetchData(PC + 1) << 8 | fetchData(PC);
    writeData(address, L);
    writeData(address + 1, H);
    PC += 2;
    return 5;
}

uint8_t intel_8080::DAA() {
    bool tmp_carry = flags.carry;
    if ((A & 0x0F) > 9 || flags.aux_carry == 1) {
        flags.carry     = flags.carry || (A + 6) > 0xFF;
        flags.aux_carry = 1;
        A += 0x06;
    } else {
        flags.aux_carry = 0;
    }
    if ((A >> 4) > 9 || tmp_carry == 1) {
        A += 0x60;
        flags.carry     = 1;
    } else {
        flags.carry = 0;
    }

    flags.zero      = A == 0x00;
    flags.sign      = (A >> 7) == 1;
    flags.parity    = (A % 2) == 0;

    return 1;
}

uint8_t intel_8080::LHLD() {
    uint16_t address = fetchData(PC + 1) << 8 | fetchData(PC);
    L = fetchData(address);
    H = fetchData(address + 1);
    PC += 2;
    return 5;
}

uint8_t intel_8080::CMA() {
    A = ~A;

    return 1;
}

uint8_t intel_8080::STA() {
    writeData(fetchData(PC + 1) << 8 | fetchData(PC), A);
    PC += 2;
    return 4;
}

uint8_t intel_8080::STC() {
    flags.carry = 1;

    return 1;
}

uint8_t intel_8080::CMC() {
    flags.carry = ~flags.carry;

    return 1;
}

uint8_t intel_8080::MOV(uint8_t *reg, const uint8_t *reg2) {
    if (reg2 == nullptr) {
        *reg = fetchData((H << 8 | L));
        return 2;
    } else if (reg == nullptr) {
        writeData((H << 8 | L), *reg2);
        return 2;
    }
    *reg = *reg2;
    return 1;
}

uint8_t intel_8080::HLT() {
    hlt = true;
    qWarning("The processor has been halted");

    return 1;
}

uint8_t intel_8080::ADD(const uint8_t *reg) {
    uint8_t cycle = 1;
    uint8_t data;
    if (reg == nullptr) {
        data = fetchData(H << 8 | L);
        cycle = 2;
    } else {
        data = *reg;
    }
    uint8_t tmp_result = A + data;

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;
    flags.carry     = (A + data) > 0xFF;
    flags.aux_carry = ((A & 0x0F) + (data & 0x0F)) > 0x0F;

    A = tmp_result;

    return cycle;
}

uint8_t intel_8080::ADC(const uint8_t *reg) {
    uint8_t cycle = 1;
    uint8_t data;
    if (reg == nullptr) {
        data = fetchData(H << 8 | L);
        cycle = 2;
    } else {
        data = *reg;
    }
    uint8_t tmp_result = A + data + flags.carry;

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;
    flags.carry     = (A + data + flags.carry) > 0xFF;
    flags.aux_carry = ((A & 0x0F) + (data & 0x0F) + flags.carry) > 0x0F;

    A = tmp_result;

    return cycle;
}

uint8_t intel_8080::SUB(const uint8_t *reg) {
    uint8_t cycle = 1;
    uint8_t data;
    if (reg == nullptr) {
        data = fetchData(H << 8 | L);
        cycle = 2;
    } else {
        data = *reg;
    }
    uint8_t tmp_result = A - data;

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;
    flags.carry     = (A - data) > 0xFF;
    flags.aux_carry = ((A & 0x0F) - (data & 0x0F)) > 0xFF;

    A = tmp_result;

    return cycle;
}

uint8_t intel_8080::SBB(const uint8_t *reg) {
    uint8_t cycle = 1;
    uint8_t data;
    if (reg == nullptr) {
        data = fetchData(H << 8 | L);
        cycle = 2;
    } else {
        data = *reg;
    }
    uint8_t tmp_result = A - data - flags.carry;

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;
    flags.carry     = (A - data - flags.carry) > 0xFF;
    flags.aux_carry = ((A & 0x0F) - (data & 0x0F) - flags.carry) > 0xFF;

    A = tmp_result;

    return cycle;
}

uint8_t intel_8080::ANA(const uint8_t *reg) {
    uint8_t cycle = 1;
    uint8_t data;
    if (reg == nullptr) {
        data = fetchData(H << 8 | L);
        cycle = 2;
    } else {
        data = *reg;
    }
    uint8_t tmp_result = A & data;

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;
    flags.carry     = 0;
    flags.aux_carry = 0;

    A = tmp_result;

    return cycle;
}

uint8_t intel_8080::XRA(const uint8_t *reg) {
    uint8_t cycle = 1;
    uint8_t data;
    if (reg == nullptr) {
        data = fetchData(H << 8 | L);
        cycle = 2;
    } else {
        data = *reg;
    }
    uint8_t tmp_result = A ^ data;

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;
    flags.carry     = 0;
    flags.aux_carry = 0;

    A = tmp_result;

    return cycle;
}

uint8_t intel_8080::ORA(const uint8_t *reg) {
    uint8_t cycle = 1;
    uint8_t data;
    if (reg == nullptr) {
        data = fetchData(H << 8 | L);
        cycle = 2;
    } else {
        data = *reg;
    }
    uint8_t tmp_result = A | data;

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;
    flags.carry     = 0;
    flags.aux_carry = 0;

    A = tmp_result;

    return cycle;
}

uint8_t intel_8080::CMP(const uint8_t *reg) {
    uint8_t cycle = 1;
    uint8_t data;
    if (reg == nullptr) {
        data = fetchData(H << 8 | L);
        cycle = 2;
    } else {
        data = *reg;
    }
    uint8_t result = A - data;

    flags.zero      = A == data;
    flags.sign      = (result >> 7) == 1;
    flags.parity    = (result % 2) == 0;
    flags.carry     = A < data;
    flags.aux_carry = ((A & 0x0F) - (data & 0x0F)) > 0xFF;

    return cycle;
}

uint8_t intel_8080::RNZ() {
    if (flags.zero == 0) {
        PC = fetchData(SP + 1) << 8 | fetchData(SP);
        SP +=2;
        return 3;
    }

    return 1;
}

uint8_t intel_8080::POP(uint8_t *rh, uint8_t *rl) {
    if (rh == nullptr && rl == nullptr) {
        uint8_t status_word = fetchData(SP);
        flags.carry = status_word & 0b00000001;
        flags.parity = status_word & 0b00000100;
        flags.aux_carry = status_word & 0b00010000;
        flags.zero = status_word & 0b01000000;
        flags.sign = status_word & 0b10000000;
        A = fetchData(SP + 1);
    } else {
        *rh = fetchData(SP + 1);
        *rl = fetchData(SP);
    }

    SP += 2;

    return 3;
}

uint8_t intel_8080::JNZ() {
    if (flags.zero == 0) {
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        PC += 2;
    }

    return 3;
}

uint8_t intel_8080::JMP() {
    PC = fetchData(PC + 1) << 8 | fetchData(PC);

    return 3;
}

uint8_t intel_8080::CNZ() {
    uint8_t cycle = 0;
    if (flags.zero == 0) {
        writeData(SP - 1, (PC + 2) >> 8);
        writeData(SP - 2, (PC + 2) & 0x00FF);

        SP -= 2;
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
        cycle = 2;
    } else {
        PC += 2;
    }

    return 5 + cycle;
}

uint8_t intel_8080::PUSH(const uint8_t *rh, const uint8_t *rl) {
    if (rh == nullptr && rl == nullptr) {
        uint8_t status_word = flags.sign << 7 | flags.zero << 6 | flags.aux_carry << 4 | flags.parity << 2 | 1 << 1 | flags.carry;
        writeData(SP - 1, A);
        writeData(SP - 2, status_word);
    } else {
        writeData(SP - 1, *rh);
        writeData(SP - 2, *rl);
    }

    SP -= 2;

    return 3;
}

uint8_t intel_8080::ADI() {
    uint8_t data = fetchData(PC++);
    uint8_t tmp_result = A + data;

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;
    flags.carry     = (A + data) > 0xFF;
    flags.aux_carry = ((A & 0x0F) + (data & 0x0F)) > 0xF;

    A = tmp_result;

    return 2;
}

uint8_t intel_8080::RST(uint16_t addr) {
    writeData(SP - 1, PC >> 8);
    writeData(SP - 2, PC & 0x00FF);

    SP -= 2;

    PC = addr;

    return 3;
}

uint8_t intel_8080::RZ() {
    if (flags.zero == 1) {
        PC = fetchData(SP + 1) << 8 | fetchData(SP);
        SP +=2;
        return 3;
    }

    return 1;
}

uint8_t intel_8080::RET() {
    PC = fetchData(SP + 1) << 8 | fetchData(SP);
    SP +=2;

    return 3;
}

uint8_t intel_8080::JZ() {
    if (flags.zero == 1) {
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        PC += 2;
    }

    return 3;
}

uint8_t intel_8080::CZ() {
    uint8_t cycle = 0;
    if (flags.zero == 1) {
        writeData(SP - 1, (PC + 2) >> 8);
        writeData(SP - 2, (PC + 2) & 0x00FF);

        SP -= 2;
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
        cycle = 2;
    } else {
        PC += 2;
    }

    return 3 + cycle;
}

uint8_t intel_8080::CALL() {
    writeData(SP - 1, (PC + 2) >> 8);
    writeData(SP - 2, (PC + 2) & 0x00FF);

    SP -= 2;
    PC = fetchData(PC + 1) << 8 | fetchData(PC);

    return 5;
}

uint8_t intel_8080::ACI() {
    uint8_t data = fetchData(PC++);
    uint8_t tmp_result = A + data + flags.carry;

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;
    flags.carry     = (A + data + flags.carry) > 0xFF;
    flags.aux_carry = ((A & 0x0F) + (data & 0x0F) + flags.carry) > 0xF;

    A = tmp_result;

    return 2;
}

uint8_t intel_8080::RNC() {
    if (flags.carry == 0) {
        PC = fetchData(SP + 1) << 8 | fetchData(SP);
        SP +=2;
        return 3;
    }

    return 1;
}

uint8_t intel_8080::JNC() {
    if (flags.carry == 0) {
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        PC += 2;
    }

    return 3;
}

uint8_t intel_8080::OUT() {
    uint8_t port = fetchData(PC++);
    if (port == 2) {
        setShiftRegisterOffset(A);
    }
    if (port == 3) {
        /*if (A & 0x01) emit main_bus->speak->pUFO();
        if (A & 0x02) emit main_bus->speak->pSHOT();
        if (A & 0x04) emit main_bus->speak->pFLASH();
        if (A & 0x08) emit main_bus->speak->pINVADER_DIE();*/
    }
    if (port == 4) {
        pushToShiftRegister(A);
    }
    if (port == 5) {
        /*if (A & 0x01) emit main_bus->speak->pFLEET_MOVEMENT_1();
        if (A & 0x02) emit main_bus->speak->pFLEET_MOVEMENT_2();
        if (A & 0x04) emit main_bus->speak->pFLEET_MOVEMENT_3();
        if (A & 0x08) emit main_bus->speak->pFLEET_MOVEMENT_4();
        if (A & 0x10) emit main_bus->speak->pUFO_HIT();*/
    }
    return 3;
}

uint8_t intel_8080::CNC() {
    uint8_t cycle = 0;
    if (flags.carry == 0) {
        writeData(SP - 1, (PC + 2) >> 8);
        writeData(SP - 2, (PC + 2) & 0x00FF);

        SP -= 2;
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
        cycle = 2;
    } else {
        PC += 2;
    }

    return 5 + cycle;
}

uint8_t intel_8080::SUI() {
    uint8_t data = fetchData(PC++);
    uint8_t tmp_result = A - data;

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;
    flags.carry     = (A - data) > 0xFF;
    flags.aux_carry = ((A & 0x0F) - (data & 0x0F)) > 0xFF;

    A = tmp_result;

    return 2;
}

uint8_t intel_8080::RC() {
    if (flags.carry == 1) {
        PC = fetchData(SP + 1) << 8 | fetchData(SP);
        SP +=2;
        return 3;
    }

    return 1;
}

uint8_t intel_8080::JC() {
    if (flags.carry == 1) {
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        PC += 2;
    }

    return 3;
}

uint8_t intel_8080::IN() {
    uint8_t port = fetchData(PC++);
    if (port == 0) {
        A = 0b00001110;
    }
    if (port == 1) {
        A = main_bus->i_port_1;
        if (main_bus->i_port_1 & 0x1) {
            main_bus->i_port_1 = main_bus->i_port_1 & 0xFE;
        }
    }
    if (port == 2) {
        A = 0b00000000;
    }
    if (port == 3) {
        A = getShiftRegisterContent();
    }
    if (port == 6) {
        reset();
    }
    return 3;
}

uint8_t intel_8080::CC() {
    uint8_t cycle = 0;
    if (flags.carry == 1) {
        writeData(SP - 1, (PC + 2) >> 8);
        writeData(SP - 2, (PC + 2) & 0x00FF);

        SP -= 2;
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
        cycle = 2;
    } else {
        PC += 2;
    }

    return 5 + cycle;
}

uint8_t intel_8080::SBI() {
    uint8_t data = fetchData(PC++);
    uint8_t tmp_result = A - data - flags.carry;

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;
    flags.carry     = (A - data - flags.carry) > 0xFF;
    flags.aux_carry = ((A & 0x0F) - (data & 0x0F) - flags.carry) > 0xFF;

    A = tmp_result;

    return 2;
}

uint8_t intel_8080::RPO() {
    if (flags.parity == 0) {
        PC = fetchData(SP + 1) << 8 | fetchData(SP);
        SP +=2;
        return 3;
    }

    return 1;
}

uint8_t intel_8080::JPO() {
    if (flags.parity == 0) {
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        PC += 2;
    }

    return 3;
}

uint8_t intel_8080::XTHL() {
    uint8_t l_data = fetchData(SP);
    uint8_t h_data = fetchData(SP + 1);

    writeData(SP, L);
    writeData(SP + 1, H);

    L = l_data;
    H = h_data;

    return 5;
}

uint8_t intel_8080::CPO() {
    uint8_t cycle = 0;
    if (flags.parity == 0) {
        writeData(SP - 1, (PC + 2) >> 8);
        writeData(SP - 2, (PC + 2) & 0x00FF);

        SP -= 2;
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
        cycle = 2;
    } else {
        PC += 2;
    }

    return 5 + cycle;
}

uint8_t intel_8080::ANI() {
    uint8_t tmp_result = A & fetchData(PC++);

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;
    flags.carry     = 0;
    flags.aux_carry = 0;

    A = tmp_result;

    return 2;
}

uint8_t intel_8080::RPE() {
    if (flags.parity == 1) {
        PC = fetchData(SP + 1) << 8 | fetchData(SP);
        SP +=2;
        return 3;
    }

    return 1;
}

uint8_t intel_8080::PCHL() {
    PC = H << 8 | L;

    return 1;
}

uint8_t intel_8080::JPE() {
    if (flags.parity == 1) {
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        PC += 2;
    }

    return 3;
}

uint8_t intel_8080::XCHG() {
    uint8_t tmp_D = D;
    uint8_t tmp_E = E;

    D = H;
    E = L;
    H = tmp_D;
    L = tmp_E;

    return 1;
}

uint8_t intel_8080::CPE() {
    uint8_t cycle = 0;
    if (flags.parity == 1) {
        writeData(SP - 1, (PC + 2) >> 8);
        writeData(SP - 2, (PC + 2) & 0x00FF);

        SP -= 2;
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
        cycle = 2;
    } else {
        PC += 2;
    }

    return 5 + cycle;
}

uint8_t intel_8080::XRI() {
    uint8_t tmp_result = A ^ fetchData(PC++);

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;
    flags.carry     = 0;
    flags.aux_carry = 0;

    A = tmp_result;

    return 1;
}

uint8_t intel_8080::RP() {
    if (flags.sign == 0) {
        PC = fetchData(SP + 1) << 8 | fetchData(SP);
        SP +=2;
        return 3;
    }

    return 1;
}

uint8_t intel_8080::JP() {
    if (flags.sign == 0) {
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        PC += 2;
    }

    return 3;
}

uint8_t intel_8080::DI() {
    enable_interrupts = false;
    return 1;
}

uint8_t intel_8080::CP() {
    uint8_t cycle = 0;
    if (flags.sign == 0) {
        writeData(SP - 1, (PC + 2) >> 8);
        writeData(SP - 2, (PC + 2) & 0x00FF);

        SP -= 2;
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
        cycle = 2;
    } else {
        PC += 2;
    }

    return 5 + cycle;
}

uint8_t intel_8080::ORI() {
    uint8_t tmp_result = A | fetchData(PC++);

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;
    flags.carry     = 0;
    flags.aux_carry = 0;

    A = tmp_result;

    return 1;
}

uint8_t intel_8080::RM() {
    if (flags.sign == 1) {
        PC = fetchData(SP + 1) << 8 | fetchData(SP);
        SP +=2;
        return 3;
    }

    return 1;
}

uint8_t intel_8080::SPHL() {
    SP = H << 8 | L;

    return 1;
}

uint8_t intel_8080::JM() {
    if (flags.sign == 1) {
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        PC += 2;
    }

    return 3;
}

uint8_t intel_8080::EI() {
    enable_interrupts = true;
    return 1;
}

uint8_t intel_8080::CM() {
    uint8_t cycle = 0;
    if (flags.sign == 1) {
        writeData(SP - 1, (PC + 2) >> 8);
        writeData(SP - 2, (PC + 2) & 0x00FF);

        SP -= 2;
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
        cycle = 2;
    } else {
        PC += 2;
    }

    return 5 + cycle;
}

uint8_t intel_8080::CPI() {
    uint8_t data = fetchData(PC++);
    uint8_t result = A - data;

    flags.zero      = A == data;
    flags.sign      = (result >> 7) == 1;
    flags.parity    = (result % 2) == 0;
    flags.carry     = A < data;
    flags.aux_carry = ((A & 0x0F) - (data & 0x0F)) > 0xFF;

    return 2;
}

intel_8080::~intel_8080() = default;
