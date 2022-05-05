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
            } else{
                pc+=4;
            }
            break;
        case 0x0D:
            //jne
            if(e == 0)
            {
                pc = mar;
            }else{
                pc+=4;
            }
            break;
        case 0x0E:
            //jl
            if(l == 1)
            {
                pc = mar;
            }else{
                pc+=4;
            }
            break;
        case 0x0F:
            //jle
            if(l == 1 || e == 1)
            {
                pc = mar;
            }else{
                pc+=4;
            }
            break;
        case 0x10:
            //jg
            if(g == 1)
            {
                pc = mar;
            }else{
                pc+=4;
            }
            break;
        case 0x11:
            //jge
            if(g == 1 || e == 1)
            {
                pc = mar;
            }else{
                pc+=4;
            }
            break;
        case 0x12:
            //jmp
            pc = mar;
            break;
        case 0x13:
            //ld
            mbr = memoria[mar++];
            for(int i = 1; i < 4; i++)
            {
                mbr = mbr << 8;
                mbr = mbr | memoria[mar++];
            }
            reg[ro0] = mbr;
            pc += 4;
            break;
        case 0x14:
            //st
            //aaaa bbbb cccc dddd eeee ffff gggg hhhh
            //memoria[novaPalvra++] = aaaa bbbb
            //memoria[novaPalvra++] = cccc dddd
            //memoria[novaPalvra++] = eeee ffff
            //memoria[novaPalvra] = gggg
            mbr = reg[ro0];
            memoria[mar++] = mbr >> 24;

            //xxxx xxxx yyyy yyyy xxxx xxxx xxxx xxxx
            //0000 0000 1111 1111 0000 0000 0000 0000
            //   0    0    f    f    0    0     0   0
            memoria[mar++] = (mbr & 0x00ff0000) >> 16;

            //xxxx xxxx xxxx xxxx yyyy yyyy xxxx xxxx
            //0000 0000 0000 0000 1111 1111 0000 0000
            //   0    0    0    0    f    f     0   0
            memoria[mar++] = (mbr & 0x0000ff00) >> 16;

            //xxxx xxxx xxxx xxxx xxxx xxxx yyyy yyyy
            //0000 0000 0000 0000 0000 0000 1111 1111
            //   0    0    0    0    f    f     0   0
            memoria[mar] = mbr & 0x000000ff;

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
            reg[ro0] = reg[ro0] >> imm;
            pc += 4;
            break;
    }
}


int criar_palavra(char mnemonico[], unsigned int reg1, unsigned int reg2, unsigned int menOuImm, int inicio)
{
    int novaPalavra;

    if(strcmp(mnemonico,"ld")== 0){
        novaPalavra = 0x13;
        novaPalavra = (novaPalavra << 3) | reg1;
        novaPalavra = (novaPalavra << 21) | menOuImm;
    }else if(strcmp(mnemonico,"st")== 0){
        novaPalavra = 0x14;
        novaPalavra = (novaPalavra << 3) | reg1;
        novaPalavra = (novaPalavra << 21) | menOuImm;
    }else if(strcmp(mnemonico,"add")== 0){
        novaPalavra = 0x02;
        novaPalavra = (novaPalavra << 3) |reg1;
        novaPalavra = (novaPalavra << 3) | reg2;
        novaPalavra =  novaPalavra << 18;
    }else if(strcmp(mnemonico,"sub")== 0) {
        novaPalavra = 0x03;
        novaPalavra = (novaPalavra << 3) | reg1;
        novaPalavra = (novaPalavra << 3) | reg2;
        novaPalavra = novaPalavra << 18;
    }else if(strcmp(mnemonico,"mul")== 0) {
        novaPalavra = 0x04;
        novaPalavra = (novaPalavra << 3) | reg1;
        novaPalavra = (novaPalavra << 3) | reg2;
        novaPalavra = novaPalavra << 18;
    }else if(strcmp(mnemonico,"div")== 0) {
        novaPalavra = 0x05;
        novaPalavra = (novaPalavra << 3) | reg1;
        novaPalavra = (novaPalavra << 3) | reg2;
        novaPalavra = novaPalavra << 18;
    }else if(strcmp(mnemonico,"cmp")== 0) {
        novaPalavra = 0x06;
        novaPalavra = (novaPalavra << 3) | reg1;
        novaPalavra = (novaPalavra << 3) | reg2;
        novaPalavra = novaPalavra << 18;
    }else if(strcmp(mnemonico,"movr")== 0) {
        novaPalavra = 0x07;
        novaPalavra = (novaPalavra << 3) | reg1;
        novaPalavra = (novaPalavra << 3) | reg2;
        novaPalavra = novaPalavra << 18;
    }
    else if(strcmp(mnemonico,"and")== 0) {
        novaPalavra = 0x08;
        novaPalavra = (novaPalavra << 3) | reg1;
        novaPalavra = (novaPalavra << 3) | reg2;
        novaPalavra = novaPalavra << 18;
    }else if(strcmp(mnemonico,"or")== 0) {
        novaPalavra = 0x09;
        novaPalavra = (novaPalavra << 3) | reg1;
        novaPalavra = (novaPalavra << 3) | reg2;
        novaPalavra = novaPalavra << 18;
    }else if(strcmp(mnemonico,"xor")== 0) {
        novaPalavra = 0x0A;
        novaPalavra = (novaPalavra << 3) | ro0;
        novaPalavra = (novaPalavra << 3) | novaPalavra;
        novaPalavra = novaPalavra << 18;
    }else if(strcmp(mnemonico,"not")== 0) {
        novaPalavra = 0x0B;
        novaPalavra = (novaPalavra << 3)|ro0;
        novaPalavra = novaPalavra << 21;
    }else if(strcmp(mnemonico,"je")== 0) {
        novaPalavra = 0x0C;
        novaPalavra = (novaPalavra << 24) | menOuImm;
    }else if(strcmp(mnemonico,"jne")== 0) {
        novaPalavra = 0x0D;
        novaPalavra = (novaPalavra << 24)| menOuImm;
    }else if(strcmp(mnemonico,"jl")== 0) {
        novaPalavra = 0x0E;
        novaPalavra = (novaPalavra << 24)| menOuImm;
    }else if(strcmp(mnemonico,"jg")== 0) {
        novaPalavra = 0x0F;
        novaPalavra = (novaPalavra << 24)| menOuImm;
    }else if(strcmp(mnemonico,"jle")== 0) {
        novaPalavra = 0x10;
        novaPalavra = (novaPalavra << 24)| menOuImm;
    }else if(strcmp(mnemonico,"jge")== 0) {
        novaPalavra = 0x11;
        novaPalavra = (novaPalavra << 24)| menOuImm;
    }else if(strcmp(mnemonico,"jmp")== 0) {
        novaPalavra = 0x12;
        novaPalavra = (novaPalavra << 24)| menOuImm;
    }else if(strcmp(mnemonico,"movi")== 0) {
        novaPalavra = 0x15;
        novaPalavra = (novaPalavra << 3) | reg1;
        novaPalavra = (novaPalavra << 21) | menOuImm;
    }else if(strcmp(mnemonico,"addi")== 0) {
        novaPalavra = 0x16;
        novaPalavra = (novaPalavra << 3) | reg1;
        novaPalavra = (novaPalavra << 21) | menOuImm;
    }else if(strcmp(mnemonico,"subi")== 0) {
        novaPalavra = 0x17;
        novaPalavra = (novaPalavra << 3) | reg1;
        novaPalavra = (novaPalavra << 21) | menOuImm;
    }else if(strcmp(mnemonico,"muli")== 0) {
        novaPalavra = 0x18;
        novaPalavra = (novaPalavra << 3) | reg1;
        novaPalavra = (novaPalavra << 21) | menOuImm;
    }else if(strcmp(mnemonico,"divi")== 0) {
        novaPalavra = 0x19;
        novaPalavra = (novaPalavra << 3) | reg1;
        novaPalavra = (novaPalavra << 21) | menOuImm;
    }else if(strcmp(mnemonico,"lsh")== 0) {
        novaPalavra = 0x1A;
        novaPalavra = (novaPalavra << 3) | reg1;
        novaPalavra = (novaPalavra << 21) | menOuImm;
    }else if(strcmp(mnemonico,"rsh")== 0) {
        novaPalavra = 0x1B;
        novaPalavra = (novaPalavra << 3) | reg1;
        novaPalavra = (novaPalavra << 21) | menOuImm;
    }else if(strcmp(mnemonico,"nop")== 0) {
        novaPalavra = 0x01;
        novaPalavra = novaPalavra << 24;
    }else{
        novaPalavra  = 0;
    }

    printf("%x\n", novaPalavra);
    memoria[inicio++] = novaPalavra >> 24;
    memoria[inicio++] = (novaPalavra & 0x00ff0000) >> 16;
    memoria[inicio++] = (novaPalavra & 0x0000ff00) >> 16;
    memoria[inicio] = novaPalavra & 0x000000ff;

    return novaPalavra;
}


void lerTexto()
{
    FILE *arq;
    char *pl;
    char str[50];
    char mnemonico[10];
    int count = 0;

    int inicio;
    char tipo;
    unsigned int palavra;
    unsigned int reg1;
    unsigned int reg2;
    unsigned int menOuImm;

    arq = fopen("programa.txt", "r");

    if (NULL == arq) {
        printf("Arquivo programa.txt nao encontrado \n");
    } else{
       while (fgets(str, 50, arq) != NULL) {
           pl = strtok(str, ";");
           while(count < 3)
           {
               if(count == 0)
               {
                   //inicio
                   inicio = (int) strtol(pl,NULL,16);
               } else if(count == 1)
               {
                   //tipo
                   tipo = *pl;
               } else{
                   if(tipo == 'i')
                   {
                       pl = strtok(pl,", ");
                      if(strcmp(pl, "add") == 0 || strcmp(pl, "sub") == 0 ||
                            strcmp(pl, "mul") == 0 || strcmp(pl, "div") == 0 ||
                            strcmp(pl, "cmp") == 0 || strcmp(pl, "movr") == 0 ||
                            strcmp(pl, "and") == 0 || strcmp(pl, "or") == 0 ||
                            strcmp(pl, "xor") == 0)
                      {
                          //Mnemônico
                          strcpy(mnemonico,pl);
                          //ro1
                          pl = strtok(NULL,"r, ");
                          reg1 = (int) strtol(pl, NULL, 16);
                          //ro2
                          pl = strtok(NULL,"r, ");
                          reg2 = (int) strtol(pl, NULL, 16);

                          criar_palavra(mnemonico,reg1,reg2,0,inicio);

                      } else if(strcmp(pl, "not") == 0)
                      {
                          //Mnemônico
                          strcpy(mnemonico,pl);
                          //ro1
                          pl = strtok(NULL,"r, ");
                          reg1 = (int) strtol(pl, NULL, 16);
                          criar_palavra(mnemonico,reg1,0x0,0x0,inicio);
                      } else if(strcmp(pl, "je") == 0 || strcmp(pl, "jne") == 0 ||
                                strcmp(pl, "jl") == 0 || strcmp(pl, "jle") == 0 ||
                                strcmp(pl, "jg") == 0 || strcmp(pl, "jge") == 0 ||
                                strcmp(pl, "jmp") == 0)
                      {
                          //Mnemônico
                          strcpy(mnemonico,pl);

                          pl = strtok(NULL,", ");

                          //memoria
                          menOuImm = (int) strtol(pl, NULL, 16);
                         palavra = criar_palavra(mnemonico,0x00,0x00,menOuImm,inicio);
                      } else if(strcmp(pl, "ld") == 0 || strcmp(pl,"st") == 0)
                      {
                          //Mnemônico
                          strcpy(mnemonico,pl);


                          pl = strtok(NULL, "r, ");

                          //reg1

                          reg1 = (int) strtol(pl,NULL,16);


                          //memória
                          pl = strtok(NULL, ", ");
                          menOuImm = (int) strtol(pl, NULL, 16);

                         palavra = criar_palavra(mnemonico,reg1,0x00,menOuImm,inicio);

                      }else if(strcmp(pl, "addi") == 0 || strcmp(pl, "subi") == 0 ||
                                strcmp(pl, "muli") == 0 || strcmp(pl, "divi") == 0 ||
                                strcmp(pl, "movi") == 0 ||strcmp(pl, "lsh") == 0 ||
                                strcmp(pl, "rsh") == 0)
                      {
                          //Mnemônico
                          strcpy(mnemonico, pl);

                          //reg1
                          pl = strtok(NULL,"r, ");
                          reg1 = (int) strtol(pl, NULL, 16);

                          //imm
                          pl = strtok(NULL,"r, ");
                          menOuImm = (int) strtol(pl,NULL,16);

                          palavra = criar_palavra(mnemonico,reg1,0x00,menOuImm,inicio);
                      }else if(strcmp(pl, "nop") == 0 || strcmp(pl, "hlt") == 0)
                      {
                          strcpy(mnemonico,pl);
                          palavra = criar_palavra(mnemonico,0x00,0x00,0x00,inicio);
                      }
                   } else{
                       palavra = (int) strtol(pl,NULL,16);
                       memoria[inicio++] = palavra >> 24;
                       memoria[inicio++] = (palavra & 0x00ff0000) >> 16;
                       memoria[inicio++] = (palavra & 0x0000ff00) >> 16;
                       memoria[inicio] = palavra & 0x000000ff;
                   }
                   //adicionar palavra

               }

               pl = strtok(NULL,";");

               count++;
           }
           count = 0;
        }
    }
    fclose(arq);
}

int main() {

  lerTexto();
  pc = 0;

  busca();
  decodifica();
  while(ir != 0x00)
  {
      executa();
      busca();
      decodifica();
  }

    printf("%x",memoria[0x7d]);
    return 0;
}

/*
 * cmp r3, r4 -> e = igual; l -> menor; -> g -> maior
 *
 *
 */
