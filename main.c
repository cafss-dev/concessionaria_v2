#include <stdio.h>
#include "menu.h"
#include "estoque.h"
#include "arquivos.h"
#include "log.h"
#include "permissoes.h"

int main()
{
    criarEstruturaPastas();
    registrarLog("Sistema iniciado.");

    int opcao;

    do {
        exibirMenu();

        printf("  Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
            case 1:
                cadastrarMoto();
                registrarLog("Cadastro de moto realizado.");
                pausar();
                break;

            case 2:
                listarMotos();
                pausar();
                break;

            case 3:
                reservarMoto();
                registrarLog("Reserva de moto realizada.");
                pausar();
                break;

            case 4:
                cancelarReserva();
                registrarLog("Reserva cancelada.");
                pausar();
                break;

            case 5:
                registrarVendaMoto();
                registrarLog("Venda registrada.");
                pausar();
                break;

            case 6:
                gerarConfigJson();
                registrarLog("Config JSON gerado/atualizado.");
                pausar();
                break;

            case 7:
                mostrarConfigJson();
                pausar();
                break;

            case 8:
                verificarLogAtivo();
                pausar();
                break;

            case 9:
                criarBackup();
                registrarLog("Backup criado.");
                pausar();
                break;

            case 10:
                restaurarBackup();
                registrarLog("Backup restaurado.");
                pausar();
                break;

            case 11:
                verificarPermissoes();
                pausar();
                break;

            case 0:
                registrarLog("Sistema encerrado.");
                printf("\n  Encerrando o sistema. Ate logo!\n\n");
                break;

            default:
                printf("\n  Opcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
