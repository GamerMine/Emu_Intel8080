//
// Created by maxime on 27/07/23.
//

#ifndef EMU_INTEL8080_INTEL8080_H
#define EMU_INTEL8080_INTEL8080_H

#include <cstdio>
#include <cstdint>
#include <QThread>
#include "Bus.h"

class Intel8080 : public QThread {
    Q_OBJECT
public:
    explicit Intel8080(class Bus *bus);
    ~Intel8080();

public slots:
    void setInterrupt(uint16_t value);

signals:
    void playSound(uint8_t data, uint8_t port);

private:
    Bus *main_bus;
    uint8_t A, B, C, D, E, H, L;
    uint16_t SP, PC;
    struct {
        bool sign       = 0;
        bool zero       = 0;
        bool parity     = 0;
        bool carry      = 0;
        bool aux_carry  = 0;
    } flags;
    bool hlt;
    bool enable_interrupts;
    uint8_t interrupt;

    using opcodes_func = std::function<uint8_t()>;
    opcodes_func opcodes[256] = {
            [this]() {return NOP();},
            [this]() {return LXI(&B, &C);},
            [this]() {return STAX(B, C);},
            [this]() {return INX(&B, &C);},
            [this]() {return INR(&B);},
            [this]() {return DCR(&B);},
            [this]() {return MVI(&B);},
            [this]() {return RLC();},
            NIMP,
            [this]() {return DAD(&B, &C);},
            [this]() {return LDAX(B, C);},
            [this]() {return DCX(&B, &C);},
            [this]() {return INR(&C);},
            [this]() {return DCR(&C);},
            [this]() {return MVI(&C);},
            [this]() {return RRC();},
            NIMP,
            [this]() {return LXI(&D, &E);},
            [this]() {return STAX(D, E);},
            [this]() {return INX(&D, &E);},
            [this]() {return INR(&D);},
            [this]() {return DCR(&D);},
            [this]() {return MVI(&D);},
            [this]() {return RAL();},
            NIMP,
            [this]() {return DAD(&D, &E);},
            [this]() {return LDAX(D, E);},
            [this]() {return DCX(&D, &E);},
            [this]() {return INR(&E);},
            [this]() {return DCR(&E);},
            [this]() {return MVI(&E);},
            [this]() {return RAR();},
            NIMP,
            [this]() {return LXI(&H, &L);},
            [this]() {return SHLD();},
            [this]() {return INX(&H, &L);},
            [this]() {return INR(&H);},
            [this]() {return DCR(&H);},
            [this]() {return MVI(&H);},
            [this]() {return DAA();},
            NIMP,
            [this]() {return DAD(&H, &L);},
            [this]() {return LHLD();},
            [this]() {return DCX(&H, &L);},
            [this]() {return INR(&L);},
            [this]() {return DCR(&L);},
            [this]() {return MVI(&L);},
            [this]() {return CMA();},
            NIMP,
            [this]() {return LXI(nullptr, nullptr);},
            [this]() { return STA();},
            [this]() {return INX(nullptr, nullptr);},
            [this]() {return INR(nullptr);},
            [this]() {return DCR(nullptr);},
            [this]() {return MVI(nullptr);},
            [this]() {return STC();},
            NIMP,
            [this]() {return DAD(nullptr, nullptr);},
            [this]() {return LDA();},
            [this]() {return DCX(nullptr, nullptr);},
            [this]() {return INR(&A);},
            [this]() {return DCR(&A);},
            [this]() {return MVI(&A);},
            [this]() {return CMC();},
            [this]() {return MOV(&B, &B);},
            [this]() {return MOV(&B, &C);},
            [this]() {return MOV(&B, &D);},
            [this]() {return MOV(&B, &E);},
            [this]() {return MOV(&B, &H);},
            [this]() {return MOV(&B, &L);},
            [this]() {return MOV(&B, nullptr);},
            [this]() {return MOV(&B, &A);},
            [this]() {return MOV(&C, &B);},
            [this]() {return MOV(&C, &C);},
            [this]() {return MOV(&C, &D);},
            [this]() {return MOV(&C, &E);},
            [this]() {return MOV(&C, &H);},
            [this]() {return MOV(&C, &L);},
            [this]() {return MOV(&C, nullptr);},
            [this]() {return MOV(&C, &A);},
            [this]() {return MOV(&D, &B);},
            [this]() {return MOV(&D, &C);},
            [this]() {return MOV(&D, &D);},
            [this]() {return MOV(&D, &E);},
            [this]() {return MOV(&D, &H);},
            [this]() {return MOV(&D, &L);},
            [this]() {return MOV(&D, nullptr);},
            [this]() {return MOV(&D, &A);},
            [this]() {return MOV(&E, &B);},
            [this]() {return MOV(&E, &C);},
            [this]() {return MOV(&E, &D);},
            [this]() {return MOV(&E, &E);},
            [this]() {return MOV(&E, &H);},
            [this]() {return MOV(&E, &L);},
            [this]() {return MOV(&E, nullptr);},
            [this]() {return MOV(&E, &A);},
            [this]() {return MOV(&H, &B);},
            [this]() {return MOV(&H, &C);},
            [this]() {return MOV(&H, &D);},
            [this]() {return MOV(&H, &E);},
            [this]() {return MOV(&H, &H);},
            [this]() {return MOV(&H, &L);},
            [this]() {return MOV(&H, nullptr);},
            [this]() {return MOV(&H, &A);},
            [this]() {return MOV(&L, &B);},
            [this]() {return MOV(&L, &C);},
            [this]() {return MOV(&L, &D);},
            [this]() {return MOV(&L, &E);},
            [this]() {return MOV(&L, &H);},
            [this]() {return MOV(&L, &L);},
            [this]() {return MOV(&L, nullptr);},
            [this]() {return MOV(&L, &A);},
            [this]() {return MOV(nullptr, &B);},
            [this]() {return MOV(nullptr, &C);},
            [this]() {return MOV(nullptr, &D);},
            [this]() {return MOV(nullptr, &E);},
            [this]() {return MOV(nullptr, &H);},
            [this]() {return MOV(nullptr, &L);},
            [this]() {return HLT();},
            [this]() {return MOV(nullptr, &A);},
            [this]() {return MOV(&A, &B);},
            [this]() {return MOV(&A, &C);},
            [this]() {return MOV(&A, &D);},
            [this]() {return MOV(&A, &E);},
            [this]() {return MOV(&A, &H);},
            [this]() {return MOV(&A, &L);},
            [this]() {return MOV(&A, nullptr);},
            [this]() {return MOV(&A, &A);},
            [this]() {return ADD(&B);},
            [this]() {return ADD(&C);},
            [this]() {return ADD(&D);},
            [this]() {return ADD(&E);},
            [this]() {return ADD(&H);},
            [this]() {return ADD(&L);},
            [this]() {return ADD(nullptr);},
            [this]() {return ADD(&A);},
            [this]() {return ADC(&B);},
            [this]() {return ADC(&C);},
            [this]() {return ADC(&D);},
            [this]() {return ADC(&E);},
            [this]() {return ADC(&H);},
            [this]() {return ADC(&L);},
            [this]() {return ADC(nullptr);},
            [this]() {return ADC(&A);},
            [this]() {return SUB(&B);},
            [this]() {return SUB(&C);},
            [this]() {return SUB(&D);},
            [this]() {return SUB(&E);},
            [this]() {return SUB(&H);},
            [this]() {return SUB(&L);},
            [this]() {return SUB(nullptr);},
            [this]() {return SUB(&A);},
            [this]() {return SBB(&B);},
            [this]() {return SBB(&C);},
            [this]() {return SBB(&D);},
            [this]() {return SBB(&E);},
            [this]() {return SBB(&H);},
            [this]() {return SBB(&L);},
            [this]() {return SBB(nullptr);},
            [this]() {return SBB(&A);},
            [this]() {return ANA(&B);},
            [this]() {return ANA(&C);},
            [this]() {return ANA(&D);},
            [this]() {return ANA(&E);},
            [this]() {return ANA(&H);},
            [this]() {return ANA(&L);},
            [this]() {return ANA(nullptr);},
            [this]() {return ANA(&A);},
            [this]() {return XRA(&B);},
            [this]() {return XRA(&C);},
            [this]() {return XRA(&D);},
            [this]() {return XRA(&E);},
            [this]() {return XRA(&H);},
            [this]() {return XRA(&L);},
            [this]() {return XRA(nullptr);},
            [this]() {return XRA(&A);},
            [this]() {return ORA(&B);},
            [this]() {return ORA(&C);},
            [this]() {return ORA(&D);},
            [this]() {return ORA(&E);},
            [this]() {return ORA(&H);},
            [this]() {return ORA(&L);},
            [this]() {return ORA(nullptr);},
            [this]() {return ORA(&A);},
            [this]() {return CMP(&B);},
            [this]() {return CMP(&C);},
            [this]() {return CMP(&D);},
            [this]() {return CMP(&E);},
            [this]() {return CMP(&H);},
            [this]() {return CMP(&L);},
            [this]() {return CMP(nullptr);},
            [this]() {return CMP(&A);},
            [this]() {return RNZ();},
            [this]() {return POP(&B, &C);},
            [this]() {return JNZ();},
            [this]() {return JMP();},
            [this]() {return CNZ();},
            [this]() {return PUSH(&B, &C);},
            [this]() {return ADI();},
            [this]() {return RST(0x0000);},
            [this]() {return RZ();},
            [this]() {return RET();},
            [this]() {return JZ();},
            NIMP,
            [this]() {return CZ();},
            [this]() {return CALL();},
            [this]() {return ACI();},
            [this]() {return RST(0x0008);},
            [this]() {return RNC();},
            [this]() {return POP(&D, &E);},
            [this]() {return JNC();},
            [this]() {return OUT();},
            [this]() {return CNC();},
            [this]() {return PUSH(&D, &E);},
            [this]() {return SUI();},
            [this]() {return RST(0x0010);},
            [this]() {return RC();},
            NIMP,
            [this]() {return JC();},
            [this]() {return IN();},
            [this]() {return CC();},
            NIMP,
            [this]() {return SBI();},
            [this]() {return RST(0x0018);},
            [this]() {return RPO();},
            [this]() {return POP(&H, &L);},
            [this]() {return JPO();},
            [this]() {return XTHL();},
            [this]() {return CPO();},
            [this]() {return PUSH(&H, &L);},
            [this]() {return ANI();},
            [this]() {return RST(0x0020);},
            [this]() {return RPE();},
            [this]() {return PCHL();},
            [this]() {return JPE();},
            [this]() {return XCHG();},
            [this]() {return CPE();},
            NIMP,
            [this]() {return XRI();},
            [this]() {return RST(0x0028);},
            [this]() {return RP();},
            [this]() {return POP(nullptr, nullptr);},
            [this]() {return JP();},
            [this]() {return DI();},
            [this]() {return CP();},
            [this]() {return PUSH(nullptr, nullptr);},
            [this]() {return ORI();},
            [this]() {return RST(0x0030);},
            [this]() {return RM();},
            [this]() {return SPHL();},
            [this]() {return JM();},
            [this]() {return EI();},
            [this]() {return CM();},
            NIMP,
            [this]() {return CPI();},
            [this]() {return RST(0x0038);},
            };

    std::string lookup[256] = {
            "NOP()",
            "LXI(&B, &C)",
            "STAX(B, C)",
            "INX(&B, &C)",
            "INR(&B)",
            "DCR(&B)",
            "MVI(&B)",
            "RLC()",
            "NIMP()",
            "DAD(&B, &C)",
            "LDAX(B, C)",
            "DCX(&B, &C)",
            "INR(&C)",
            "DCR(&C)",
            "MVI(&C)",
            "RRC()",
            "NIMP()",
            "LXI(&D, &E)",
            "STAX(D, E)",
            "INX(&D, &E)",
            "INR(&D)",
            "DCR(&D)",
            "MVI(&D)",
            "RAL()",
            "NIMP()",
            "DAD(&D, &E)",
            "LDAX(D, E)",
            "DCX(&D, &E)",
            "INR(&E)",
            "DCR(&E)",
            "MVI(&E)",
            "RAR()",
            "NIMP()",
            "LXI(&H, &L)",
            "SHLD()",
            "INX(&H, &L)",
            "INR(&H)",
            "DCR(&H)",
            "MVI(&H)",
            "DAA()",
            "NIMP()",
            "DAD(&H, &L)",
            "LHLD()",
            "DCX(&H, &L)",
            "INR(&L)",
            "DCR(&L)",
            "MVI(&L)",
            "CMA()",
            "NIMP()",
            "LXI(nullptr, nullptr)",
            "return STA()",
            "INX(nullptr, nullptr)",
            "INR(nullptr)",
            "DCR(nullptr)",
            "MVI(nullptr)",
            "STC()",
            "NIMP()",
            "DAD(nullptr, nullptr)",
            "LDA()",
            "DCX(nullptr, nullptr)",
            "INR(&A)",
            "DCR(&A)",
            "MVI(&A)",
            "CMC()",
            "MOV(&B, &B)",
            "MOV(&B, &C)",
            "MOV(&B, &D)",
            "MOV(&B, &E)",
            "MOV(&B, &H)",
            "MOV(&B, &L)",
            "MOV(&B, nullptr)",
            "MOV(&B, &A)",
            "MOV(&C, &B)",
            "MOV(&C, &C)",
            "MOV(&C, &D)",
            "MOV(&C, &E)",
            "MOV(&C, &H)",
            "MOV(&C, &L)",
            "MOV(&C, nullptr)",
            "MOV(&C, &A)",
            "MOV(&D, &B)",
            "MOV(&D, &C)",
            "MOV(&D, &D)",
            "MOV(&D, &E)",
            "MOV(&D, &H)",
            "MOV(&D, &L)",
            "MOV(&D, nullptr)",
            "MOV(&D, &A)",
            "MOV(&E, &B)",
            "MOV(&E, &C)",
            "MOV(&E, &D)",
            "MOV(&E, &E)",
            "MOV(&E, &H)",
            "MOV(&E, &L)",
            "MOV(&E, nullptr)",
            "MOV(&E, &A)",
            "MOV(&H, &B)",
            "MOV(&H, &C)",
            "MOV(&H, &D)",
            "MOV(&H, &E)",
            "MOV(&H, &H)",
            "MOV(&H, &L)",
            "MOV(&H, nullptr)",
            "MOV(&H, &A)",
            "MOV(&L, &B)",
            "MOV(&L, &C)",
            "MOV(&L, &D)",
            "MOV(&L, &E)",
            "MOV(&L, &H)",
            "MOV(&L, &L)",
            "MOV(&L, nullptr)",
            "MOV(&L, &A)",
            "MOV(nullptr, &B)",
            "MOV(nullptr, &C)",
            "MOV(nullptr, &D)",
            "MOV(nullptr, &E)",
            "MOV(nullptr, &H)",
            "MOV(nullptr, &L)",
            "HLT()",
            "MOV(nullptr, &A)",
            "MOV(&A, &B)",
            "MOV(&A, &C)",
            "MOV(&A, &D)",
            "MOV(&A, &E)",
            "MOV(&A, &H)",
            "MOV(&A, &L)",
            "MOV(&A, nullptr)",
            "MOV(&A, &A)",
            "ADD(&B)",
            "ADD(&C)",
            "ADD(&D)",
            "ADD(&E)",
            "ADD(&H)",
            "ADD(&L)",
            "ADD(nullptr)",
            "ADD(&A)",
            "ADC(&B)",
            "ADC(&C)",
            "ADC(&D)",
            "ADC(&E)",
            "ADC(&H)",
            "ADC(&L)",
            "ADC(nullptr)",
            "ADC(&A)",
            "SUB(&B)",
            "SUB(&C)",
            "SUB(&D)",
            "SUB(&E)",
            "SUB(&H)",
            "SUB(&L)",
            "SUB(nullptr)",
            "SUB(&A)",
            "SBB(&B)",
            "SBB(&C)",
            "SBB(&D)",
            "SBB(&E)",
            "SBB(&H)",
            "SBB(&L)",
            "SBB(nullptr)",
            "SBB(&A)",
            "ANA(&B)",
            "ANA(&C)",
            "ANA(&D)",
            "ANA(&E)",
            "ANA(&H)",
            "ANA(&L)",
            "ANA(nullptr)",
            "ANA(&A)",
            "XRA(&B)",
            "XRA(&C)",
            "XRA(&D)",
            "XRA(&E)",
            "XRA(&H)",
            "XRA(&L)",
            "XRA(nullptr)",
            "XRA(&A)",
            "ORA(&B)",
            "ORA(&C)",
            "ORA(&D)",
            "ORA(&E)",
            "ORA(&H)",
            "ORA(&L)",
            "ORA(nullptr)",
            "ORA(&A)",
            "CMP(&B)",
            "CMP(&C)",
            "CMP(&D)",
            "CMP(&E)",
            "CMP(&H)",
            "CMP(&L)",
            "CMP(nullptr)",
            "CMP(&A)",
            "RNZ()",
            "POP(&B, &C)",
            "JNZ()",
            "JMP()",
            "CNZ()",
            "PUSH(&B, &C)",
            "ADI()",
            "RST(0x0000)",
            "RZ()",
            "RET()",
            "JZ()",
            "NIMP()",
            "CZ()",
            "CALL()",
            "ACI()",
            "RST(0x0008)",
            "RNC()",
            "POP(&D, &E)",
            "JNC()",
            "OUT()",
            "CNC()",
            "PUSH(&D, &E)",
            "SUI()",
            "RST(0x0010)",
            "RC()",
            "NIMP()",
            "JC()",
            "IN()",
            "CC()",
            "NIMP()",
            "SBI()",
            "RST(0x0018)",
            "RPO()",
            "POP(&H, &L)",
            "JPO()",
            "XTHL()",
            "CPO()",
            "PUSH(&H, &L)",
            "ANI()",
            "RST(0x0020)",
            "RPE()",
            "PCHL()",
            "JPE()",
            "XCHG()",
            "CPE()",
            "NIMP()",
            "XRI()",
            "RST(0x0028)",
            "RP()",
            "POP(nullptr, nullptr)",
            "JP()",
            "DI()",
            "CP()",
            "PUSH(nullptr, nullptr)",
            "ORI()",
            "RST(0x0030)",
            "RM()",
            "SPHL()",
            "JM()",
            "EI()",
            "CM()",
            "NIMP()",
            "CPI()",
            "RST(0x0038)"
    };
protected:
    void run() override;

private:
    uint8_t fetchInstruction();
    uint8_t fetchData(uint16_t address);
    void writeData(uint16_t address, uint8_t data);
    void reset();
    void checkInterrupts();
    void pushToShiftRegister(uint8_t data);
    void setShiftRegisterOffset(uint8_t data);
    uint8_t getShiftRegisterContent();

private:
    static uint8_t NIMP();
    uint8_t NOP();
    uint8_t LXI(uint8_t *rh, uint8_t *rl);
    uint8_t STAX(uint8_t rh, uint8_t rl);
    uint8_t INX(uint8_t *rh, uint8_t *rl);
    uint8_t INR(uint8_t *reg);
    uint8_t DCR(uint8_t *reg);
    uint8_t MVI(uint8_t *reg);
    uint8_t RLC();
    uint8_t DAD(const uint8_t *reg, const uint8_t *reg2);
    uint8_t LDA();
    uint8_t LDAX(uint8_t rh, uint8_t rl);
    uint8_t DCX(uint8_t *reg, uint8_t *reg2);
    uint8_t RRC();
    uint8_t RAL();
    uint8_t RAR();
    uint8_t SHLD();
    uint8_t DAA();
    uint8_t LHLD();
    uint8_t CMA();
    uint8_t STA();
    uint8_t STC();
    uint8_t CMC();
    uint8_t MOV(uint8_t *reg, const uint8_t *reg2);
    uint8_t HLT();
    uint8_t ADD(const uint8_t *reg);
    uint8_t ADC(const uint8_t *reg);
    uint8_t SUB(const uint8_t *reg);
    uint8_t SBB(const uint8_t *reg);
    uint8_t ANA(const uint8_t *reg);
    uint8_t XRA(const uint8_t *reg);
    uint8_t ORA(const uint8_t *reg);
    uint8_t CMP(const uint8_t *reg);
    uint8_t RNZ();
    uint8_t POP(uint8_t *rh, uint8_t *rl);
    uint8_t JNZ();
    uint8_t JMP();
    uint8_t CNZ();
    uint8_t PUSH(const uint8_t *rh, const uint8_t *rl);
    uint8_t ADI();
    uint8_t RST(uint16_t addr);
    uint8_t RZ();
    uint8_t RET();
    uint8_t JZ();
    uint8_t CZ();
    uint8_t CALL();
    uint8_t ACI();
    uint8_t RNC();
    uint8_t JNC();
    uint8_t OUT();
    uint8_t CNC();
    uint8_t SUI();
    uint8_t RC();
    uint8_t JC();
    uint8_t IN();
    uint8_t CC();
    uint8_t SBI();
    uint8_t RPO();
    uint8_t JPO();
    uint8_t XTHL();
    uint8_t CPO();
    uint8_t ANI();
    uint8_t PCHL();
    uint8_t JPE();
    uint8_t XCHG();
    uint8_t CPE();
    uint8_t XRI();
    uint8_t RP();
    uint8_t JP();
    uint8_t DI();
    uint8_t CP();
    uint8_t ORI();
    uint8_t RM();
    uint8_t RPE();
    uint8_t SPHL();
    uint8_t JM();
    uint8_t EI();
    uint8_t CM();
    uint8_t CPI();
};


#endif //EMU_INTEL8080_INTEL8080_H
