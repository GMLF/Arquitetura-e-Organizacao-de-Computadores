#ifndef MIPS_EMULATOR_OPERATIONS_H
#define MIPS_EMULATOR_OPERATIONS_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#define RAM_SIZE 256

//Verificando Parâmetros
void VerificaParametros(FILE *entrada, FILE *saida_op, FILE *saida_reg,int argc);

// Logicas e aritimeticas
void add (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]);
void addi (uint8_t dest, uint8_t src, int16_t imm, int reg[]);
void addu (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]);
void addiu (uint8_t dest, uint8_t src, uint16_t imm, int reg[]);
void sub (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]);
void mult (uint8_t src_1, uint8_t src_2, int reg[]);
void divv (uint8_t src_1, uint8_t src_2, int reg[]);
void mfhi (uint8_t dest, int reg[]);
void mflo (uint8_t dest, int reg[]);
void and (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]);
void or (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]);
void xor (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]);
void nor (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]);
void slt (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]);
void sll (uint8_t dest, uint8_t src, uint8_t shamt, int reg[]);
void srl (uint8_t dest, uint8_t src, uint8_t shamt, int reg[]);
void sra (uint8_t dest, uint8_t src, uint8_t shamt, int reg[]);
void andi (uint8_t dest, uint8_t src, int16_t imm, int reg[]);
void ori (uint8_t dest, uint8_t src, int16_t imm, int reg[]);
void xori (uint8_t dest, uint8_t src, int16_t imm, int reg[]);

// Sistema
int syscall(int pc, int reg[], char string_data[]);

// Load e Store
bool lw(uint8_t dest, int16_t offset, uint8_t src, int reg[], int stack[]);
bool lb(uint8_t dest, int16_t offset, uint8_t src, int reg[], int stack[]);
bool lbu(uint8_t dest, int16_t offset, uint8_t src, int reg[], int stack[]);
void lui (uint8_t dest, uint8_t src, int16_t imm, int reg[]);
bool sw(uint8_t dest, int16_t offset, uint8_t src, int reg[], int stack[]);
bool sb(uint8_t dest, int16_t offset, uint8_t src, int reg[], int stack[]);

// Condicionais e Saltos Condicionais
int beq(int pc, uint8_t src1, uint8_t src2, int16_t offset, int reg[]);
int bne(int pc, uint8_t src1, uint8_t src2, int16_t offset, int reg[]);
int blez(int pc, uint8_t src, int16_t offset, int reg[]);
int bgtz(int pc, uint8_t src, int16_t offset, int reg[]);
void slti (uint8_t dest, uint8_t src, int16_t imm, int reg[]);
void sltiu (uint8_t dest, uint8_t src, uint16_t imm, int reg[]);

// Saltos incondicionais
int j(uint32_t target);
int jr(uint8_t src, int reg[]);
int jal(int pc, uint32_t target, int reg[]);

//Verificando Parâmetros
void VerificaParametros(FILE *entrada, FILE *saida_op, FILE *saida_reg,int argc){
  if(argc != 4) {
      printf("Parametros fornecidos sao invalidos -> [nome_do_programa arquivo_entrada.bin arquivo_saida_op.txt arquivo_saida_reg.txt]");
      exit(1);
  }//if

  if(entrada == NULL || saida_op == NULL || saida_reg ==NULL) {
      printf("Erro ao abrir um dos arquivos\n");
      exit(1);
  }//if
}//VerificaAberturaArquivos


// Logicas e aritimeticas

void add (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]) {
    reg[dest] = reg[src_1] + reg[src_2];
}

void addi (uint8_t dest, uint8_t src, int16_t imm, int reg[]) {
    if(dest == 29)
        reg[dest] = reg[src] + imm/4;
    else
        reg[dest] = reg[src] + imm;
}

void addu (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]) {
    reg[dest] = reg[src_1] + reg[src_2];
}

void addiu (uint8_t dest, uint8_t src, uint16_t imm, int reg[]) {
    reg[dest] = reg[src] + imm;
}

void sub (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]){
    reg[dest] = reg[src_1] - reg[src_2];
}

void mult (uint8_t src_1, uint8_t src_2, int reg[]) {
    reg[33] = reg[src_1] * reg[src_2];
    reg[32] = 0;
}

void divv (uint8_t src_1, uint8_t src_2, int reg[]) {
    reg[32] = reg[src_1] % reg[src_2];
    reg[33] = reg[src_1] / reg[src_2];
}

void mfhi (uint8_t dest, int reg[]) {
    reg[dest] = reg[32];
}

void mflo (uint8_t dest, int reg[]) {
    reg[dest] = reg[33];
}

void and (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]) {
    reg[dest] = reg[src_1] & reg[src_2];
}

void or (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]) {
    reg[dest] = reg[src_1] | reg[src_2];
}

void xor (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]) {
    reg[dest] = reg[src_1] ^ reg[src_2];
}

void nor (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]) {
    reg[dest] = ~(reg[src_1] | reg[src_2]);
}

void slt (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]) {
    reg[dest] = reg[src_1] < reg[src_2];
}

void sll (uint8_t dest, uint8_t src, uint8_t shamt, int reg[]) {
    reg[dest] = reg[src] << shamt;
}

void srl (uint8_t dest, uint8_t src, uint8_t shamt, int reg[]) {
    reg[dest] = reg[src] >> shamt;
}

void sra (uint8_t dest, uint8_t src, uint8_t shamt, int reg[]) {
    reg[dest] = reg[src] >> shamt;
}

void andi (uint8_t dest, uint8_t src, int16_t imm, int reg[]) {
    reg[dest] = reg[src] & imm;
}

void ori (uint8_t dest, uint8_t src, int16_t imm, int reg[]) {
    reg[dest] = reg[src] | imm;
}

void xori (uint8_t dest, uint8_t src, int16_t imm, int reg[]) {
    reg[dest] = reg[src] ^ imm;
}

// Sistema

int syscall(int pc, int reg[], char string_data[]) {
    uint8_t v0 = reg[2];
    uint8_t a0 = reg[4];

    switch(v0) {
        case 1:
            printf("Printando um valor inteiro...\n");
            printf("%d\n\n", a0);
            break;

        case 4:
            printf("Printando uma string...\n");
            printf("%s\n\n", string_data);
            break;

        case 5:
            printf("Lendo um valor inteiro...\n");
            scanf("%d", &reg[2]);
            printf("\n\n");
            break;

        case 8:
            printf("Lendo uma string...\n");
            scanf("%d", &reg[2]);
            fgets(string_data, RAM_SIZE - 1, stdin);
            string_data[strcspn(string_data, "\n")] = 0;
            printf("\n\n");
            break;

        case 10:
            printf("Saindo do programa...\n\n");
            return RAM_SIZE;

        case 11:
            printf("Printando um caractere...\n");
            printf("%c\n\n", a0);
            break;

        case 12:
            printf("Lendo um caractere...\n");
            char c;
            scanf("%c", &c);
            a0 = c;
            printf("\n\n");
            break;

        case 17:
            printf("Saindo do programa...\n\n");
            return RAM_SIZE;

        default:
            printf("Chamada de sistema recebeu um código inválido");
    }

    return pc;
}

// Load e Store

bool lw(uint8_t dest, int16_t offset, uint8_t src, int reg[], int stack[]) {
    if(offset/4 + reg[src] >= RAM_SIZE)
        return false;

    reg[dest] = stack[offset/4 + reg[src]];
    return true;
}

bool lb(uint8_t dest, int16_t offset, uint8_t src, int reg[], int stack[]) {
    if(offset/4 + reg[src] >= RAM_SIZE)
        return false;

    reg[dest] = stack[offset/4 + reg[src]];
    return true;
}

bool lbu(uint8_t dest, int16_t offset, uint8_t src, int reg[], int stack[]) {
    if(offset/4 + reg[src] >= RAM_SIZE)
        return false;

    reg[dest] = stack[offset/4 + reg[src]];
    return true;
}

void lui (uint8_t dest, uint8_t src, int16_t imm, int reg[]) {
    reg[dest] = imm << 16;
}

bool sw(uint8_t dest, int16_t offset, uint8_t src, int reg[], int stack[]) {
    if(offset/4 + reg[src] >= RAM_SIZE)
        return false;

    stack[offset/4 + reg[src]] = reg[dest];
    return true;
}

bool sb(uint8_t dest, int16_t offset, uint8_t src, int reg[], int stack[]) {
    if(offset/4 + reg[src] >= RAM_SIZE)
        return false;

    stack[offset/4 + reg[src]] = reg[dest];
    return true;
}

// Condicionais e Saltos Condicionais

int beq(int pc, uint8_t src1, uint8_t src2, int16_t offset, int reg[]) {
    if(reg[src1] == reg[src2]) {
        int aux = pc + offset;
        return aux;
    }
    else
        return pc;
}

int bne(int pc, uint8_t src1, uint8_t src2, int16_t offset, int reg[]) {
    if(reg[src1] != reg[src2]) {
        int aux = pc + offset;
        return aux;
    }
    else
        return pc;
}

int blez(int pc, uint8_t src, int16_t offset, int reg[]) {
    if(reg[src] <= 0) {
        int aux = pc + offset;
        return aux;
    }
    else
        return pc;
}

int bgtz(int pc, uint8_t src, int16_t offset, int reg[]) {
    if(reg[src] > 0) {
        int aux = pc + offset;
        return aux;
    }
    else
        return pc;
}

void slti (uint8_t dest, uint8_t src, int16_t imm, int reg[]) {
    reg[dest] = reg[src] < imm;
}

void sltiu (uint8_t dest, uint8_t src, uint16_t imm, int reg[]) {
    reg[dest] = reg[src] < imm;
}

// Saltos Incondicionais

int j(uint32_t target) {
    return (int) ( (target << 2) - 4194304) / 4;
}

int jr(uint8_t src, int reg[]) {
    return reg[src];
}

int jal(int pc, uint32_t target, int reg[]) {
    reg[31] = pc;
    return (int) ( (target << 2) - 4194304) / 4;
}

#endif //MIPS_EMULATOR_OPERATIONS_H
