#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char memoria[154];
unsigned int mbr;
unsigned int mar;
unsigned char ir;
unsigned char ro0;
unsigned char ro1;
unsigned char imm;
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

       //0000 0011 0000 0100 0000 0000 0000 0000
       ro1 = (mbr & 0x001c0000) >> 18;
    } else if(ir == 0xB)
   {
       //NOT
       //xxxx xxxx 001x xxxx xxxx xxxx xxxx xxxx
       //0000 0000 1110 0000 0000 0000 0000 0000
       //   0    0    e    0    0    0    0    0
       ro0 = (mbr & 0x00e00000) >> 21;
   } else if(ir >=0xC && ir <= 0x12)
   {
        //JE, JE JL JLE JG JGE JMP
        //xxxx xxxx xxxy yyyy yyyy yyyy yyyy yyyy
        //0000 0000 0001 1111 1111 1111 1111 1111
        //   0    0    1    f    f    f    f    f

        mar = mbr & 0x001fffff;
   } else if(ir >= 0x13 && ir <= 0x14)
   {
       //LD, ST
       //xxxx xxxx yyyx xxxx xxxx xxxx xxxx xxxx
       //0000 0000 1110 0000 0000 0000 0000 0000
       //   0    0    e    0    0    0    0    0

       ro0 = (mbr & 0x00E00000) >> 21;

       //xxxx xxxx xxxy yyyy yyyy yyyy yyyy yyyy
       //0000 0000 0001 1111 1111 1111 1111 1111
       //   0    0    1    f    f    f    f    f

       mar = mbr & 0x001fffff;




   } else if(ir >= 0x15 && ir <= 0x1B)
   {
       //MOVI
       //xxxx xxxx yyyx xxxx xxxx xxxx xxxx xxxx
       //0000 0000 1110 0000 0000 0000 0000 0000
       //   0    0    e    0    0    0    0    0

       ro0 = (mbr & 0x00E00000) >> 21;

       //xxxx xxxx xxxy yyyy yyyy yyyy yyyy yyyy
       //0000 0000 0001 1111 1111 1111 1111 1111
       //   0    0    1    f    f    f    f    f

       imm = mbr & 0x001fffff;
   }
}
void executa()
{
    switch (ir) {
        case 0x00:
            //hlt
            break;
        case 0x01:
            //nop
            pc += 4;
            break;
        case 0x02:
            //add
            reg[ro0] = reg[ro0] + reg[ro1];
            pc += 4;
            break;
        case 0x03:
            //sub
            reg[ro0] = reg[ro0] - reg[ro1];
            pc += 4;
            break;
        case 0x04:
            //mull
            reg[ro0] = reg[ro0] * reg[ro1];
            pc += 4;
            break;
        case 0x05:
            //div
            reg[ro0] = reg[ro0] / reg[ro1];
            pc += 4;
            break;
        case 0x06:
            //cmp
            if(reg[ro0] == reg[ro1])
            {
                e = 1;
                l = 0;
                g = 0;
            } else if(reg[ro0] < reg[ro1])
            {
                e = 0;
                l = 1;
                g = 0;
            } else{
                e = 0;
                l = 0;
                g = 0;
            }
            pc += 4;
            break;
        case 0x07:
            //movr
            reg[ro0] = reg[ro1];
            pc += 4;
            break;
        case 0x08:
            //and
            reg[ro0] = reg[ro0] & reg[ro1];
            pc += 4;
            break;
        case 0x09:
            //or
            reg[ro0] = reg[ro0] | reg[ro1];
            pc += 4;
            break;
        case 0x0A:
            //xor
            reg[ro0] = reg[ro0] ^ reg[ro1];
            pc += 4;
            break;
        case 0x0B:
            //not
            reg[ro0] = !reg[ro0];
            pc += 4;
            break;
        case 0x0C:
            //je
            if(e == 1)
            {
                pc = mar;
            }
            break;
        case 0x0D:
            //jne
            if(e == 0)
            {
                pc = mar;
            }
            break;
        case 0x0E:
            //jl
            if(l == 1)
            {
                pc = mar;
            }
            break;
        case 0x0F:
            //jle
            if(l == 1 || e == 1)
            {
                pc = mar;
            }
            break;
        case 0x10:
            //jg
            if(g == 1)
            {
                pc = mar;
            }
            break;
        case 0x11:
            //jge
            if(g == 1 || e == 1)
            {
                pc = mar;
            }
            break;
        case 0x12:
            //jmp
            pc = mar;
            break;
        case 0x13:
            //ld
            reg[ro0] = memoria[mar++];
            for(int i = 1; i < 4; i++)
            {
                reg[ro0] = reg[ro0] << 8;
                reg[ro0] = reg[ro0] | memoria[mar++];
            }

            pc += 4;
            break;
        case 0x14:
            //st
            //aaaa bbbb cccc dddd eeee ffff gggg hhhh
            //memoria[x++] = aaaa bbbb
            //memoria[x++] = cccc dddd
            //memoria[x++] = eeee ffff
            //memoria[x] = gggg

            memoria[mar++] = reg[ro0] >> 24;

            //xxxx xxxx yyyy yyyy xxxx xxxx xxxx xxxx
            //0000 0000 1111 1111 0000 0000 0000 0000
            //   0    0    f    f    0    0     0   0
            memoria[mar++] = (reg[ro0] & 0x00ff0000) >> 16;

            //xxxx xxxx xxxx xxxx yyyy yyyy xxxx xxxx
            //0000 0000 0000 0000 1111 1111 0000 0000
            //   0    0    0    0    f    f     0   0
            memoria[mar++] = (reg[ro0] & 0x0000ff00) >> 16;

            //xxxx xxxx xxxx xxxx xxxx xxxx yyyy yyyy
            //0000 0000 0000 0000 0000 0000 1111 1111
            //   0    0    0    0    f    f     0   0
            memoria[mar] = reg[ro0] & 0x000000ff;

            pc += 4;

            break;
        case 0x15:
            //movi
            reg[ro0] = imm;

            pc += 4;
            break;
        case 0x16:
            //addi
            reg[ro0] = reg[ro0] + imm;

            pc += 4;
            break;
        case 0x17:
            //subi
            reg[ro0] = reg[ro0] - imm;

            pc += 4;
            break;
        case 0x18:
            //muli
            reg[ro0] = reg[ro0] * imm;

            pc += 4;
            break;
        case 0x19:
            //divi
            reg[ro0] = reg[ro0] / imm;

            pc += 4;
            break;
        case 0x1A:
            //lsh
            reg[ro0] = reg[ro0] << imm;
            pc += 4;
            break;
        case 0x1B:
            //rsh
            reg[ro0] = reg[ro0] << imm;
            pc += 4;
            break;
    }
}

void lerTexto()
{
    FILE *arq;
    char *pl;
    char str[50];
    int count = 0;

    int inicio;
    char tipo;
    unsigned int palavra;
    unsigned char opcode;
    unsigned int reg1;
    unsigned int reg2;
    unsigned int menOuImm;

    arq = fopen("programa.txt", "r");

    if (NULL == arq) {
        printf("Arquivo programa.txt nao encontrado \n");
        exit(1);
    } else{

       while (fgets(str, 50, arq) != NULL) {
           pl = strtok(str, ";");
           while(count < 3)
           {
               printf("token:%s\n",pl);
               if(count == 0)
               {
                   //inicio
                   inicio = (int) strtol(pl,NULL,16);
                   printf("%d --- %x\n",inicio,inicio);
               } else if(count == 1)
               {
                   //tipo
                   tipo = *pl;
                   printf("%c\n",tipo);
               } else{
                   if(tipo == 'i')
                   {
                       pl = strtok(pl, "r, ");
                       //palavra
                       if(strcmp(pl,"nop") == 0)
                       {
                           opcode = 0x01;
                       } else if(strcmp(pl,"ld") == 0)
                       {
                           opcode = 0x13;
                           pl = strtok(NULL,"r, ");
                           reg1 = (int) strtol(pl,NULL, 16);
                           pl = strtok(NULL,"r, ");
                           menOuImm = (int) strtol(pl,NULL, 16);
                       }
                       return;
                   } else{
                       palavra = (int) strtol(pl,NULL,16);
                       printf("%d --- %x\n",palavra,palavra);
                   }
               }
               pl = strtok(NULL,";");
               count++;
           }
           count = 0;
           printf("\n");
        }
    }
    fclose(arq);
}

int criar_palavra(unsigned int palavra, unsigned char opcode, unsigned int reg1, unsigned int reg2, unsigned int menOuImm)
{
    int novaPalavra;

    if(ir >= 0x2 && ir <= 0xA)
    {
        //ADD,SUB,MUL,DIV,CMP,AND,OR,XOR

    } else if(ir == 0xB)
    {
        //NOT

    } else if(ir >=0xC && ir <= 0x12)
    {
        //JE, JE JL JLE JG JGE JMP

    } else if(ir >= 0x13 && ir <= 0x14)
    {
        //LD, ST
    } else if(ir >= 0x15 && ir <= 0x1B)
    {
        //MOVI

    }

    return palavra;
}

int main() {

  //  lerTexto();
//
//  pc = 0;
//  memoria[0] = 0x13;
//  memoria[1] = 0x00;
//  memoria[2] = 0x00;
//  memoria[3] = 0x10;
//
//  memoria[4] = 0x13;
//  memoria[5] = 0x20;
//  memoria[6] = 0x00;
//  memoria[7] = 0x14;
//
//  memoria[0x08] = 0x16;
//  memoria[0x09] = 0x00;
//  memoria[0x0a] = 0x00;
//  memoria[0x0b] = 0x14;
//
//  memoria[0x0c] = 0x00;
//  memoria[0x0d] = 0x00;
//  memoria[0x0e] = 0x00;
//  memoria[0x0f] = 0x00;
//
//  memoria[0x10] = 0x00;
//  memoria[0x11] = 0x00;
//  memoria[0x12] = 0x00;
//  memoria[0x13] = 0x0f;
//
//  memoria[0x14] = 0x00;
//  memoria[0x15] = 0x00;
//  memoria[0x16] = 0x00;
//  memoria[0x17] = 0x08;
//
//  busca();
//  decodifica();
//  while(ir != 0x00)
//  {
//
//      executa();
//      printf("mbr -- %x\n", mbr);
//      printf("mar -- %x\n", mar);
//      printf("ir -- %x\n", ir);
//      printf("ro0 -- %x\n", ro0);
//      printf("ro1 -- %x\n", ro1);
//      printf("imm -- %x\n", imm);
//      printf("pc -- %x\n", pc);
//      printf("e -- %x\n", e);
//      printf("l -- %x\n", l);
//      printf("g -- %x\n", g);
//      printf("r0 -- %x\n", reg[0]);
//      printf("r1 -- %x\n", reg[1]);
//      printf("r2 -- %x\n", reg[2]);
//      printf("r3 -- %x\n", reg[3]);
//      printf("r4 -- %x\n", reg[4]);
//      printf("r5 -- %x\n", reg[5]);
//      printf("r6 -- %x\n", reg[6]);
//      printf("r7 -- %x\n", reg[7]);
//      printf("------------------------\n");
//      busca();
//      decodifica();
//  }

    lerTexto();
    return 0;
}

/*
 * cmp r3, r4 -> e = igual; l -> menor; -> g -> maior
 *
 *
 */
