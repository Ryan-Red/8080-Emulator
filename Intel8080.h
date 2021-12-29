#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "decompiler.c"

typedef struct ConditionCodes //Condition flags of the 8080. Gets changed after some opcodes
{
    uint8_t z : 1;
    uint8_t s : 1;
    uint8_t p : 1;
    uint8_t cy : 1;
    uint8_t ac : 1;
    uint8_t pad : 3;
} ConditionCodes;

typedef struct State8080
{ //All registers, pointers etc in the 8080

    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
    uint16_t sp;
    uint16_t pc;
    uint8_t *memory;
    ConditionCodes cc;
    uint8_t int_enable;

} State8080;

class Intel8080
{
private:
    State8080 m_state; //m_state of emulator

    char *m_gameDir; //Directory where the game file is located

    unsigned char *m_gameBuffer; //buffer that has all the of the game buffer.
    int m_gameBufferLen;

    void UnimplementedInstruction(unsigned char *instruction); // Instruction not implemented, use this to remind myself what still needs to be done

    void GenerateBuffer(); //Generate the game buffer to be used to emulate the game

    void ResetState(); // reinitialize the m_state of the 8080

public:
    Intel8080(char *gameDir);
    ~Intel8080();

    int Emulate8080OpCode(); //Run the emulator

    int run();
    uint8_t Parity(uint8_t val);
};

Intel8080::~Intel8080()
{
}
