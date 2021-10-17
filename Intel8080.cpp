#include "Intel8080.h"

Intel8080::Intel8080()
{
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
        printf("error: Couldn't open %s\n", m_gameDir);    
        exit(1);    
    }    

    //Get the file size and read it into a memory buffer    
    fseek(f, 0L, SEEK_END);    
    int fsize = ftell(f);    
    fseek(f, 0L, SEEK_SET);    

    m_gameBuffer = (unsigned char*) malloc(fsize);

    
}