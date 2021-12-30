/*
=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-==-=-=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-==--==-=-=-=-=-=-=-=-=-=-=-=-=-==

                                  ¥ Simulador Mips 32 BIts ¥

=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-==-=-=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-==--==-=-=-=-=-=-=-=-=-=-=-=-=-==

                                          ¥ Participantes ¥

@Gabriel Molina de Lima                     RA 2208423
@Guilherme Moreira Lima Furlaneto           RA 2207192
@Gustavo Geovane Tamião de Souza            RA 2271990
@Pedro Mian Parra                           RA 2207249

=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-==-=-=-=-=--=-=-=-=-=--=-=-=-=-=-=-=-==--==-=-=-=-=-=-=-=-=-=-=-=-=-==
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "funcoes.h"

int main(int argc, char *argv[]) {

    //Arquivos
    FILE *entrada = fopen(argv[1], "r+b");
    FILE *saida_op = fopen(argv[2], "w");
    FILE *saida_reg = fopen(argv[3], "w");

    //Verifica Parâmetros para rodar o código
    VerificaParametros(entrada, saida_op, saida_reg, argc);

    int opcao = 0;
    //verifica quantos bytes tem no arquivo binário
    fseek(entrada, 0, SEEK_END);
    //realiza a divisão por 32 bits
    opcao = ftell(entrada) / sizeof(uint32_t);

    //voltando pro inicio arquivo
    rewind(entrada);

    //lendo opcode arquivo
    uint32_t instrucoes[opcao];
    fread(instrucoes, sizeof(uint32_t), opcao, entrada);

    int a = 0x12345678;
    unsigned char *c = (unsigned char*) (&a);
    bool little_endian = false;
    if (*c == 0x78) little_endian = true;
    else little_endian = false;

//Invertendo a ordem das instruções de little endian para Big Endian
    if (little_endian) {
        uint32_t ic;
        for (int i = 0; i < opcao; i++) {
            ic = instrucoes[i];
            instrucoes[i] = ((ic & 0xFF000000) >> 24 | (ic & 0x00FF0000) >> 8 | (ic & 0x0000FF00) << 8 |  (ic & 0x000000FF) << 24 );
        }
    }//if

    int stack[RAM_SIZE];
    for(int i = 0; i < RAM_SIZE; i++) {
        stack[i] = 0;
    }

    char string_data[RAM_SIZE];     // Vetor para salvar strings
    for(int i = 0; i < RAM_SIZE; i++) {
        string_data[i] = 0;
    }

    int registrador[34];  // (indice = código do registrador; conteúdo salvo na posiçao)
    for(int i = 0; i < 34; i++) {
        registrador[i] = 0;
    }
    registrador[29] = RAM_SIZE;

    int pc = 0;
    int aux;
    int opt;
    char aux_char;

    printf ("\nSimulador MIPS\n");
    do {
        printf ("1 - Rodar o programa inteiro\n2 - Rodar passo a passo\nSua escolha: ");
        scanf ("%d", &opt);
        fflush(stdin);
        printf ("\n");
        if (opt != 1 && opt != 2) printf ("Opcao invalida, tente novamente\n");
    } while (opt != 1 && opt != 2);

    int pc_aux = -1;
    do {
        if (opt == 1) { }
        else {
            printf ("Precione Enter para compilar a proxima instrucao:");
            scanf ("%c", &aux_char);
            fflush(stdin);
            printf ("\n");
        }

        uint8_t opcode = (instrucoes[pc] & 4227858432) >> 26;

        if(opcode == 0) {   // r-type instruction
            uint8_t rs = (instrucoes[pc] & 65011712) >> 21;  //fonte
            uint8_t rt = (instrucoes[pc] & 2031616) >> 16;   //temp
            uint8_t rd = (instrucoes[pc] & 63488) >> 11;     //definitivo
            uint8_t shamt = (instrucoes[pc] & 1984) >> 6;    //shift amount
            uint8_t funct = instrucoes[pc] & 63;             //function
            switch(funct) {
                case 32:    // add
                add(rd, rs, rt, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: add %d, %d, %d\n", pc, rd, rs, rt);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rs, registrador[rs], rt, registrador[rt]);
                if (opt == 2) {
                    printf("$pc = %d\nOperacao: add %d, %d, %d\n", pc, rd, rs, rt);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rs, registrador[rs], rt, registrador[rt]);
                }
                break;

                case 33:    // addu
                addu(rd, rs, rt, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: addu %d, %d, %d\n", pc, rd, rs, rt);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rs, registrador[rs], rt, registrador[rt]);
                if (opt == 2) {
                    printf("$pc = %d\nOperacao: addu %d, %d, %d\n", pc, rd, rs, rt);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rs, registrador[rs], rt, registrador[rt]);
                }
                break;

                case 34:    // sub
                sub(rd, rs, rt, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: sub %d, %d, %d\n", pc, rd, rs, rt);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rs, registrador[rs], rt, registrador[rt]);
                if (opt == 2) {
                    printf("$pc = %d\nOperacao: sub %d, %d, %d\n", pc, rd, rs, rt);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rs, registrador[rs], rt, registrador[rt]);
                }
                break;

                case 24:    // mult
                mult (rs, rt, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: mult %d, %d\n", pc, rs, rt);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador HI: %d\n  Registrador LO: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", registrador[32], registrador[33], rs, registrador[rs], rt, registrador[rt]);
                if (opt == 2) {
                    printf("$pc = %d\nOperacao: mult %d, %d\n", pc, rs, rt);
                    printf("Conteudo dos registradores:\n  Registrador HI: %d\n  Registrador LO: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", registrador[32], registrador[33], rs, registrador[rs], rt, registrador[rt]);
                }
                break;

                case 26:    // div
                divv (rs, rt, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: div %d, %d\n", pc, rs, rt);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador HI: %d\n  Registrador LO: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", registrador[32], registrador[33], rs, registrador[rs], rt, registrador[rt]);
                if (opt == 2) {
                    printf("$pc = %d\nOperacao: div %d, %d\n", pc, rs, rt);
                    printf("Conteudo dos registradores:\n  Registrador HI: %d\n  Registrador LO: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", registrador[32], registrador[33], rs, registrador[rs], rt, registrador[rt]);
                }
                break;

                case 36:    // and
                and(rd, rs, rt, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: and %d, %d, %d\n", pc, rd, rs, rt);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rs, registrador[rs], rt, registrador[rt]);
                if(opt == 2){
                    printf("$pc = %d\nOperacao: and %d, %d, %d\n", pc, rd, rs, rt);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rs, registrador[rs], rt, registrador[rt]);
                }
                break;

                case 37:    // or
                or(rd, rs, rt, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: or %d, %d, %d\n", pc, rd, rs, rt);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rs, registrador[rs], rt, registrador[rt]);
                if(opt == 2) {
                    printf("$pc = %d\nOperacao: or %d, %d, %d\n", pc, rd, rs, rt);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rs, registrador[rs], rt, registrador[rt]);
                }
                break;

                case 38:    // xor
                xor(rd, rs, rt, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: xor %d, %d, %d\n", pc, rd, rs, rt);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rs, registrador[rs], rt, registrador[rt]);
                if(opt == 2){
                    printf("$pc = %d\nOperacao: xor %d, %d, %d\n", pc, rd, rs, rt);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rs, registrador[rs], rt, registrador[rt]);
                }
                break;

                case 39:    // nor
                nor(rd, rs, rt, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: nor %d, %d, %d\n", pc, rd, rs, rt);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rs, registrador[rs], rt, registrador[rt]);
                if(opt==2){
                    printf("$pc = %d\nOperacao: nor %d, %d, %d\n", pc, rd, rs, rt);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rs, registrador[rs], rt, registrador[rt]);
                }
                break;

                case 42:    // slt
                slt(rd, rs, rt, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: slt %d, %d, %d\n", pc, rd, rs, rt);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rs, registrador[rs], rt, registrador[rt]);
                if(opt==2){
                    printf("$pc = %d\nOperacao: slt %d, %d, %d\n", pc, rd, rs, rt);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rs, registrador[rs], rt, registrador[rt]);
                }
                break;

                case 0:    // sll
                sll (rd, rt, shamt, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: sll %d, %d, %d\n", pc, rd, rt, shamt);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rt, registrador[rt]);
                if(opt==2){
                    printf("$pc = %d\nOperacao: sll %d, %d, %d\n", pc, rd, rt, shamt);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rt, registrador[rt]);
                }
                break;

                case 2:    // srl
                srl (rd, rt, shamt, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: srl %d, %d, %d\n", pc, rd, rt, shamt);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rt, registrador[rt]);
                if(opt==2){
                    printf("$pc = %d\nOperacao: srl %d, %d, %d\n", pc, rd, rt, shamt);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rt, registrador[rt]);
                }
                break;

                case 3:    // sra
                sra (rd, rt, shamt, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: sra %d, %d, %d\n", pc, rd, rt, shamt);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rt, registrador[rt]);
                if(opt==2){
                    printf("$pc = %d\nOperacao: sra %d, %d, %d\n", pc, rd, rt, shamt);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, registrador[rd], rt, registrador[rt]);
                }
                break;

                case 12:    // syscall -> falta colocar uma condiçao de v0 != de 1 ou v0 != 10 (só implementei pros dois casos, nao vai fazer nada caso v0 seja diferente desses valores)
                fprintf(saida_op, "$pc = %d\nOperacao: syscall\n", pc);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador 2: %d\n  Registrador 4: %d\n\n", registrador[2], registrador[4]);
                if(opt==2){
                    printf("$pc = %d\nOperacao: syscall\n", pc);
                    printf("Conteudo dos registradores:\n  Registrador 2: %d\n  Registrador 4: %d\n\n", registrador[2], registrador[4]);
                }
                pc_aux = pc;
                pc = syscall(pc, registrador, string_data);
                break;

                case 8:    // jr
                fprintf(saida_op, "$pc = %d\nOperacao: jr %d\n", pc, rs);
                if(opt==2) printf("$pc = %d\nOperacao: jr %d\n", pc, rs);
                aux = jr(rs, registrador);
                if(aux < opcao && aux >= 0) {
                    pc = aux;
                    fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Novo $pc: %d\n\n", rs, registrador[rs], pc);
                    if(opt==2) printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Novo $pc: %d\n\n", rs, registrador[rs], pc);
                }
                else {
                    fprintf(saida_op, "Nao foi possivel realizar o salto -> valor de $pc fora do escopo\n");
                    if(opt==2) printf("Nao foi possivel realizar o salto -> valor de $pc fora do escopo\n");
                    exit(3);
                }
                break;

                case 16:    // mfhi
                mfhi (rd, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: mfhi %d\n", pc, rd);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador HI: %d\n  Registrador %d: %d\n\n", registrador[32], rd, registrador[rd]);
                if(opt==2){
                    printf("$pc = %d\nOperacao: mfhi %d\n", pc, rd);
                    printf("Conteudo dos registradores:\n  Registrador HI: %d\n  Registrador %d: %d\n\n", registrador[32], rd, registrador[rd]);
                }
                break;

                case 18:    // mflo
                mflo (rd, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: mflo %d\n", pc, rd);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador LO: %d\n  Registrador %d: %d\n\n", registrador[33], rd, registrador[rd]);
                if(opt==2){
                    printf("$pc = %d\nOperacao: mflo %d\n", pc, rd);
                    printf("Conteudo dos registradores:\n  Registrador LO: %d\n  Registrador %d: %d\n\n", registrador[33], rd, registrador[rd]);
                }
                break;

                default:
                    fprintf(saida_op, "Instruçao invalida\n\n");
                    if(opt==2) printf("Instruçao invalida\n\n");
                    break;
            }

        } else if(opcode == 2 || opcode == 3) {   // j-type instruction
            uint32_t taropcao = instrucoes[pc] & 67108863;

            switch(opcode) {
                case 2:     // j
                fprintf(saida_op, "$pc = %d\nOperacao: j 0x%08x\n", pc, taropcao);
                if(opt==2) printf("$pc = %d\nOperacao: j 0x%08x\n", pc, taropcao);
                aux = j(taropcao);
                if(aux < opcao && aux >= 0) {
                    pc = aux - 1;
                    fprintf(saida_op, "Conteudo dos registradores:\n  Novo $pc: %d\n\n", pc + 1);
                    if(opt==2) printf("Conteudo dos registradores:\n  Novo $pc: %d\n\n", pc + 1);
                }
                else {
                    fprintf(saida_op, "Nao foi possivel realizar o salto -> valor de $pc fora do escopo\n");
                    if(opt==2) printf("Nao foi possivel realizar o salto -> valor de $pc fora do escopo\n");
                    exit(3);
                }
                break;

                case 3:     // jal
                fprintf(saida_op, "$pc = %d\nOperacao: jal 0x%08x\n", pc, taropcao);
                if(opt==2) printf("$pc = %d\nOperacao: jal 0x%08x\n", pc, taropcao);
                aux = jal(pc, taropcao, registrador);
                if(aux < opcao && aux >= 0) {
                    pc = aux - 1;
                    fprintf(saida_op, "Conteudo dos registradores:\n  Registrador 31: %d\n  Novo $pc: %d\n\n", registrador[31], pc + 1);
                    if(opt==2) printf("Conteudo dos registradores:\n  Registrador 31: %d\n  Novo $pc: %d\n\n", registrador[31], pc + 1);
                }
                else {
                    fprintf(saida_op, "Nao foi possivel realizar o salto -> valor de $pc fora do escopo\n");
                    if(opt==2) printf("Nao foi possivel realizar o salto -> valor de $pc fora do escopo\n");
                    exit(3);
                }
                break;

                default:
                    fprintf(saida_op, "Instruçao invalida\n\n");
                    if(opt==2) printf("Instruçao invalida\n\n");
                    break;
            }

        } else {    // i-type instruction
            uint8_t rs = (instrucoes[pc] & 65011712) >> 21;
            uint8_t rt = (instrucoes[pc] & 2031616) >> 16;
            int16_t immediate = instrucoes[pc] & 65535;

            switch(opcode) {
                case 4:     // beq
                fprintf(saida_op, "$pc = %d\nOperacao: beq %d, %d, %d\n", pc, rs, rt, immediate);
                if(opt==2) printf("$pc = %d\nOperacao: beq %d, %d, %d\n", pc, rs, rt, immediate);
                aux = beq(pc, rs, rt, immediate, registrador);
                if(aux < opcao && aux >= 0) {
                    pc = aux;
                    fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Novo $pc: %d\n\n", rs, registrador[rs], rt, registrador[rt], pc);
                    if(opt==2) printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Novo $pc: %d\n\n", rs, registrador[rs], rt, registrador[rt], pc);
                }
                else {
                    fprintf(saida_op, "Nao foi possivel realizar o salto -> valor de $pc fora do escopo\n");
                    if(opt==2) printf("Nao foi possivel realizar o salto -> valor de $pc fora do escopo\n");
                    exit(3);
                }
                break;

                case 5:     // bne
                fprintf(saida_op, "$pc = %d\nOperacao: bne %d, %d, %d\n", pc, rs, rt, immediate);
                if(opt==2) printf("$pc = %d\nOperacao: bne %d, %d, %d\n", pc, rs, rt, immediate);
                aux = bne(pc, rs, rt, immediate, registrador);
                if(aux < opcao || aux >= 0) {
                    pc = aux;
                    fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Novo $pc: %d\n\n", rs, registrador[rs], rt, registrador[rt], pc);
                    if(opt==2) printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n  Novo $pc: %d\n\n", rs, registrador[rs], rt, registrador[rt], pc);
                }
                else {
                    fprintf(saida_op, "Nao foi possivel realizar o salto -> valor de $pc fora do escopo\n");
                    if(opt==2) printf("Nao foi possivel realizar o salto -> valor de $pc fora do escopo\n");
                    exit(3);
                }
                break;

                case 6:     // blez
                fprintf(saida_op, "$pc = %d\nOperacao: blez %d, %d\n", pc, rs, immediate);
                if(opt==2) printf("$pc = %d\nOperacao: blez %d, %d\n", pc, rs, immediate);
                aux = blez(pc, rs, immediate, registrador);
                if(aux < opcao || aux >= 0) {
                    pc = aux;
                    fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Novo $pc: %d\n\n", rs, registrador[rs], pc);
                    if(opt==2) printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Novo $pc: %d\n\n", rs, registrador[rs], pc);
                }
                else {
                    fprintf(saida_op, "Nao foi possivel realizar o salto -> valor de $pc fora do escopo\n");
                    if(opt==2) printf("Nao foi possivel realizar o salto -> valor de $pc fora do escopo\n");
                    exit(3);
                }
                break;

                case 7:     // bgtz
                fprintf(saida_op, "$pc = %d\nOperacao: blez %d, %d\n", pc, rs, immediate);
                if(opt==2) printf("$pc = %d\nOperacao: blez %d, %d\n", pc, rs, immediate);
                aux = bgtz(pc, rs, immediate, registrador);
                if(aux < opcao || aux >= 0) {
                    pc = aux;
                    fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Novo $pc: %d\n\n", rs, registrador[rs], pc);
                    if(opt==2) printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Novo $pc: %d\n\n", rs, registrador[rs], pc);
                }
                else {
                    fprintf(saida_op, "Nao foi possivel realizar o salto -> valor de $pc fora do escopo\n");
                    if(opt==2) printf("Nao foi possivel realizar o salto -> valor de $pc fora do escopo\n");
                    exit(3);
                }
                break;

                case 8:     // addi
                addi(rt, rs, immediate, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: addi %d, %d, %d\n", pc, rt, rs, immediate);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                if(opt==2){
                    printf("$pc = %d\nOperacao: addi %d, %d, %d\n", pc, rt, rs, immediate);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                }
                break;

                case 9:     // addiu
                addiu(rt, rs, immediate, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: addiu %d, %d, %d\n", pc, rt, rs, immediate);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                if(opt==2){
                    printf("$pc = %d\nOperacao: addiu %d, %d, %d\n", pc, rt, rs, immediate);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                }
                break;

                case 10:     // slti
                slti(rt, rs, immediate, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: slti %d, %d, %d\n", pc, rt, rs, immediate);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                if(opt==2){
                    printf("$pc = %d\nOperacao: slti %d, %d, %d\n", pc, rt, rs, immediate);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                }
                break;

                case 11:     // sltiu
                sltiu(rt, rs, immediate, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: sltu %d, %d, %d\n", pc, rt, rs, immediate);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                if(opt==2){
                    printf("$pc = %d\nOperacao: sltu %d, %d, %d\n", pc, rt, rs, immediate);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                }
                break;

                case 12:     // andi
                andi(rt, rs, immediate, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: andi %d, %d, %d\n", pc, rt, rs, immediate);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                if(opt==2){
                    printf("$pc = %d\nOperacao: andi %d, %d, %d\n", pc, rt, rs, immediate);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                }
                break;

                case 13:     // ori
                ori(rt, rs, immediate, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: ori %d, %d, %d\n", pc, rt, rs, immediate);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                if(opt==2){
                    printf("$pc = %d\nOperacao: ori %d, %d, %d\n", pc, rt, rs, immediate);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                }
                break;

                case 14:     // xori
                xori(rt, rs, immediate, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: xori %d, %d, %d\n", pc, rt, rs, immediate);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                if(opt==2){
                    printf("$pc = %d\nOperacao: xori %d, %d, %d\n", pc, rt, rs, immediate);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                }
                break;

                case 15:     // lui
                lui(rt, rs, immediate, registrador);
                fprintf(saida_op, "$pc = %d\nOperacao: lui %d, %d, %d\n", pc, rt, rs, immediate);
                fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                if(opt==2){
                    printf("$pc = %d\nOperacao: lui %d, %d, %d\n", pc, rt, rs, immediate);
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                }
                break;

                case 32:    // lb
                fprintf(saida_op, "$pc = %d\nOperacao: lb %d, %d(%d)\n", pc, rt, immediate, rs);
                if(opt==2) printf("$pc = %d\nOperacao: lb %d, %d(%d)\n", pc, rt, immediate, rs);
                if(lb(rt, immediate, rs, registrador, stack)) {
                    fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                    if(opt==2) printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                } else {
                    fprintf(saida_op, "Erro ao realizar a operacao -> Nao foi reservado espaço na memoria\n");
                    if(opt==2) printf("Erro ao realizar a operacao -> Nao foi reservado espaço na memoria\n");
                    exit(3);
                }
                break;

                case 35:    // lw
                fprintf(saida_op, "$pc = %d\nOperacao: lw %d, %d(%d)\n", pc, rt, immediate, rs);
                if(opt==2) printf("$pc = %d\nOperacao: lw %d, %d(%d)\n", pc, rt, immediate, rs);
                if(lw(rt, immediate, rs, registrador, stack)) {
                    fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                    if(opt==2) printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                } else {
                    fprintf(saida_op, "Erro ao realizar a operacao -> Nao foi reservado espaço na memoria\n");
                    if(opt==2) printf("Erro ao realizar a operacao -> Nao foi reservado espaço na memoria\n");
                    exit(3);
                }
                break;

                case 36:    // lbu
                fprintf(saida_op, "$pc = %d\nOperacao: lbu %d, %d(%d)\n", pc, rt, immediate, rs);
                if(opt==2) printf("$pc = %d\nOperacao: lbu %d, %d(%d)\n", pc, rt, immediate, rs);
                if(lbu(rt, immediate, rs, registrador, stack)) {
                    fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                    if(opt==2) printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                } else {
                    fprintf(saida_op, "Erro ao realizar a operacao -> Nao foi reservado espaço na memoria\n");
                    if(opt==2) printf("Erro ao realizar a operacao -> Nao foi reservado espaço na memoria\n");
                    exit(3);
                }
                break;

                case 40:    // sb
                fprintf(saida_op, "$pc = %d\nOperacao: sb %d, %d(%d)\n", pc, rt, immediate, rs);
                if(opt==2) printf("$pc = %d\nOperacao: sb %d, %d(%d)\n", pc, rt, immediate, rs);
                if(sb(rt, immediate, rs, registrador, stack)) {
                    fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                    fprintf(saida_op, "Conteudo da memoria:\n  RAM[%d]: %d\n\n", registrador[rs] + immediate / 4, stack[immediate / 4 + registrador[rs]]);
                    if(opt==2) {
                        printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rt, registrador[rt], rs, registrador[rs]);
                        printf("Conteudo da memoria:\n  RAM[%d]: %d\n\n", registrador[rs] + immediate / 4, stack[immediate / 4 + registrador[rs]]);
                    }
                } else {
                    fprintf(saida_op, "Erro ao realizar a operacao -> Nao foi reservado espaço na memoria\n");
                    if(opt==2) printf("Erro ao realizar a operacao -> Nao foi reservado espaço na memoria\n");
                    exit(3);
                }
                break;

                case 43:    // sw
                fprintf(saida_op, "$pc = %d\nOperacao: sw %d, %d(%d)\n", pc, rt, immediate, rs);
                if(opt==2) printf("$pc = %d\nOperacao: sw %d, %d(%d)\n", pc, rt, immediate, rs);
                if(sw(rt, immediate, rs, registrador, stack)) {
                    fprintf(saida_op, "Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n", rt, registrador[rt], rs, registrador[rs]);
                    fprintf(saida_op, "Conteudo da memoria:\n  RAM[%d]: %d\n\n", registrador[rs] + immediate / 4, stack[immediate / 4 + registrador[rs]]);
                    if(opt==2) {
                        printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n", rt, registrador[rt], rs, registrador[rs]);
                        printf("Conteudo da memoria:\n  RAM[%d]: %d\n\n",  registrador[rs] + immediate / 4, stack[immediate / 4 + registrador[rs]]);
                    }
                } else {
                    fprintf(saida_op, "Erro ao realizar a operacao -> Nao foi reservado espaço na memoria\n");
                    if(opt==2) printf("Erro ao realizar a operacao -> Nao foi reservado espaço na memoria\n");
                    exit(3);
                }
                break;

                default:
                    fprintf(saida_op, "Instruçao invalida\n\n");
                    if(opt==2) printf("Instruçao invalida\n\n");
                    break;
            }
        }
        pc++;
    } while (pc < opcao);

    if (pc == RAM_SIZE + 1) pc = pc_aux;
    else pc--;
    printf("Finalizado!\nO arquivo com todas as instrucoes realizadas, bem como o arquivo com o estado final de cada registrador\nfoi gerado no diretorio do programa\n");

    fprintf (saida_reg, "Nome\t\tNumero\t\tValor\n\n"
                         "$zero\t\t0\t\t%d\n"
                         "$at\t\t1\t\t%d\n"
                         "$v0\t\t2\t\t%d\n"
                         "$v1\t\t3\t\t%d\n"
                         "$a0\t\t4\t\t%d\n"
                         "$a1\t\t5\t\t%d\n"
                         "$a2\t\t6\t\t%d\n"
                         "$a3\t\t7\t\t%d\n"
                         "$t0\t\t8\t\t%d\n"
                         "$t1\t\t9\t\t%d\n"
                         "$t2\t\t10\t\t%d\n"
                         "$t3\t\t11\t\t%d\n"
                         "$t4\t\t12\t\t%d\n"
                         "$t5\t\t13\t\t%d\n"
                         "$t6\t\t14\t\t%d\n"
                         "$t7\t\t15\t\t%d\n"
                         "$s0\t\t16\t\t%d\n"
                         "$s1\t\t17\t\t%d\n"
                         "$s2\t\t18\t\t%d\n"
                         "$s3\t\t19\t\t%d\n"
                         "$s4\t\t20\t\t%d\n"
                         "$s5\t\t21\t\t%d\n"
                         "$s6\t\t22\t\t%d\n"
                         "$s7\t\t23\t\t%d\n"
                         "$t8\t\t24\t\t%d\n"
                         "$t9\t\t25\t\t%d\n"
                         "$k0\t\t26\t\t%d\n"
                         "$k1\t\t27\t\t%d\n"
                         "$gp\t\t28\t\t%d\n"
                         "$sp\t\t29\t\t%d\n"
                         "$fp\t\t30\t\t%d\n"
                         "$ra\t\t31\t\t%d\n"
                         "pc\t\t\t\t%d\n"
                         "hi\t\t\t\t%d\n"
                         "lo\t\t\t\t%d\n",
                         registrador[0], registrador[1], registrador[2], registrador[3],
                         registrador[4], registrador[5], registrador[6], registrador[7],
                         registrador[8], registrador[9], registrador[10], registrador[11],
                         registrador[12], registrador[13], registrador[14], registrador[15],
                         registrador[16], registrador[17], registrador[18], registrador[19],
                         registrador[20], registrador[21], registrador[22], registrador[23],
                         registrador[24], registrador[25], registrador[26], registrador[27],
                         registrador[28], registrador[29], registrador[30], registrador[31],
                         pc, registrador[32], registrador[33]);

    fclose(entrada);
    fclose(saida_op);
    fclose(saida_reg);
    return 0;
}
