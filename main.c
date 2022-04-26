#include <stdio.h>

unsigned char memoria[154];
unsigned int mbr;
unsigned int mar;
unsigned char ir;
unsigned char ro0;
unsigned char ro1;
unsigned int pc;
unsigned char e;
unsigned char l;
unsigned char g;
unsigned int reg[8];

void busca()
{
    mar = pc;
    mbr = memoria[mar++];
 for(int i = 1; i < 4; i++)
 {
     mbr = mbr << 8;
     mbr = mbr | memoria[mar++];
 }
}

void decodifica()
{
    ir = mbr >> 24;
   if(ir >= 0x2 && ir <= 0xA)
   {
       //ADD,SUB,MUL,DIV,CMP,AND,OR,XOR
       //xxxx xxxx 001x xxxx xxxx xxxx xxxx xxxx
       //0000 0000 1110 0000 0000 0000 0000 0000
       //   0    0    e    0    0    0    0    0
       ro0 = (mbr & 0x00e00000) >> 21;
       //xxxx xxxx xxx0 01xx xxxx xxxx xxxx xxxx
       //0000 0000 0001 1100 0000 0000 0000 0000
       //   0    0    1    C    0    0    0    0
       ro1 = (mbr & 0x001c0000) >> 16;
    } else if(ir == 0xB)
   {
       printf("dsa");
       //NOT
       //xxxx xxxx 001x xxxx xxxx xxxx xxxx xxxx
       //0000 0000 1110 0000 0000 0000 0000 0000
       //   0    0    e    0    0    0    0    0
       ro0 = (mbr & 0x00e00000) >> 21;
   } else if(ir <=0xC && ir <= 0x12)
   {

   }
}

int main() {
    pc = 0;
    memoria[0] = 0x02;
    memoria[1] = 0xff;
    memoria[2] = 0;
    memoria[3] = 0x1f;
    busca();
    decodifica();
    printf("%x", ro1);
    //0000 0010 1111 0000 0001 1111

    return 0;
}

/*
 * cmp r3, r4 -> e = igual; l -> menor; -> g -> maior
 *
 *
 *
 */
