#include <stdio.h>
#include "menu.h"
#include "arquivos.h"

void pausar()
{
    printf("\nPressione ENTER para continuar...");
    // Limpa qualquer caractere sobrando no buffer antes de esperar
    while (getchar() != '\n');
    getchar();
}
void exibirMenu()
{
    // Lê o nome da loja do config.json para exibir no cabeçalho
    char nomeLoja[100];
    lerNomeLoja(nomeLoja, sizeof(nomeLoja));

    printf("\n==========================================\n");
    printf("  %-40s\n", nomeLoja);
    printf("==========================================\n");
    printf("\n  --- ESTOQUE ---\n");
    printf("  1.  Cadastrar moto\n");
    printf("  2.  Listar motos\n");
    printf("  3.  Reservar moto\n");
    printf("  4.  Cancelar reserva\n");
    printf("  5.  Registrar venda\n");
    printf("\n  --- ADMINISTRATIVO ---\n");
    printf("  6.  Gerar config.json\n");
    printf("  7.  Mostrar config.json\n");
    printf("  8.  Verificar log ativo\n");
    printf("  9.  Criar backup\n");
    printf("  10. Restaurar backup\n");
    printf("  11. Verificar permissoes\n");
    printf("\n  0.  Sair\n");
    printf("==========================================\n");
}
