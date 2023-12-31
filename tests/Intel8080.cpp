//
// Created by maxime on 27/07/23.
//

#include <QDebug>
#include "Intel8080.h"

Intel8080::Intel8080() {
    this->ram = {};

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
}

void Intel8080::loadRAM(uint8_t *new_ram) {
    this->ram = new_ram;
}

uint8_t Intel8080::fetchData(uint16_t address) {
    return ram[address];
}

void Intel8080::writeData(uint16_t address, uint8_t data) {
    ram[address] = data;
}

void Intel8080::reset() {
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
}

uint8_t Intel8080::NIMP() {
    qDebug("Not implemented");
    return 1;
}

uint8_t Intel8080::NOP() {
    return 1;
}

uint8_t Intel8080::LXI(uint8_t *rh, uint8_t *rl) {
    if (rh == nullptr && rl == nullptr) {
        SP = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        *rh = fetchData(PC + 1);
        *rl = fetchData(PC);
    }
    PC += 2;
    return 3;
}

uint8_t Intel8080::STAX(uint8_t rh, uint8_t rl) {
    writeData(rh << 8 | rl, A);
    return 2;
}

uint8_t Intel8080::INX(uint8_t *rh, uint8_t *rl) {
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

uint8_t Intel8080::INR(uint8_t *reg) {
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

uint8_t Intel8080::DCR(uint8_t *reg) {
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

uint8_t Intel8080::MVI(uint8_t *reg) {
    if (reg == nullptr) {
        writeData((H << 8 | L), fetchData(PC++));
        return 3;
    }
    *reg = fetchData(PC++);
    return 2;
}

uint8_t Intel8080::RLC() {
    bool high_bit = A >> 7;

    flags.carry = high_bit;

    A = (A << 1) + high_bit;

    return 1;
}

uint8_t Intel8080::DAD(const uint8_t *rh, const uint8_t *rl) {
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

uint8_t Intel8080::LDAX(uint8_t rh, uint8_t rl) {
    A = fetchData(rh << 8 | rl);
    return 2;
}

uint8_t Intel8080::LDA() {
    A = fetchData(fetchData(PC + 1) << 8 | fetchData(PC));
    PC += 2;
    return 4;
}

uint8_t Intel8080::DCX(uint8_t *rh, uint8_t *rl) {
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

uint8_t Intel8080::RRC() {
    bool low_bit = A & 0x01;

    flags.carry = low_bit;

    A = (A >> 1) | low_bit << 7;

    return 1;
}

uint8_t Intel8080::RAL() {
    bool high_bit = A >> 7;

    A = (A << 1) + flags.carry;

    flags.carry = high_bit;

    return 1;
}

uint8_t Intel8080::RAR() {
    bool low_bit = A & 0x01;

    A = (A >> 1) + (flags.carry == 1 ? 0x80 : 0x00);

    flags.carry = low_bit;

    return 1;
}

uint8_t Intel8080::SHLD() {
    uint16_t address = fetchData(PC + 1) << 8 | fetchData(PC);
    writeData(address, L);
    writeData(address + 1, H);
    PC += 2;
    return 5;
}

uint8_t Intel8080::DAA() {
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

uint8_t Intel8080::LHLD() {
    uint16_t address = fetchData(PC + 1) << 8 | fetchData(PC);
    L = fetchData(address);
    H = fetchData(address + 1);
    PC += 2;
    return 5;
}

uint8_t Intel8080::CMA() {
    A = ~A;

    return 1;
}

uint8_t Intel8080::STA() {
    writeData(fetchData(PC + 1) << 8 | fetchData(PC), A);
    PC += 2;
    return 4;
}

uint8_t Intel8080::STC() {
    flags.carry = 1;

    return 1;
}

uint8_t Intel8080::CMC() {
    flags.carry = ~flags.carry;

    return 1;
}

uint8_t Intel8080::MOV(uint8_t *reg, const uint8_t *reg2) {
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

uint8_t Intel8080::HLT() {
    hlt = true;
    qWarning("The processor has been halted");

    return 1;
}

uint8_t Intel8080::ADD(const uint8_t *reg) {
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

uint8_t Intel8080::ADC(const uint8_t *reg) {
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

uint8_t Intel8080::SUB(const uint8_t *reg) {
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

uint8_t Intel8080::SBB(const uint8_t *reg) {
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

uint8_t Intel8080::ANA(const uint8_t *reg) {
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

uint8_t Intel8080::XRA(const uint8_t *reg) {
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

uint8_t Intel8080::ORA(const uint8_t *reg) {
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

uint8_t Intel8080::CMP(const uint8_t *reg) {
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

uint8_t Intel8080::RNZ() {
    if (flags.zero == 0) {
        PC = fetchData(SP + 1) << 8 | fetchData(SP);
        SP +=2;
        return 3;
    }

    return 1;
}

uint8_t Intel8080::POP(uint8_t *rh, uint8_t *rl) {
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

uint8_t Intel8080::JNZ() {
    if (flags.zero == 0) {
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        PC += 2;
    }

    return 3;
}

uint8_t Intel8080::JMP() {
    PC = fetchData(PC + 1) << 8 | fetchData(PC);

    return 3;
}

uint8_t Intel8080::CNZ() {
    uint8_t cycle = 0;
    if (flags.zero == 0) {
        writeData(SP - 1, PC >> 8);
        writeData(SP - 2, PC & 0x00FF);

        SP -= 2;
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
        cycle = 2;
    }

    PC += 2;

    return 5 + cycle;
}

uint8_t Intel8080::PUSH(const uint8_t *rh, const uint8_t *rl) {
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

uint8_t Intel8080::ADI() {
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

uint8_t Intel8080::RST(uint16_t addr) {
    writeData(SP - 1, PC >> 8);
    writeData(SP - 2, PC & 0x00FF);

    SP -= 2;

    PC = addr;

    return 3;
}

uint8_t Intel8080::RZ() {
    if (flags.zero == 1) {
        PC = fetchData(SP + 1) << 8 | fetchData(SP);
        SP +=2;
        return 3;
    }

    return 1;
}

uint8_t Intel8080::RET() {
    PC = fetchData(SP + 1) << 8 | fetchData(SP);
    SP +=2;

    return 3;
}

uint8_t Intel8080::JZ() {
    if (flags.zero == 1) {
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        PC += 2;
    }

    return 3;
}

uint8_t Intel8080::CZ() {
    uint8_t cycle = 0;
    if (flags.zero == 1) {
        writeData(SP - 1, PC >> 8);
        writeData(SP - 2, PC & 0x00FF);

        SP -= 2;
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
        cycle = 2;
    }

    PC += 2;

    return 3 + cycle;
}

uint8_t Intel8080::CALL() {
    writeData(SP - 1, (PC + 2) >> 8);
    writeData(SP - 2, (PC + 2) & 0x00FF);

    SP -= 2;
    PC = fetchData(PC + 1) << 8 | fetchData(PC);

    return 5;
}

uint8_t Intel8080::ACI() {
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

uint8_t Intel8080::RNC() {
    if (flags.carry == 0) {
        PC = fetchData(SP + 1) << 8 | fetchData(SP);
        SP +=2;
        return 3;
    }

    return 1;
}

uint8_t Intel8080::JNC() {
    if (flags.carry == 0) {
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        PC += 2;
    }

    return 3;
}

uint8_t Intel8080::OUT() {
    qWarning("OUT");
    //TODO
    PC++;
    return 3;
}

uint8_t Intel8080::CNC() {
    uint8_t cycle = 0;
    if (flags.carry == 0) {
        writeData(SP - 1, PC >> 8);
        writeData(SP - 2, PC & 0x00FF);

        SP -= 2;
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
        cycle = 2;
    }

    PC += 2;

    return 5 + cycle;
}

uint8_t Intel8080::SUI() {
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

uint8_t Intel8080::RC() {
    if (flags.carry == 1) {
        PC = fetchData(SP + 1) << 8 | fetchData(SP);
        SP +=2;
        return 3;
    }

    return 1;
}

uint8_t Intel8080::JC() {
    if (flags.carry == 1) {
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        PC += 2;
    }

    return 3;
}

uint8_t Intel8080::IN() {
    qWarning("IN");
    //TODO
    return 3;
}

uint8_t Intel8080::CC() {
    uint8_t cycle = 0;
    if (flags.carry == 1) {
        writeData(SP - 1, PC >> 8);
        writeData(SP - 2, PC & 0x00FF);

        SP -= 2;
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
        cycle = 2;
    }

    PC += 2;

    return 5 + cycle;
}

uint8_t Intel8080::SBI() {
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

uint8_t Intel8080::RPO() {
    if (flags.parity == 0) {
        PC = fetchData(SP + 1) << 8 | fetchData(SP);
        SP +=2;
        return 3;
    }

    return 1;
}

uint8_t Intel8080::JPO() {
    if (flags.parity == 0) {
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        PC += 2;
    }

    return 3;
}

uint8_t Intel8080::XTHL() {
    uint8_t l_data = fetchData(SP);
    uint8_t h_data = fetchData(SP + 1);

    writeData(SP, L);
    writeData(SP + 1, H);

    L = l_data;
    H = h_data;

    return 5;
}

uint8_t Intel8080::CPO() {
    uint8_t cycle = 0;
    if (flags.parity == 0) {
        writeData(SP - 1, PC >> 8);
        writeData(SP - 2, PC & 0x00FF);

        SP -= 2;
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
        cycle = 2;
    }

    PC += 2;

    return 5 + cycle;
}

uint8_t Intel8080::ANI() {
    uint8_t tmp_result = A & fetchData(PC++);

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;
    flags.carry     = 0;
    flags.aux_carry = 0;

    A = tmp_result;

    return 2;
}

uint8_t Intel8080::RPE() {
    if (flags.parity == 1) {
        PC = fetchData(SP + 1) << 8 | fetchData(SP);
        SP +=2;
        return 3;
    }

    return 1;
}

uint8_t Intel8080::PCHL() {
    PC = H << 8 | L;

    return 1;
}

uint8_t Intel8080::JPE() {
    if (flags.parity == 1) {
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        PC += 2;
    }

    return 3;
}

uint8_t Intel8080::XCHG() {
    uint8_t tmp_D = D;
    uint8_t tmp_E = E;

    D = H;
    E = L;
    H = tmp_D;
    L = tmp_E;

    return 1;
}

uint8_t Intel8080::CPE() {
    uint8_t cycle = 0;
    if (flags.parity == 1) {
        writeData(SP - 1, PC >> 8);
        writeData(SP - 2, PC & 0x00FF);

        SP -= 2;
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
        cycle = 2;
    }

    PC += 2;

    return 5 + cycle;
}

uint8_t Intel8080::XRI() {
    uint8_t tmp_result = A ^ fetchData(PC++);

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;
    flags.carry     = 0;
    flags.aux_carry = 0;

    A = tmp_result;

    return 1;
}

uint8_t Intel8080::RP() {
    if (flags.sign == 0) {
        PC = fetchData(SP + 1) << 8 | fetchData(SP);
        SP +=2;
        return 3;
    }

    return 1;
}

uint8_t Intel8080::JP() {
    if (flags.sign == 0) {
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        PC += 2;
    }

    return 3;
}

uint8_t Intel8080::DI() {
    enable_interrupts = false;
    return 1;
}

uint8_t Intel8080::CP() {
    uint8_t cycle = 0;
    if (flags.sign == 0) {
        writeData(SP - 1, PC >> 8);
        writeData(SP - 2, PC & 0x00FF);

        SP -= 2;
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
        cycle = 2;
    }

    PC += 2;

    return 5 + cycle;
}

uint8_t Intel8080::ORI() {
    uint8_t tmp_result = A | fetchData(PC++);

    flags.zero      = tmp_result == 0x00;
    flags.sign      = (tmp_result >> 7) == 1;
    flags.parity    = (tmp_result % 2) == 0;
    flags.carry     = 0;
    flags.aux_carry = 0;

    A = tmp_result;

    return 1;
}

uint8_t Intel8080::RM() {
    if (flags.sign == 1) {
        PC = fetchData(SP + 1) << 8 | fetchData(SP);
        SP +=2;
        return 3;
    }

    return 1;
}

uint8_t Intel8080::SPHL() {
    SP = H << 8 | L;

    return 1;
}

uint8_t Intel8080::JM() {
    if (flags.sign == 1) {
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
    } else {
        PC += 2;
    }

    return 3;
}

uint8_t Intel8080::EI() {
    enable_interrupts = true;
    return 1;
}

uint8_t Intel8080::CM() {
    uint8_t cycle = 0;
    if (flags.sign == 1) {
        writeData(SP - 1, PC >> 8);
        writeData(SP - 2, PC & 0x00FF);

        SP -= 2;
        PC = fetchData(PC + 1) << 8 | fetchData(PC);
        cycle = 2;
    }

    PC += 2;

    return 5 + cycle;
}

uint8_t Intel8080::CPI() {
    uint8_t data = fetchData(PC++);
    uint8_t result = A - data;

    flags.zero      = A == data;
    flags.sign      = (result >> 7) == 1;
    flags.parity    = (result % 2) == 0;
    flags.carry     = A < data;
    flags.aux_carry = ((A & 0x0F) - (data & 0x0F)) > 0xFF;

    return 2;
}

Intel8080::~Intel8080() = default;
