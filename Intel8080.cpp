#include "Intel8080.h"


Intel8080::Intel8080(char* gameDir)
{
    m_gameDir = gameDir;
    ResetState();
    GenerateBuffer();


}

void Intel8080::ResetState(){
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
    m_state.cc.z= 0;
    m_state.cc.s = 0;
    m_state.cc.p = 0;
    m_state.cc.pad = 0;
    m_state.cc.cy = 0;
    m_state.cc.ac = 0;

}


void Intel8080::GenerateBuffer(){

    FILE *f= fopen(m_gameDir, "rb"); 

    if (f==NULL)    
    {    
        std::cout << "error: Couldn't open " << m_gameDir << std::endl;    
    }    

    //Get the file size and read it into a memory buffer    
    fseek(f, 0L, SEEK_END);    
    int fsize = ftell(f);  
    m_gameBufferLen = fsize;  

    fseek(f, 0L, SEEK_SET);    

    m_gameBuffer = (unsigned char*) malloc(fsize);

    fread(m_gameBuffer, fsize, 1, f);    
    fclose(f);    



    
}

 void Intel8080::UnimplementedInstruction(unsigned char* instruction ){
    //pc will have advanced one, so undo that   
    printf("Error: Instruction $#%02x not yet implemented\n", m_gameBuffer[m_state.pc]);
    exit(1);   
 }

 int Intel8080::run(){

    while(m_state.pc < m_gameBufferLen){
    
        Emulate8080OpCode();

    }


 }
 int Intel8080::Emulate8080OpCode(){

    unsigned char* opcode = &m_state.memory[m_state.pc];

    switch(*opcode){

        case 0x00: printf("NOP"); break;
        case 0x01:{ //LXI B,D16         B <- byte 3, C <- byte 2
            m_state.b = opcode[2];
            m_state.c = opcode[1];
            m_state.pc +=2;
            break;

        }
        case 0x02:{
            m_state.memory[(m_state.b << 8) | (m_state.c) ] = m_state.a;


        } printf("STAX B"); break;
        case 0x03: printf("INX  B"); break;
        case 0x04: printf("INR  B"); break;
        case 0x05: printf("DCR  B"); break;
        case 0x06: printf("MVI  B,#$%02x",code[1]); opbyte = 2; break;
        case 0x07: printf("RLC"); break;

        default:{ 
            UnimplementedInstruction(opcode[0]);
        }
        
    }






 }