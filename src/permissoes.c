#include <stdio.h>
#include "permissoes.h"

// Verifica se um perfil tem determinada permissão usando AND bit a bit
// Ex: PERFIL_VENDEDOR & PERM_RESERVAR → resultado != 0 → tem permissão
static int temPermissao(int perfil, int permissao)
{
    return (perfil & permissao) != 0;
}

// Converte um inteiro para representação binária de 5 bits (string)
static void intParaBinario(int valor, char *saida)
{
    saida[0] = (valor >> 4 & 1) ? '1' : '0';
    saida[1] = (valor >> 3 & 1) ? '1' : '0';
    saida[2] = (valor >> 2 & 1) ? '1' : '0';
    saida[3] = (valor >> 1 & 1) ? '1' : '0';
    saida[4] = (valor      & 1) ? '1' : '0';
    saida[5] = '\0';
}

// Exibe todas as permissões de um perfil
static void exibirPerfil(const char *nomePerfil, int perfil)
{
    char binario[6];
    intParaBinario(perfil, binario);

    printf("\n  [ %s ]  (binario: %s | decimal: %d)\n", nomePerfil, binario, perfil);
    printf("  %-20s %s\n", "Listar motos:",   temPermissao(perfil, PERM_LISTAR)    ? "SIM" : "NAO");
    printf("  %-20s %s\n", "Cadastrar:",      temPermissao(perfil, PERM_CADASTRAR) ? "SIM" : "NAO");
    printf("  %-20s %s\n", "Reservar:",       temPermissao(perfil, PERM_RESERVAR)  ? "SIM" : "NAO");
    printf("  %-20s %s\n", "Vender:",         temPermissao(perfil, PERM_VENDER)    ? "SIM" : "NAO");
    printf("  %-20s %s\n", "Administrador:",  temPermissao(perfil, PERM_ADMIN)     ? "SIM" : "NAO");
}

void verificarPermissoes()
{
    printf("\n========== SISTEMA DE PERMISSOES (BITWISE) ==========\n");

    printf("\nCada permissao ocupa 1 bit no numero inteiro do perfil:\n");
    printf("  LISTAR    = 0x01 = 00001\n");
    printf("  CADASTRAR = 0x02 = 00010\n");
    printf("  RESERVAR  = 0x04 = 00100\n");
    printf("  VENDER    = 0x08 = 01000\n");
    printf("  ADMIN     = 0x10 = 10000\n");

    printf("\nPerfis do sistema:\n");
    printf("------------------------------------------------------");

    exibirPerfil("MECANICO",  PERFIL_MECANICO);
    exibirPerfil("VENDEDOR",  PERFIL_VENDEDOR);
    exibirPerfil("GERENTE",   PERFIL_GERENTE);

    printf("\n------------------------------------------------------\n");
    printf("Exemplo de verificacao:\n");
    printf("  VENDEDOR & PERM_CADASTRAR = %d & %d = %d → %s\n",
           PERFIL_VENDEDOR, PERM_CADASTRAR,
           PERFIL_VENDEDOR & PERM_CADASTRAR,
           (PERFIL_VENDEDOR & PERM_CADASTRAR) ? "TEM PERMISSAO" : "SEM PERMISSAO");
    printf("  GERENTE  & PERM_CADASTRAR = %d & %d = %d → %s\n",
           PERFIL_GERENTE, PERM_CADASTRAR,
           PERFIL_GERENTE & PERM_CADASTRAR,
           (PERFIL_GERENTE & PERM_CADASTRAR) ? "TEM PERMISSAO" : "SEM PERMISSAO");
    printf("======================================================\n");
}
