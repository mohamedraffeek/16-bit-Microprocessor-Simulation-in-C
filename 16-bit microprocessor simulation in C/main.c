#include <stdio.h>
#include <stdlib.h>

//define op-codes
#define MOVE 0
#define ADD  1
#define SUB  2
#define BRA  3
#define CMP  4
#define BEQ  5
#define BNE  6
#define EXG  7
#define STOP 15

int main()
{
    //assigning processing elements

    unsigned short int PC  = 0;
    unsigned short int D0  = 0;
    unsigned short int A0  = 0;
    unsigned short int CCR = 0;
    unsigned short int MAR;
    unsigned short int MBR;
    unsigned short int IR;
    unsigned short int operand;
    unsigned short int source;
    unsigned short int destination;
    unsigned short int opcode;
    unsigned short int amode;
    unsigned short int direction;
    unsigned short int memory[256];
    unsigned short int run = 1;

    //write your program in memory here
    memory[0] = 5;
    memory[1] = 9;      //MOVE #2,D0
    memory[2] = 21;
    memory[3] = 2;      //ADD  #2,D0
    memory[4] = 0;
    memory[5] = 45;     //MOVE D0,(20)
    memory[6] = 240;    //STOP

    //main loop

    while(run){
        //fetch the opcode
        MAR = PC;
        PC = PC + 1;
        MBR = memory[MAR];
        IR = MBR;
        opcode = IR;

        //fetch the operand
        MAR = PC;
        PC = PC + 1;
        MBR = memory[MAR];
        IR = MBR;
        operand = IR;

        //extract amode, direction, and instruction bits from opcode
        amode = opcode & 0x03;
        direction = (opcode & 0x04) >> 2;
        opcode = opcode >> 4;

        //use the amode to get the source operand
        switch(amode){
            case 0: {source = memory[operand];      break;}
            case 1: {source = operand;              break;}
            case 2: {source = memory[A0 + operand]; break;}
            case 3: {source = memory[PC + operand]; break;}
        }

        //now execute the instruction
        switch(opcode){
            case MOVE:  {if(direction == 0) destination = D0;
                        else               D0 = source;
                        if(D0 == 0) CCR = 1; else CCR = 0;
                        break;}

            case ADD:   {if(direction == 0){
                            destination = D0 + source;
                            if(destination == 0) CCR = 1; else CCR = 0;
                        }
                        else{
                            D0 = D0 + source;
                            if(D0 == 0) CCR = 1; else CCR = 0;
                        }
                        break;}

            case SUB:   {if(direction == 0){
                            destination = D0 - source;
                            if(destination == 0) CCR = 1; else CCR = 0;
                        }
                        else{
                            D0 = D0 - source;
                            if(D0 == 0) CCR = 1; else CCR = 0;
                        }
                        break;}

            case BRA:   {if(amode == 0) PC = operand;
                        if(amode == 1) PC = PC + operand;
                        break;}

            case CMP:   {MBR = D0 - source;
                        if(MBR == 0) CCR = 1;
                        else CCR = 0;
                        break;}

            case BEQ:   {if(CCR == 1)
                            {if(amode == 0) PC = operand;
                            if(amode == 1) PC = PC + operand;}
                        break;}

            case BNE:   {if(CCR != 1)
                            {if(amode == 0) PC = operand;
                            if(amode == 1) PC = PC + operand;}
                        break;}

            case EXG:   {MBR = D0;  D0 = A0;    A0 = MBR;   break;}

            case STOP:  {run = 0;   break;}
        }

        //save results in memory if the direction bit = 0
        if(direction == 0){
            switch(amode){
                case 0: {memory[operand] = destination;         break;}
                case 1: {                                       break;}
                case 2: {memory[A0 + operand] = destination;    break;}
                case 3: {memory[PC + operand] = destination;    break;}
            }
        }
    }
    //test results
    printf("%i", memory[45]);

    return 0;
}
