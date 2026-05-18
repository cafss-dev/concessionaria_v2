#include <stdio.h>
#include <string.h>
#include <strings.h>   // strcasecmp (Linux)
#include "estoque.h"

// FUNÇÕES AUXILIARES

// Conta quantas linhas (motos) existem no arquivo e volta o cursor para o início
int contarMotos(FILE *arquivo)
{
    if (arquivo == NULL) return 0;

    int contador = 0;
    char linha[300];

    while (fgets(linha, sizeof(linha), arquivo) != NULL)
        contador++;

    rewind(arquivo);
    return contador;
}

// Lê todas as motos do arquivo para o vetor lista[]
void lerMotos(FILE *arquivo, struct Moto lista[], int *total)
{
    char linha[300];
    *total = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        sscanf(linha, "%d;%49[^;];%49[^;];%d;%f;%d;%d;%d",
               &lista[*total].id,
               lista[*total].marca,
               lista[*total].modelo,
               &lista[*total].ano,
               &lista[*total].preco,
               &lista[*total].disponiveis,
               &lista[*total].reservadas,
               &lista[*total].vendidas);
        (*total)++;
    }
}

// Reescreve o arquivo inteiro com o vetor atualizado
void salvarMotos(struct Moto lista[], int total)
{
    FILE *arquivo = fopen("estoque/motos.txt", "w");
    if (arquivo == NULL)
    {
        printf("ERRO ao salvar estoque!\n");
        return;
    }

    for (int i = 0; i < total; i++)
    {
        fprintf(arquivo, "%d;%s;%s;%d;%.2f;%d;%d;%d\n",
                lista[i].id,
                lista[i].marca,
                lista[i].modelo,
                lista[i].ano,
                lista[i].preco,
                lista[i].disponiveis,
                lista[i].reservadas,
                lista[i].vendidas);
    }

    fclose(arquivo);
}

// CADASTRAR MOTO
// Se já existe moto com mesma marca + modelo + ano, apenas incrementa o estoque
void cadastrarMoto()
{
    struct Moto lista[200];
    int total = 0;

    // Carrega estoque atual (se existir)
    FILE *arquivo = fopen("estoque/motos.txt", "r");
    if (arquivo != NULL)
    {
        lerMotos(arquivo, lista, &total);
        fclose(arquivo);
    }

    struct Moto nova;
    printf("\n----- CADASTRAR MOTO -----\n");
    printf("Marca     : "); scanf(" %[^\n]", nova.marca);
    printf("Modelo    : "); scanf(" %[^\n]", nova.modelo);
    printf("Ano       : "); scanf("%d", &nova.ano);
    printf("Preco (R$): "); scanf("%f", &nova.preco);

    int quantidade;
    printf("Quantidade: "); scanf("%d", &quantidade);

    // Verifica se já existe uma entrada com mesma marca, modelo e ano
    for (int i = 0; i < total; i++)
    {
        if (strcasecmp(lista[i].marca,  nova.marca)  == 0 &&
            strcasecmp(lista[i].modelo, nova.modelo) == 0 &&
            lista[i].ano == nova.ano)
        {
            lista[i].disponiveis += quantidade;
            salvarMotos(lista, total);

            printf("\nMoto ja existe no sistema! Estoque atualizado.\n");
            printf("ID %d | %s %s %d | Disponiveis: %d\n",
                   lista[i].id, lista[i].marca, lista[i].modelo,
                   lista[i].ano, lista[i].disponiveis);
            return;
        }
    }

    // Moto nova — gera ID sequencial
    nova.id          = (total > 0) ? lista[total - 1].id + 1 : 1;
    nova.disponiveis = quantidade;
    nova.reservadas  = 0;
    nova.vendidas    = 0;

    lista[total] = nova;
    total++;

    salvarMotos(lista, total);
    printf("\nMoto cadastrada com sucesso! ID: %d\n", nova.id);
}

// LISTAR MOTOS — exibe todas com seus contadores
void listarMotos()
{
    FILE *arquivo = fopen("estoque/motos.txt", "r");
    if (arquivo == NULL)
    {
        printf("\nNenhuma moto cadastrada ainda.\n");
        return;
    }

    struct Moto lista[200];
    int total = 0;
    lerMotos(arquivo, lista, &total);
    fclose(arquivo);

    if (total == 0)
    {
        printf("Estoque vazio.\n");
        return;
    }

    printf("\n%-4s %-15s %-15s %-6s %-12s %-5s %-5s %-5s\n",
           "ID", "Marca", "Modelo", "Ano", "Preco", "Disp", "Res", "Vend");
    printf("-------------------------------------------------------------------\n");

    for (int i = 0; i < total; i++)
    {
        printf("%-4d %-15s %-15s %-6d R$%-10.2f %-5d %-5d %-5d\n",
               lista[i].id,
               lista[i].marca,
               lista[i].modelo,
               lista[i].ano,
               lista[i].preco,
               lista[i].disponiveis,
               lista[i].reservadas,
               lista[i].vendidas);
    }
}

// RESERVAR MOTO — decrementa disponiveis, incrementa reservadas
void reservarMoto()
{
    FILE *arquivo = fopen("estoque/motos.txt", "r");
    if (arquivo == NULL)
    {
        printf("\nNenhuma moto cadastrada ainda.\n");
        return;
    }

    struct Moto lista[200];
    int total = 0;
    lerMotos(arquivo, lista, &total);
    fclose(arquivo);

    printf("\n--- MOTOS DISPONIVEIS ---\n");
    int temDisponivel = 0;
    for (int i = 0; i < total; i++)
    {
        if (lista[i].disponiveis > 0)
        {
            printf("ID: %-4d | %-15s %-15s %d | R$%.2f | Disp: %d\n",
                   lista[i].id, lista[i].marca, lista[i].modelo,
                   lista[i].ano, lista[i].preco, lista[i].disponiveis);
            temDisponivel = 1;
        }
    }

    if (!temDisponivel)
    {
        printf("Nenhuma moto disponivel no momento.\n");
        return;
    }

    int id;
    int quantidade;
    printf("\nDigite o ID da moto para reservar: ");
    scanf("%d", &id);
    printf("\nDigite a quantidade que deseja reservar: ");
    scanf("%d", &quantidade);


    for (int i = 0; i < total; i++)
    {
        if (lista[i].id == id)
        {
            if (lista[i].disponiveis == 0)
            {
                printf("Sem unidades disponiveis para esse modelo.\n");
                return;
            }
            lista[i].disponiveis -= quantidade;
            lista[i].reservadas += quantidade;
            salvarMotos(lista, total);
            printf("Moto ID %d reservada com sucesso!\n", id);
            return;
        }
    }

    printf("ID %d nao encontrado.\n", id);
}

// CANCELAR RESERVA — decrementa reservadas, devolve para disponiveis
void cancelarReserva()
{
    FILE *arquivo = fopen("estoque/motos.txt", "r");
    if (arquivo == NULL)
    {
        printf("\nNenhuma moto cadastrada ainda.\n");
        return;
    }

    struct Moto lista[200];
    int total = 0;
    lerMotos(arquivo, lista, &total);
    fclose(arquivo);

    printf("\n--- MOTOS RESERVADAS ---\n");
    int temReservada = 0;
    for (int i = 0; i < total; i++)
    {
        if (lista[i].reservadas > 0)
        {
            printf("ID: %-4d | %-15s %-15s %d | Reservadas: %d\n",
                   lista[i].id, lista[i].marca, lista[i].modelo,
                   lista[i].ano, lista[i].reservadas);
            temReservada = 1;
        }
    }

    if (!temReservada)
    {
        printf("Nenhuma moto reservada no momento.\n");
        return;
    }

    int id;
    int quantidade;
    printf("\nDigite o ID da moto para cancelar a reserva: ");
    scanf("%d", &id);
    printf("\nDigite a quantidade de motos que terão a reserva cancelada: ");
    scanf("%d", &quantidade);

    for (int i = 0; i < total; i++)
    {
        if (lista[i].id == id)
        {
            if (lista[i].reservadas == 0)
            {
                printf("Essa moto nao possui reservas.\n");
                return;
            }
            lista[i].reservadas -= quantidade;
            lista[i].disponiveis += quantidade;
            salvarMotos(lista, total);
            printf("Reserva cancelada. Moto ID %d voltou para disponivel.\n", id);
            return;
        }
    }

    printf("ID %d nao encontrado.\n", id);
}

// REGISTRAR VENDA — decrementa reservadas, incrementa vendidas
// Fluxo obrigatório: reservar → vender
void registrarVendaMoto()
{
    FILE *arquivo = fopen("estoque/motos.txt", "r");
    if (arquivo == NULL)
    {
        printf("\nNenhuma moto cadastrada ainda.\n");
        return;
    }

    struct Moto lista[200];
    int total = 0;
    lerMotos(arquivo, lista, &total);
    fclose(arquivo);

    printf("\n--- MOTOS PRONTAS PARA VENDA (reservadas) ---\n");
    int temReservada = 0;
    for (int i = 0; i < total; i++)
    {
        if (lista[i].reservadas > 0)
        {
            printf("ID: %-4d | %-15s %-15s %d | R$%.2f | Reservadas: %d\n",
                   lista[i].id, lista[i].marca, lista[i].modelo,
                   lista[i].ano, lista[i].preco, lista[i].reservadas);
            temReservada = 1;
        }
    }

    if (!temReservada)
    {
        printf("Nenhuma moto reservada. Reserve uma moto antes de vender.\n");
        return;
    }

    int id;
    int quantidade;
    printf("\nDigite o ID da moto para venda: ");
    scanf("%d", &id);
    printf("\nDigite a quantidade de motos que serão vendidas: ");
    scanf("%d", &quantidade);

    for (int i = 0; i < total; i++)
    {
        if (lista[i].id == id)
        {
            if (lista[i].reservadas == 0)
            {
                printf("Sem reservas para esse modelo. Use a opcao Reservar primeiro.\n");
                return;
            }
            lista[i].reservadas-=quantidade;
            lista[i].vendidas+=quantidade;
            salvarMotos(lista, total);
            printf("Venda da moto ID %d registrada com sucesso!\n", id);
            printf("Total vendido desse modelo: %d unidade(s).\n", lista[i].vendidas);
            return;
        }
    }

    printf("ID %d nao encontrado.\n", id);
}
