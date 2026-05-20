#ifndef ESTOQUE_H
#define ESTOQUE_H

#include <stdio.h>

struct Moto {
    int   id;
    char  marca[50];
    char  modelo[50];
    int   ano;
    float preco;
    int   disponiveis;   // unidades disponíveis para venda
    int   reservadas;    // unidades reservadas por clientes
    int   vendidas;      // unidades já vendidas (histórico)
};

int  contarMotos(FILE *arquivo);
void lerMotos(FILE *arquivo, struct Moto lista[], int *total);
void salvarMotos(struct Moto lista[], int total);
void cadastrarMoto();
void listarMotos();
void reservarMoto();
void cancelarReserva();
void registrarVendaMoto();

#endif
