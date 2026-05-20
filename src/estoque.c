#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "estoque.h"

// LER MOTOS JSON

void lerMotos(FILE *arquivo, struct Moto lista[], int *total)
{
    char linha[200];

    char marcaAtual[50] = "";

    *total = 0;

    while (fgets(linha, sizeof(linha), arquivo))
    {
        // procura marca
        if (strstr(linha, "{") && strstr(linha, "\""))
        {
            char chave[50];

            if (sscanf(linha, " \"%[^\"]\"", chave) == 1)
            {
                // ignora atributos internos
                if (
                    strcmp(chave, "id") != 0 &&
                    strcmp(chave, "marca") != 0 &&
                    strcmp(chave, "ano") != 0 &&
                    strcmp(chave, "preco") != 0 &&
                    strcmp(chave, "disponiveis") != 0 &&
                    strcmp(chave, "reservadas") != 0 &&
                    strcmp(chave, "vendidas") != 0)
                {
                    long pos = ftell(arquivo);

                    char prox[200];

                    if (fgets(prox, sizeof(prox), arquivo))
                    {
                        // se próxima linha também tiver {
                        // então é uma marca
                        if (strstr(prox, "{"))
                        {
                            strcpy(marcaAtual, chave);
                        }
                    }

                    fseek(arquivo, pos, SEEK_SET);
                }
            }
        }

        // procura modelo
        if (strstr(linha, "{") && strstr(linha, "\""))
        {
            char modelo[50];

            if (sscanf(linha, " \"%[^\"]\"", modelo) == 1)
            {
                if (
                    strcmp(modelo, "id") != 0 &&
                    strcmp(modelo, "marca") != 0 &&
                    strcmp(modelo, "ano") != 0 &&
                    strcmp(modelo, "preco") != 0 &&
                    strcmp(modelo, "disponiveis") != 0 &&
                    strcmp(modelo, "reservadas") != 0 &&
                    strcmp(modelo, "vendidas") != 0 &&
                    strcmp(modelo, marcaAtual) != 0)
                {
                    struct Moto m;

                    strcpy(m.modelo, modelo);

                    // marca
                    fgets(linha, sizeof(linha), arquivo);
                    sscanf(linha, " \"marca\": \"%[^\"]\"", m.marca);

                    // id
                    fgets(linha, sizeof(linha), arquivo);
                    sscanf(linha, " \"id\": %d", &m.id);

                    // ano
                    fgets(linha, sizeof(linha), arquivo);
                    sscanf(linha, " \"ano\": %d", &m.ano);

                    // preco
                    fgets(linha, sizeof(linha), arquivo);
                    sscanf(linha, " \"preco\": %f", &m.preco);

                    // disponiveis
                    fgets(linha, sizeof(linha), arquivo);
                    sscanf(linha, " \"disponiveis\": %d", &m.disponiveis);

                    // reservadas
                    fgets(linha, sizeof(linha), arquivo);
                    sscanf(linha, " \"reservadas\": %d", &m.reservadas);

                    // vendidas
                    fgets(linha, sizeof(linha), arquivo);
                    sscanf(linha, " \"vendidas\": %d", &m.vendidas);

                    lista[*total] = m;
                    (*total)++;
                }
            }
        }
    }
}

// SALVAR MOTOS JSON

void salvarMotos(struct Moto lista[], int total)
{
    FILE *f = fopen("data/estoque/motos.json", "w");

    if (f == NULL)
    {
        printf("Erro ao salvar motos.json\n");
        return;
    }

    fprintf(f, "{\n");

    for (int i = 0; i < total; i++)
    {
        int marcaExiste = 0;

        for (int j = 0; j < i; j++)
        {
            if (strcmp(lista[i].marca, lista[j].marca) == 0)
            {
                marcaExiste = 1;
                break;
            }
        }

        if (!marcaExiste)
        {
            fprintf(f, "  \"%s\": {\n", lista[i].marca);

            int primeiraMoto = 1;

            for (int j = 0; j < total; j++)
            {
                if (strcmp(lista[i].marca, lista[j].marca) == 0)
                {
                    if (!primeiraMoto)
                        fprintf(f, ",\n");

                    fprintf(f,
                            "    \"%s\": {\n"
                            "      \"marca\": \"%s\",\n"
                            "      \"id\": %d,\n"
                            "      \"ano\": %d,\n"
                            "      \"preco\": %.2f,\n"
                            "      \"disponiveis\": %d,\n"
                            "      \"reservadas\": %d,\n"
                            "      \"vendidas\": %d\n"
                            "    }",
                            lista[j].modelo,
                            lista[j].marca,
                            lista[j].id,
                            lista[j].ano,
                            lista[j].preco,
                            lista[j].disponiveis,
                            lista[j].reservadas,
                            lista[j].vendidas);

                    primeiraMoto = 0;
                }
            }

            fprintf(f, "\n  }");

            int outraMarca = 0;

            for (int k = i + 1; k < total; k++)
            {
                int repetida = 0;

                for (int z = 0; z <= i; z++)
                {
                    if (strcmp(lista[k].marca, lista[z].marca) == 0)
                    {
                        repetida = 1;
                        break;
                    }
                }

                if (!repetida)
                {
                    outraMarca = 1;
                    break;
                }
            }

            if (outraMarca)
                fprintf(f, ",");

            fprintf(f, "\n");
        }
    }

    fprintf(f, "}\n");

    fclose(f);

    printf("motos.json salvo com sucesso!\n");
}

// CADASTRAR MOTO

void cadastrarMoto()
{
    struct Moto lista[200];
    int total = 0;

    FILE *arquivo = fopen("data/estoque/motos.json", "r");

    if (arquivo != NULL)
    {
        lerMotos(arquivo, lista, &total);
        fclose(arquivo);
    }

    struct Moto nova;

    printf("\n----- CADASTRAR MOTO -----\n");

    printf("Marca     : ");
    scanf(" %[^\n]", nova.marca);

    printf("Modelo    : ");
    scanf(" %[^\n]", nova.modelo);

    printf("Ano       : ");
    scanf("%d", &nova.ano);

    printf("Preco (R$): ");
    scanf("%f", &nova.preco);

    int quantidade;

    printf("Quantidade: ");
    scanf("%d", &quantidade);

    // verifica duplicidade
    for (int i = 0; i < total; i++)
    {
        if (
            strcasecmp(lista[i].marca, nova.marca) == 0 &&
            strcasecmp(lista[i].modelo, nova.modelo) == 0 &&
            lista[i].ano == nova.ano)
        {
            lista[i].disponiveis += quantidade;

            salvarMotos(lista, total);

            printf("\nMoto ja existe! Estoque atualizado.\n");

            return;
        }
    }

    nova.id = (total > 0) ? lista[total - 1].id + 1 : 1;

    nova.disponiveis = quantidade;
    nova.reservadas = 0;
    nova.vendidas = 0;

    lista[total] = nova;
    total++;

    salvarMotos(lista, total);

    printf("\nMoto cadastrada com sucesso!\n");
}

// LISTAR MOTOS

void listarMotos()
{
    FILE *arquivo = fopen("data/estoque/motos.json", "r");

    if (arquivo == NULL)
    {
        printf("\nNenhuma moto cadastrada.\n");
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

// RESERVAR MOTO

void reservarMoto()
{
    FILE *arquivo = fopen("data/estoque/motos.json", "r");

    if (arquivo == NULL)
    {
        printf("\nNenhuma moto cadastrada.\n");
        return;
    }

    struct Moto lista[200];

    int total = 0;

    lerMotos(arquivo, lista, &total);

    fclose(arquivo);

    int id, quantidade;

    printf("\nDigite o ID da moto: ");
    scanf("%d", &id);

    printf("Quantidade: ");
    scanf("%d", &quantidade);

    for (int i = 0; i < total; i++)
    {
        if (lista[i].id == id)
        {
            if (quantidade > lista[i].disponiveis)
            {
                printf("Quantidade indisponivel.\n");
                return;
            }

            lista[i].disponiveis -= quantidade;
            lista[i].reservadas += quantidade;

            salvarMotos(lista, total);

            printf("Reserva realizada.\n");

            return;
        }
    }

    printf("ID nao encontrado.\n");
}

// CANCELAR RESERVA

void cancelarReserva()
{
    FILE *arquivo = fopen("data/estoque/motos.json", "r");

    if (arquivo == NULL)
    {
        printf("\nNenhuma moto cadastrada.\n");
        return;
    }

    struct Moto lista[200];

    int total = 0;

    lerMotos(arquivo, lista, &total);

    fclose(arquivo);

    int id, quantidade;

    printf("\nDigite o ID da moto: ");
    scanf("%d", &id);

    printf("Quantidade: ");
    scanf("%d", &quantidade);

    for (int i = 0; i < total; i++)
    {
        if (lista[i].id == id)
        {
            if (quantidade > lista[i].reservadas)
            {
                printf("Quantidade reservada insuficiente.\n");
                return;
            }

            lista[i].reservadas -= quantidade;
            lista[i].disponiveis += quantidade;

            salvarMotos(lista, total);

            printf("Reserva cancelada.\n");

            return;
        }
    }

    printf("ID nao encontrado.\n");
}

// REGISTRAR VENDA

void registrarVendaMoto()
{
    FILE *arquivo = fopen("data/estoque/motos.json", "r");

    if (arquivo == NULL)
    {
        printf("\nNenhuma moto cadastrada.\n");
        return;
    }

    struct Moto lista[200];// ======================================================
// ======================================================
// ======================================================
// ======================================================
// ======================================================
// ======================================================
// ======================================================
// ======================================================
// ======================================================
// ======================================================
// ======================================================
// ======================================================
// ======================================================
// ======================================================

    int total = 0;

    lerMotos(arquivo, lista, &total);

    fclose(arquivo);

    int id, quantidade;

    printf("\nDigite o ID da moto: ");
    scanf("%d", &id);

    printf("Quantidade vendida: ");
    scanf("%d", &quantidade);

    for (int i = 0; i < total; i++)
    {
        if (lista[i].id == id)
        {
            if (quantidade > lista[i].reservadas)
            {
                printf("Quantidade reservada insuficiente.\n");
                return;
            }

            lista[i].reservadas -= quantidade;
            lista[i].vendidas += quantidade;

            salvarMotos(lista, total);

            printf("Venda registrada.\n");

            return;
        }
    }

    printf("ID nao encontrado.\n");
}