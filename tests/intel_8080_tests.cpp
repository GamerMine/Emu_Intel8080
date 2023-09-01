//
// Created by maxime on 05/08/23.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <QDebug>
#include "doctest.h"
#include "intel_8080.h"

uint8_t ram[1024*6];
intel_8080 cpu;

TEST_CASE("JMP tests") {
    for (unsigned char & i : ram) {
        i = 0x00;
    }
    cpu.reset();
    ram[0x00] = 0x00;
    ram[0x01] = 0x10;

    cpu.loadRAM(ram);
    cpu.opcodes[0xC3]();

    CHECK((cpu.PC == 0x1000));
}

TEST_CASE("LXI tests") {
    SUBCASE("Register B,C") {
        for (unsigned char & i : ram) {
        i = 0x00;
    }
        cpu.reset();
        ram[0x00] = 0x01;
        ram[0x01] = 0x10;

        cpu.loadRAM(ram);
        cpu.opcodes[0x01]();

        CHECK((cpu.B == 0x10));
        CHECK((cpu.C == 0x01));
    }
    SUBCASE("Register D,E") {
        for (unsigned char & i : ram) {
        i = 0x00;
    }
        cpu.reset();
        ram[0x00] = 0x01;
        ram[0x01] = 0x10;

        cpu.loadRAM(ram);
        cpu.opcodes[0x11]();

        CHECK((cpu.D == 0x10));
        CHECK((cpu.E == 0x01));
    }
    SUBCASE("Register H,L") {
        for (unsigned char & i : ram) {
        i = 0x00;
    }
        cpu.reset();
        ram[0x00] = 0x01;
        ram[0x01] = 0x10;

        cpu.loadRAM(ram);
        cpu.opcodes[0x21]();

        CHECK((cpu.H == 0x10));
        CHECK((cpu.L == 0x01));
    }
    SUBCASE("On SP") {
        for (unsigned char & i : ram) {
        i = 0x00;
    }
        cpu.reset();
        ram[0x00] = 0x01;
        ram[0x01] = 0x10;

        cpu.loadRAM(ram);
        cpu.opcodes[0x31]();

        CHECK((cpu.SP == 0x1001));
    }
}

TEST_CASE("MVI tests") {
    SUBCASE("Register B") {
        for (unsigned char & i : ram) {
        i = 0x00;
    }
        cpu.reset();
        ram[0x00] = 0x10;

        cpu.loadRAM(ram);
        cpu.opcodes[0x06]();

        CHECK((cpu.B == 0x10));
    }
    SUBCASE("Register C") {
        for (unsigned char & i : ram) {
        i = 0x00;
    }
        cpu.reset();
        ram[0x00] = 0x10;

        cpu.loadRAM(ram);
        cpu.opcodes[0x0E]();

        CHECK((cpu.C == 0x10));
    }
    SUBCASE("Register D") {
        for (unsigned char & i : ram) {
        i = 0x00;
    }
        cpu.reset();
        ram[0x00] = 0x10;

        cpu.loadRAM(ram);
        cpu.opcodes[0x16]();

        CHECK((cpu.D == 0x10));
    }
    SUBCASE("Register E") {
        for (unsigned char & i : ram) {
        i = 0x00;
    }
        cpu.reset();
        ram[0x00] = 0x10;

        cpu.loadRAM(ram);
        cpu.opcodes[0x1E]();

        CHECK((cpu.E == 0x10));
    }
    SUBCASE("Register H") {
        for (unsigned char & i : ram) {
        i = 0x00;
    }
        cpu.reset();
        ram[0x00] = 0x10;

        cpu.loadRAM(ram);
        cpu.opcodes[0x26]();

        CHECK((cpu.H == 0x10));
    }
    SUBCASE("Register L") {
        for (unsigned char & i : ram) {
        i = 0x00;
    }
        cpu.reset();
        ram[0x00] = 0x10;

        cpu.loadRAM(ram);
        cpu.opcodes[0x2E]();

        CHECK((cpu.L == 0x10));
    }
    SUBCASE("Memory") {
        for (unsigned char & i : ram) {
        i = 0x00;
    }
        cpu.reset();

        for (unsigned char & i : ram) {
            i = 0x00;
        }

        ram[0x00] = 0xFF;
        cpu.H = 0x10;
        cpu.L = 0x00;

        cpu.loadRAM(ram);
        cpu.opcodes[0x36]();

        CHECK((ram[0x1000] == 0xFF));
    }
    SUBCASE("Register A") {
        for (unsigned char & i : ram) {
        i = 0x00;
    }
        cpu.reset();

        for (unsigned char & i : ram) {
            i = 0x00;
        }

        ram[0x00] = 0x10;

        cpu.loadRAM(ram);
        cpu.opcodes[0x3E]();

        CHECK((cpu.A == 0x10));
    }
}

TEST_CASE("CALL tests") {
    for (unsigned char & i : ram) {
        i = 0x00;
    }
    cpu.reset();

    cpu.SP = 0x500;
    ram[0x00] = 0x00;
    ram[0x01] = 0x10;

    cpu.loadRAM(ram);
    cpu.opcodes[0xCD]();

    CHECK((cpu.SP == 0x4FE));
    CHECK((cpu.PC == 0x1000));
    CHECK((ram[0x4FF] == 0x00));
    CHECK((ram[0x4FE] == 0x02));
}

TEST_CASE("LDAX tests") {
    SUBCASE("Register B,C") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.B = 0x10;
        cpu.C = 0x00;
        ram[0x1000] = 0xFF;

        cpu.opcodes[0x0A]();

        CHECK((cpu.A == 0xFF));
    }
    SUBCASE("Register D,E") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.D = 0x10;
        cpu.E = 0x00;
        ram[0x1000] = 0xFF;

        cpu.opcodes[0x1A]();

        CHECK((cpu.A == 0xFF));
    }
}

TEST_CASE("MOV tests") {
    SUBCASE("From B To B") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.B = 0xFF;

        cpu.opcodes[0x40]();

        CHECK((cpu.B == 0xFF));
    }
    SUBCASE("From C To B") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.C = 0xFF;

        cpu.opcodes[0x41]();

        CHECK((cpu.B == 0xFF));
    }
    SUBCASE("From D To B") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.D = 0xFF;

        cpu.opcodes[0x42]();

        CHECK((cpu.B == 0xFF));
    }
    SUBCASE("From E To B") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.E = 0xFF;

        cpu.opcodes[0x43]();

        CHECK((cpu.B == 0xFF));
    }
    SUBCASE("From H To B") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0xFF;

        cpu.opcodes[0x44]();

        CHECK((cpu.B == 0xFF));
    }
    SUBCASE("From L To B") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.L = 0xFF;

        cpu.opcodes[0x45]();

        CHECK((cpu.B == 0xFF));
    }
    SUBCASE("From Memory To B") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0x10;
        cpu.L = 0x00;
        ram[0x1000] = 0xFF;

        cpu.opcodes[0x46]();

        CHECK((cpu.B == 0xFF));
    }
    SUBCASE("From A To B") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0xFF;

        cpu.opcodes[0x47]();

        CHECK((cpu.B == 0xFF));
    }
    SUBCASE("From B To C") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.B = 0xFF;

        cpu.opcodes[0x48]();

        CHECK((cpu.C == 0xFF));
    }
    SUBCASE("From C To C") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.C = 0xFF;

        cpu.opcodes[0x49]();

        CHECK((cpu.C == 0xFF));
    }
    SUBCASE("From D To C") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.D = 0xFF;

        cpu.opcodes[0x4A]();

        CHECK((cpu.C == 0xFF));
    }
    SUBCASE("From E To C") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.E = 0xFF;

        cpu.opcodes[0x4B]();

        CHECK((cpu.C == 0xFF));
    }
    SUBCASE("From H To C") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0xFF;

        cpu.opcodes[0x4C]();

        CHECK((cpu.C == 0xFF));
    }
    SUBCASE("From L To C") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.L = 0xFF;

        cpu.opcodes[0x4D]();

        CHECK((cpu.C == 0xFF));
    }
    SUBCASE("From Memory To C") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0x10;
        cpu.L = 0x00;
        ram[0x1000] = 0xFF;

        cpu.opcodes[0x4E]();

        CHECK((cpu.C == 0xFF));
    }
    SUBCASE("From A To C") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0xFF;

        cpu.opcodes[0x4F]();

        CHECK((cpu.C == 0xFF));
    }
    SUBCASE("From B To D") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.B = 0xFF;

        cpu.opcodes[0x50]();

        CHECK((cpu.D == 0xFF));
    }
    SUBCASE("From C To D") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.C = 0xFF;

        cpu.opcodes[0x51]();

        CHECK((cpu.D == 0xFF));
    }
    SUBCASE("From D To D") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.D = 0xFF;

        cpu.opcodes[0x52]();

        CHECK((cpu.D == 0xFF));
    }
    SUBCASE("From E To D") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.E = 0xFF;

        cpu.opcodes[0x53]();

        CHECK((cpu.D == 0xFF));
    }
    SUBCASE("From H To D") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0xFF;

        cpu.opcodes[0x54]();

        CHECK((cpu.D == 0xFF));
    }
    SUBCASE("From L To D") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.L = 0xFF;

        cpu.opcodes[0x55]();

        CHECK((cpu.D == 0xFF));
    }
    SUBCASE("From Memory To D") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0x10;
        cpu.L = 0x00;
        ram[0x1000] = 0xFF;

        cpu.opcodes[0x56]();

        CHECK((cpu.D == 0xFF));
    }
    SUBCASE("From A To D") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0xFF;

        cpu.opcodes[0x57]();

        CHECK((cpu.D == 0xFF));
    }
    SUBCASE("From B To E") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.B = 0xFF;

        cpu.opcodes[0x58]();

        CHECK((cpu.E == 0xFF));
    }
    SUBCASE("From C To E") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.C = 0xFF;

        cpu.opcodes[0x59]();

        CHECK((cpu.E == 0xFF));
    }
    SUBCASE("From D To E") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.D = 0xFF;

        cpu.opcodes[0x5A]();

        CHECK((cpu.E == 0xFF));
    }
    SUBCASE("From E To E") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.E = 0xFF;

        cpu.opcodes[0x5B]();

        CHECK((cpu.E == 0xFF));
    }
    SUBCASE("From H To E") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0xFF;

        cpu.opcodes[0x5C]();

        CHECK((cpu.E == 0xFF));
    }
    SUBCASE("From L To E") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.L = 0xFF;

        cpu.opcodes[0x5D]();

        CHECK((cpu.E == 0xFF));
    }
    SUBCASE("From Memory To E") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0x10;
        cpu.L = 0x00;
        ram[0x1000] = 0xFF;

        cpu.opcodes[0x5E]();

        CHECK((cpu.E == 0xFF));
    }
    SUBCASE("From A To E") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0xFF;

        cpu.opcodes[0x5F]();

        CHECK((cpu.E == 0xFF));
    }
    SUBCASE("From B To H") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.B = 0xFF;

        cpu.opcodes[0x60]();

        CHECK((cpu.H == 0xFF));
    }
    SUBCASE("From C To H") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.C = 0xFF;

        cpu.opcodes[0x61]();

        CHECK((cpu.H == 0xFF));
    }
    SUBCASE("From D To H") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.D = 0xFF;

        cpu.opcodes[0x62]();

        CHECK((cpu.H == 0xFF));
    }
    SUBCASE("From E To H") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.E = 0xFF;

        cpu.opcodes[0x63]();

        CHECK((cpu.H == 0xFF));
    }
    SUBCASE("From H To H") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0xFF;

        cpu.opcodes[0x64]();

        CHECK((cpu.H == 0xFF));
    }
    SUBCASE("From L To H") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.L = 0xFF;

        cpu.opcodes[0x65]();

        CHECK((cpu.H == 0xFF));
    }
    SUBCASE("From Memory To H") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0x10;
        cpu.L = 0x00;
        ram[0x1000] = 0xFF;

        cpu.opcodes[0x66]();

        CHECK((cpu.H == 0xFF));
    }
    SUBCASE("From A To H") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0xFF;

        cpu.opcodes[0x67]();

        CHECK((cpu.H == 0xFF));
    }
    SUBCASE("From B To L") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.B = 0xFF;

        cpu.opcodes[0x68]();

        CHECK((cpu.L == 0xFF));
    }
    SUBCASE("From C To L") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.C = 0xFF;

        cpu.opcodes[0x69]();

        CHECK((cpu.L == 0xFF));
    }
    SUBCASE("From D To L") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.D = 0xFF;

        cpu.opcodes[0x6A]();

        CHECK((cpu.L == 0xFF));
    }
    SUBCASE("From E To L") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.E = 0xFF;

        cpu.opcodes[0x6B]();

        CHECK((cpu.L == 0xFF));
    }
    SUBCASE("From H To L") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0xFF;

        cpu.opcodes[0x6C]();

        CHECK((cpu.L == 0xFF));
    }
    SUBCASE("From L To L") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.L = 0xFF;

        cpu.opcodes[0x6D]();

        CHECK((cpu.L == 0xFF));
    }
    SUBCASE("From Memory To L") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0x10;
        cpu.L = 0x00;
        ram[0x1000] = 0xFF;

        cpu.opcodes[0x6E]();

        CHECK((cpu.L == 0xFF));
    }
    SUBCASE("From A To L") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0xFF;

        cpu.opcodes[0x6F]();

        CHECK((cpu.L == 0xFF));
    }
    SUBCASE("From B To Memory") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.B = 0xFF;
        cpu.H = 0x10;
        cpu.L = 0x00;

        cpu.opcodes[0x70]();

        CHECK((ram[0x1000] == 0xFF));
    }
    SUBCASE("From C To Memory") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.C = 0xFF;
        cpu.H = 0x10;
        cpu.L = 0x00;

        cpu.opcodes[0x71]();

        CHECK((ram[0x1000] == 0xFF));
    }
    SUBCASE("From D To Memory") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.D = 0xFF;
        cpu.H = 0x10;
        cpu.L = 0x00;

        cpu.opcodes[0x72]();

        CHECK((ram[0x1000] == 0xFF));
    }
    SUBCASE("From E To Memory") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.E = 0xFF;
        cpu.H = 0x10;
        cpu.L = 0x00;

        cpu.opcodes[0x73]();

        CHECK((ram[0x1000] == 0xFF));
    }
    SUBCASE("From H To Memory") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0x10;
        cpu.L = 0x00;

        cpu.opcodes[0x74]();

        CHECK((ram[0x1000] == 0x10));
    }
    SUBCASE("From L To Memory") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0x10;
        cpu.L = 0x00;

        cpu.opcodes[0x75]();

        CHECK((ram[0x1000] == 0x00));
    }
    SUBCASE("From A To Memory") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0xFF;
        cpu.H = 0x10;
        cpu.L = 0x00;

        cpu.opcodes[0x77]();

        CHECK((ram[0x1000] == 0xFF));
    }
    SUBCASE("From B To A") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.B = 0xFF;

        cpu.opcodes[0x78]();

        CHECK((cpu.A == 0xFF));
    }
    SUBCASE("From C To A") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.C = 0xFF;

        cpu.opcodes[0x79]();

        CHECK((cpu.A == 0xFF));
    }
    SUBCASE("From D To A") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.D = 0xFF;

        cpu.opcodes[0x7A]();

        CHECK((cpu.A == 0xFF));
    }
    SUBCASE("From E To A") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.E = 0xFF;

        cpu.opcodes[0x7B]();

        CHECK((cpu.A == 0xFF));
    }
    SUBCASE("From H To A") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0xFF;

        cpu.opcodes[0x7C]();

        CHECK((cpu.A == 0xFF));
    }
    SUBCASE("From L To A") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.L = 0xFF;

        cpu.opcodes[0x7D]();

        CHECK((cpu.A == 0xFF));
    }
    SUBCASE("From Memory To A") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0x10;
        cpu.L = 0x00;
        ram[0x1000] = 0xFF;

        cpu.opcodes[0x7E]();

        CHECK((cpu.A == 0xFF));
    }
    SUBCASE("From A To A") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0xFF;

        cpu.opcodes[0x7F]();

        CHECK((cpu.A == 0xFF));
    }
}

TEST_CASE("INX tests") {
    SUBCASE("Register B,C") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.opcodes[0x03]();

        CHECK((cpu.B == 0x00));
        CHECK((cpu.C == 0x01));
    }
    SUBCASE("Register D,E") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.opcodes[0x13]();

        CHECK((cpu.D == 0x00));
        CHECK((cpu.E == 0x01));
    }
    SUBCASE("Register H,L") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.opcodes[0x23]();

        CHECK((cpu.H == 0x00));
        CHECK((cpu.L == 0x01));
    }
    SUBCASE("Stack pointer") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.opcodes[0x33]();

        CHECK((cpu.SP == 0x0001));
    }
}

TEST_CASE("DCR tests") {
    SUBCASE("Register B") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.B = 0xFF;

        cpu.opcodes[0x05]();

        CHECK((cpu.B == 0xFE));
    }
    SUBCASE("Register C") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.C = 0xFF;

        cpu.opcodes[0x0D]();

        CHECK((cpu.C == 0xFE));
    }
    SUBCASE("Register D") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.D = 0xFF;

        cpu.opcodes[0x15]();

        CHECK((cpu.D == 0xFE));
    }
    SUBCASE("Register E") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.E = 0xFF;

        cpu.opcodes[0x1D]();

        CHECK((cpu.E == 0xFE));
    }
    SUBCASE("Register H") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0xFF;

        cpu.opcodes[0x25]();

        CHECK((cpu.H == 0xFE));
    }
    SUBCASE("Register L") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.L = 0xFF;

        cpu.opcodes[0x2D]();

        CHECK((cpu.L == 0xFE));
    }
    SUBCASE("Register Memory") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0x10;
        cpu.L = 0x00;
        ram[0x1000] = 0xFF;

        cpu.loadRAM(ram);
        cpu.opcodes[0x35]();

        CHECK((ram[0x1000] == 0xFE));
    }
    SUBCASE("Register A") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0xFF;

        cpu.opcodes[0x3D]();

        CHECK((cpu.A == 0xFE));
    }
}

TEST_CASE("JNZ") {
    SUBCASE("Condition met") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        ram[0x00] = 0x00;
        ram[0x01] = 0x10;
        cpu.flags.zero = 0;

        cpu.loadRAM(ram);
        cpu.opcodes[0xC2]();

        CHECK((cpu.PC == 0x1000));
    }
    SUBCASE("Condition unmet") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        ram[0x00] = 0x00;
        ram[0x01] = 0x10;
        cpu.flags.zero = 1;

        cpu.loadRAM(ram);
        cpu.opcodes[0xC2]();

        CHECK((cpu.PC == 0x0002));
    }
}

TEST_CASE("JZ") {
    SUBCASE("Condition met") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        ram[0x00] = 0x00;
        ram[0x01] = 0x10;
        cpu.flags.zero = 1;

        cpu.loadRAM(ram);
        cpu.opcodes[0xCA]();

        CHECK((cpu.PC == 0x1000));
    }
    SUBCASE("Condition unmet") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        ram[0x00] = 0x00;
        ram[0x01] = 0x10;
        cpu.flags.zero = 0;

        cpu.loadRAM(ram);
        cpu.opcodes[0xCA]();

        CHECK((cpu.PC == 0x0002));
    }
}

TEST_CASE("JNC") {
    SUBCASE("Condition met") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        ram[0x00] = 0x00;
        ram[0x01] = 0x10;
        cpu.flags.carry = 0;

        cpu.loadRAM(ram);
        cpu.opcodes[0xD2]();

        CHECK((cpu.PC == 0x1000));
    }
    SUBCASE("Condition unmet") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        ram[0x00] = 0x00;
        ram[0x01] = 0x10;
        cpu.flags.carry = 1;

        cpu.loadRAM(ram);
        cpu.opcodes[0xD2]();

        CHECK((cpu.PC == 0x0002));
    }
}

TEST_CASE("JC") {
    SUBCASE("Condition met") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        ram[0x00] = 0x00;
        ram[0x01] = 0x10;
        cpu.flags.carry = 1;

        cpu.loadRAM(ram);
        cpu.opcodes[0xDA]();

        CHECK((cpu.PC == 0x1000));
    }
    SUBCASE("Condition unmet") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        ram[0x00] = 0x00;
        ram[0x01] = 0x10;
        cpu.flags.carry = 0;

        cpu.loadRAM(ram);
        cpu.opcodes[0xDA]();

        CHECK((cpu.PC == 0x0002));
    }
}

TEST_CASE("RET") {
    for (unsigned char & i : ram) {
        i = 0x00;
    }
    cpu.reset();

    ram[0x1000] = 0xFF;
    ram[0x1001] = 0x10;
    cpu.SP = 0x1000;

    cpu.loadRAM(ram);
    cpu.opcodes[0xC9]();

    CHECK((cpu.SP == 0x1002));
    CHECK((cpu.PC == 0x10FF));
}

TEST_CASE("CPI") {
    SUBCASE("Flag zero") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        ram[0x00] = 0x02;
        cpu.A = 0x02;

        cpu.loadRAM(ram);
        cpu.opcodes[0xFE]();

        CHECK((cpu.flags.zero == 1));
        CHECK((cpu.flags.sign == 0));
        CHECK((cpu.flags.parity == 1));
        CHECK((cpu.flags.carry == 0));
        CHECK((cpu.flags.aux_carry == 0));
    }
    SUBCASE("Flag carry") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        ram[0x00] = 0x04;
        cpu.A = 0x02;

        cpu.loadRAM(ram);
        cpu.opcodes[0xFE]();

        CHECK((cpu.flags.zero == 0));
        CHECK((cpu.flags.sign == 1));
        CHECK((cpu.flags.parity == 1));
        CHECK((cpu.flags.carry == 1));
        CHECK((cpu.flags.aux_carry == 0));
    }
}

TEST_CASE("PUSH") {
    SUBCASE("Register B,C") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.SP = 0x1000;
        cpu.B = 0x10;
        cpu.C = 0xFF;

        cpu.opcodes[0xC5]();

        CHECK((cpu.SP == 0x0FFE));
        CHECK((ram[0x0FFF] == 0x10));
        CHECK((ram[0x0FFE] == 0xFF));
    }
    SUBCASE("Register D,E") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.SP = 0x1000;
        cpu.D = 0x10;
        cpu.E = 0xFF;

        cpu.opcodes[0xD5]();

        CHECK((cpu.SP == 0x0FFE));
        CHECK((ram[0x0FFF] == 0x10));
        CHECK((ram[0x0FFE] == 0xFF));
    }
    SUBCASE("Register H,L") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.SP = 0x1000;
        cpu.H = 0x10;
        cpu.L = 0xFF;

        cpu.opcodes[0xE5]();

        CHECK((cpu.SP == 0x0FFE));
        CHECK((ram[0x0FFF] == 0x10));
        CHECK((ram[0x0FFE] == 0xFF));
    }
    SUBCASE("Register PSW") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.SP = 0x1000;
        cpu.A = 0x10;
        cpu.flags.carry = 1;
        cpu.flags.parity = 1;
        cpu.flags.aux_carry = 1;
        cpu.flags.zero = 1;
        cpu.flags.sign = 1;

        cpu.opcodes[0xF5]();

        CHECK((cpu.SP == 0x0FFE));
        CHECK((ram[0x0FFF] == 0x10));
        CHECK((ram[0x0FFE] == 0xD7));
    }
}

TEST_CASE("DAD") {
    SUBCASE("Register B,C") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0x10;
        cpu.L = 0xFF;
        cpu.B = 0x01;
        cpu.C = 0x01;

        cpu.opcodes[0x09]();

        CHECK((cpu.H == 0x12));
        CHECK((cpu.L == 0x00));
    }
    SUBCASE("Register D,E") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0x10;
        cpu.L = 0xFF;
        cpu.D = 0x01;
        cpu.E = 0x01;

        cpu.opcodes[0x19]();

        CHECK((cpu.H == 0x12));
        CHECK((cpu.L == 0x00));
    }
    SUBCASE("Register H,L") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0x01;
        cpu.L = 0x01;

        cpu.opcodes[0x29]();

        CHECK((cpu.H == 0x02));
        CHECK((cpu.L == 0x02));
    }
    SUBCASE("Register B,C") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.H = 0x10;
        cpu.L = 0xFF;
        cpu.SP = 0x0101;

        cpu.opcodes[0x39]();

        CHECK((cpu.H == 0x12));
        CHECK((cpu.L == 0x00));
    }
}

TEST_CASE("XCHG") {
    for (unsigned char & i : ram) {
        i = 0x00;
    }
    cpu.reset();

    cpu.H = 0x01;
    cpu.L = 0x01;
    cpu.D = 0x10;
    cpu.E = 0x10;

    cpu.opcodes[0xEB]();

    CHECK((cpu.H == 0x10));
    CHECK((cpu.L == 0x10));
    CHECK((cpu.D == 0x01));
    CHECK((cpu.E == 0x01));
}

TEST_CASE("POP") {
    SUBCASE("Register B,C") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.SP = 0x1000;
        ram[0x1000] = 0xFF;
        ram[0x1001] = 0x10;

        cpu.loadRAM(ram);
        cpu.opcodes[0xC1]();

        CHECK((cpu.SP == 0x1002));
        CHECK((cpu.B == 0x10));
        CHECK((cpu.C == 0xFF));
    }
    SUBCASE("Register D,E") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.SP = 0x1000;
        ram[0x1000] = 0xFF;
        ram[0x1001] = 0x10;

        cpu.loadRAM(ram);
        cpu.opcodes[0xD1]();

        CHECK((cpu.SP == 0x1002));
        CHECK((cpu.D == 0x10));
        CHECK((cpu.E == 0xFF));
    }
    SUBCASE("Register H,L") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.SP = 0x1000;
        ram[0x1000] = 0xFF;
        ram[0x1001] = 0x10;

        cpu.loadRAM(ram);
        cpu.opcodes[0xE1]();

        CHECK((cpu.SP == 0x1002));
        CHECK((cpu.H == 0x10));
        CHECK((cpu.L == 0xFF));
    }
    SUBCASE("Register PSW") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.SP = 0x1000;
        ram[0x1000] = 0xD7;
        ram[0x1001] = 0xFF;

        cpu.loadRAM(ram);
        cpu.opcodes[0xF1]();

        CHECK((cpu.SP == 0x1002));
        CHECK((cpu.flags.zero == 1));
        CHECK((cpu.flags.sign == 1));
        CHECK((cpu.flags.parity == 1));
        CHECK((cpu.flags.carry == 1));
        CHECK((cpu.flags.aux_carry == 1));
        CHECK((cpu.A == 0xFF));
    }
}

TEST_CASE("RRC") {
    SUBCASE("No Carry") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0x10;

        cpu.opcodes[0x0F]();

        CHECK((cpu.flags.carry == 0));
        CHECK((cpu.A == 0x08));
    }
    SUBCASE("Carry Out") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0x01;

        cpu.opcodes[0x0F]();

        CHECK((cpu.flags.carry == 1));
        CHECK((cpu.A == 0x80));
    }
}

TEST_CASE("ANI") {
    for (unsigned char & i : ram) {
        i = 0x00;
    }
    cpu.reset();

    cpu.A = 0xFF;
    ram[0x00] = 0x0F;

    cpu.loadRAM(ram);
    cpu.opcodes[0xE6]();

    CHECK((cpu.A == 0x0F));
    CHECK((cpu.PC == 0x0001));
}

TEST_CASE("ANA") {
    SUBCASE("Register A") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0x00;

        cpu.opcodes[0xA7]();

        CHECK((cpu.A == 0x00));
        CHECK((cpu.flags.zero == 1));
    }
}

TEST_CASE("ADI") {
    for (unsigned char & i : ram) {
        i = 0x00;
    }
    cpu.reset();

    cpu.A = 0x10;
    ram[0x0000] = 0x01;

    cpu.loadRAM(ram);
    cpu.opcodes[0xC6]();

    CHECK((cpu.A == 0x11));
    CHECK((cpu.PC == 0x0001));
}

TEST_CASE("STA") {
    for (unsigned char & i : ram) {
        i = 0x00;
    }
    cpu.reset();

    cpu.A = 0x10;
    ram[0x0000] = 0xFF;
    ram[0x0001] = 0x10;

    cpu.loadRAM(ram);
    cpu.opcodes[0x32]();

    CHECK((ram[0x10FF] == 0x10));
    CHECK((cpu.PC == 0x0002));
}

TEST_CASE("XRA") {
    SUBCASE("Register B") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0x0F;
        cpu.B = 0xF0;

        cpu.opcodes[0xA8]();

        CHECK((cpu.A == 0xFF));
    }
    SUBCASE("Register C") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0x0F;
        cpu.C = 0xF0;

        cpu.opcodes[0xA9]();

        CHECK((cpu.A == 0xFF));
    }
    SUBCASE("Register D") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0x0F;
        cpu.D = 0xF0;

        cpu.opcodes[0xAA]();

        CHECK((cpu.A == 0xFF));
    }
    SUBCASE("Register E") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0x0F;
        cpu.E = 0xF0;

        cpu.opcodes[0xAB]();

        CHECK((cpu.A == 0xFF));
    }
    SUBCASE("Register H") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0x0F;
        cpu.H = 0xF0;

        cpu.opcodes[0xAC]();

        CHECK((cpu.A == 0xFF));
    }
    SUBCASE("Register L") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0x0F;
        cpu.L = 0xF0;

        cpu.opcodes[0xAD]();

        CHECK((cpu.A == 0xFF));
    }
    SUBCASE("Register Memory") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0x0F;
        cpu.H = 0x10;
        cpu.L = 0x00;
        ram[0x1000] = 0xF0;

        cpu.loadRAM(ram);
        cpu.opcodes[0xAE]();

        CHECK((cpu.A == 0xFF));
    }
    SUBCASE("Register A") {
        for (unsigned char & i : ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0x0F;
        cpu.A = 0xF0;

        cpu.opcodes[0xAF]();

        CHECK((cpu.A == 0x00));
    }
}

TEST_CASE("RZ") {
    SUBCASE("Condition met") {
        for (unsigned char &i: ram) {
            i = 0x00;
        }
        cpu.reset();

        ram[0x1000] = 0xFF;
        ram[0x1001] = 0x10;
        cpu.SP = 0x1000;
        cpu.flags.zero = 1;

        cpu.loadRAM(ram);
        cpu.opcodes[0xC8]();

        CHECK((cpu.SP == 0x1002));
        CHECK((cpu.PC == 0x10FF));
    }
    SUBCASE("Condition unmet") {
        for (unsigned char &i: ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.flags.zero = 0;

        cpu.opcodes[0xC8]();

        CHECK((cpu.PC == 0x0000));
    }
}

TEST_CASE("PCHL") {
    for (unsigned char &i: ram) {
        i = 0x00;
    }
    cpu.reset();

    cpu.H = 0x10;
    cpu.L = 0xFF;

    cpu.opcodes[0xE9]();

    CHECK((cpu.PC == 0x10FF));
}

TEST_CASE("XTHL") {
    for (unsigned char &i: ram) {
        i = 0x00;
    }
    cpu.reset();

    cpu.SP = 0x1000;
    ram[0x1000] = 0x10;
    ram[0x1001] = 0xCC;
    cpu.H = 0x05;
    cpu.L = 0xFF;

    cpu.opcodes[0xE3]();

    CHECK((ram[0x1000] == 0xFF));
    CHECK((ram[0x1001] == 0x05));
    CHECK((cpu.L == 0x10));
    CHECK((cpu.H == 0xCC));
}

TEST_CASE("ORA") {
    SUBCASE("Register B") {
        for (unsigned char &i: ram) {
            i = 0x00;
        }
        cpu.reset();

        cpu.A = 0x0F;
        cpu.B = 0xF0;

        cpu.opcodes[0x80]();

        CHECK((cpu.A == 0xFF));
    }
}