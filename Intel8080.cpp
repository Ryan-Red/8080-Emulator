#include "Intel8080.h"

Intel8080::Intel8080(char *gameDir)
{
    m_gameDir = gameDir;
    ResetState();
    GenerateBuffer();
}

uint8_t Intel8080::Parity(uint8_t val)
{

    int counter;
    int i = 0;

    for (i = 0; i < 16; i++)
    {
        counter += (val >> i) & 0x1;
    }

    return (counter % 2) == 0 ? 1 : 0;
}

void Intel8080::ResetState()
{
    m_state.a = 0;
    m_state.b = 0;
    m_state.c = 0;
    m_state.d = 0;
    m_state.e = 0;
    m_state.h = 0;
    m_state.l = 0;
    m_state.sp = 0;
    m_state.pc = 0;
    m_state.memory = NULL; /////////////////To change when I know the initial Memory position

    m_state.int_enable = 0; // To be changed later on when starting the emulator ig.

    //Reset the ConditionCodes
    m_state.cc.z = 0;
    m_state.cc.s = 0;
    m_state.cc.p = 0;
    m_state.cc.pad = 0;
    m_state.cc.cy = 0;
    m_state.cc.ac = 0;
}

void Intel8080::GenerateBuffer()
{

    FILE *f = fopen(m_gameDir, "rb");

    if (f == NULL)
    {
        std::cout << "error: Couldn't open " << m_gameDir << std::endl;
    }

    //Get the file size and read it into a memory buffer
    fseek(f, 0L, SEEK_END);
    int fsize = ftell(f);
    m_gameBufferLen = fsize;

    fseek(f, 0L, SEEK_SET);

    m_gameBuffer = (unsigned char *)malloc(fsize);

    fread(m_gameBuffer, fsize, 1, f);
    fclose(f);
}

void Intel8080::UnimplementedInstruction(unsigned char *instruction)
{
    //pc will have advanced one, so undo that
    printf("Error: Instruction $#%02x not yet implemented\n", m_gameBuffer[m_state.pc]);
    exit(1);
}

int Intel8080::run()
{

    while (m_state.pc < m_gameBufferLen)
    {

        Emulate8080OpCode();
    }
}

int Intel8080::Emulate8080OpCode()
{

    unsigned char *opcode = &m_state.memory[m_state.pc];

    switch (*opcode)
    {

    case 0x00:
        printf("NOP");
        break;
    case 0x01:
    { //LXI B,D16         B <- byte 3, C <- byte 2
        m_state.b = opcode[2];
        m_state.c = opcode[1];
        m_state.pc += 2;
        break;
    }
    case 0x02:
    {
        m_state.memory[(m_state.b << 8) | (m_state.c)] = m_state.a;
        break;
    }
    case 0x03:
        printf("INX  B");
        break;
    case 0x04:
        printf("INR  B");
        break;
    case 0x05:
        m_state.b = m_state.b - 1;

        m_state.cc.z = (m_state.b & 0xFF) == 0;  //Zero flag
        m_state.cc.s = (m_state.b & 0x80) == 1;  // Sign flag
        m_state.cc.ac = (m_state.b < 0xF) == 1;  //Aux Carry Flag
        m_state.cc.p = Parity(m_state.b & 0xFF); //Parity

        break;

    case 0x06:
        m_state.b = opcode[1];
        break;

    case 0x09:
        uint8_t temp = ((m_state.h << 8) | (m_state.l)) + ((m_state.b << 8) | (m_state.c));
        m_state.h = (temp >> 8) & 0xFF;
        m_state.l = temp & 0xFF;

        m_state.cc.cy = (temp > 0xFF); //Carry flag
        break;

    case 0x0d:
        m_state.c = m_state.c - 1;

        m_state.cc.z = (m_state.c & 0xFF) == 0;  //Zero flag
        m_state.cc.s = (m_state.c & 0x80) == 1;  // Sign flag
        m_state.cc.ac = (m_state.c < 0xF) == 1;  //Aux Carry Flag
        m_state.cc.p = Parity(m_state.c & 0xFF); //Parity
        break;

    case 0x0e:
        m_state.c = opcode[1];
        break;

    case 0x0f:
        int8_t tempA = m_state.a;
        int8_t prev0 = tempA & 0x01;

        m_state.a = (m_state.a >> 1) & ((prev0 << 7) | 0x7F);
        m_state.cc.cy = prev0;
        break;

    case 0x11:
        m_state.d = opcode[2];
        m_state.e = opcode[1];
        m_state.pc += 2;
        break;

    default:
    {
        UnimplementedInstruction(opcode);
    }
    }
    m_state.pc += 1;
}

int main()
{
    int wow = Parity(26);
    std::cout << wow;
    return 0;
}