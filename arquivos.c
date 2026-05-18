#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include "arquivos.h"

// CRIAR ESTRUTURA DE PASTAS
void criarEstruturaPastas()
{
    char *pastas[] = {"estoque", "logs", "backup", "config"};
    int total = 4;

    for (int i = 0; i < total; i++)
    {
        if (mkdir(pastas[i], 0777) == 0)
            printf("  [OK] Pasta '%s' criada.\n", pastas[i]);
    }
}

// LER NOME DA LOJA
// Extrai o valor de "nome" do config.json
void lerNomeLoja(char *saida, int tamanho)
{
    FILE *f = fopen("config/config.json", "r");
    if (f == NULL)
    {
        snprintf(saida, tamanho, "SEM CONFIGURACAO");
        return;
    }

    char linha[200];
    while (fgets(linha, sizeof(linha), f) != NULL)
    {
        if (strstr(linha, "\"nome\"") != NULL)
        {
            // Avança até os dois pontos, depois até as aspas
            char *p = strchr(linha, ':');
            if (p != NULL)
            {
                p = strchr(p, '"');
                if (p != NULL)
                {
                    p++; // pula a aspas de abertura
                    char *fim = strchr(p, '"');
                    if (fim != NULL)
                    {
                        *fim = '\0';
                        snprintf(saida, tamanho, "%s", p);
                        fclose(f);
                        return;
                    }
                }
            }
        }
    }

    fclose(f);
    snprintf(saida, tamanho, "SEM CONFIGURACAO");
}

// GERAR CONFIG.JSON
void gerarConfigJson()
{
    char nome[100], cnpj[20], gerente[100];
    int  logAtivo;

    printf("\n----- GERAR CONFIG.JSON -----\n");
    printf("Nome da concessionaria : "); scanf(" %[^\n]", nome);
    printf("CNPJ                   : "); scanf(" %[^\n]", cnpj);
    printf("Nome do gerente        : "); scanf(" %[^\n]", gerente);
    printf("Log ativo? (1=sim 0=nao): "); scanf("%d", &logAtivo);

    FILE *f = fopen("config/config.json", "w");
    if (f == NULL)
    {
        printf("Erro ao criar config.json!\n");
        return;
    }

    fprintf(f, "{\n");
    fprintf(f, "  \"nome\"     : \"%s\",\n", nome);
    fprintf(f, "  \"cnpj\"     : \"%s\",\n", cnpj);
    fprintf(f, "  \"gerente\"  : \"%s\",\n", gerente);
    fprintf(f, "  \"logAtivo\" : %d\n",       logAtivo);
    fprintf(f, "}\n");

    fclose(f);
    printf("config.json gerado com sucesso!\n");
}

// MOSTRAR CONFIG.JSON
void mostrarConfigJson()
{
    FILE *f = fopen("config/config.json", "r");
    if (f == NULL)
    {
        printf("\nconfig.json nao encontrado. Use a opcao 6 para gerar.\n");
        return;
    }

    printf("\n----- CONFIG.JSON -----\n");
    char linha[200];
    while (fgets(linha, sizeof(linha), f) != NULL)
        printf("%s", linha);

    fclose(f);
}

// logEstaAtivo — verifica se "logAtivo" é 1 no config.json
int logEstaAtivo()
{
    FILE *f = fopen("config/config.json", "r");
    if (f == NULL) return 0;

    char linha[200];
    while (fgets(linha, sizeof(linha), f) != NULL)
    {
        if (strstr(linha, "\"logAtivo\"") != NULL)
        {
            fclose(f);
            char *p = strchr(linha, ':');
            if (p != NULL)
                return atoi(p + 1);
        }
    }

    fclose(f);
    return 0;
}

// VERIFICAR LOG ATIVO — opção de menu
void verificarLogAtivo()
{
    if (logEstaAtivo())
        printf("\nRegistro de log esta ATIVO. Eventos estao sendo gravados.\n");
    else
        printf("\nRegistro de log esta INATIVO. Use a opcao 6 para configurar.\n");
}

// AUXILIAR — copia um arquivo de origem para destino
static void copiarArquivo(const char *origem, const char *destino)
{
    FILE *src = fopen(origem, "r");
    if (src == NULL)
    {
        printf("  [--] '%s' nao encontrado, pulando.\n", origem);
        return;
    }

    FILE *dst = fopen(destino, "w");
    if (dst == NULL)
    {
        printf("  [ERRO] Nao foi possivel criar '%s'.\n", destino);
        fclose(src);
        return;
    }

    char ch;
    while ((ch = fgetc(src)) != EOF)
        fputc(ch, dst);

    fclose(src);
    fclose(dst);
    printf("  [OK] '%s' copiado.\n", origem);
}

// CRIAR BACKUP
// Salva os arquivos numa subpasta com o nome da loja dentro de backup/
// Exemplo: backup/MOTO FAST/config.json
//                           motos.txt
void criarBackup()
{
    char nomeLoja[100];
    lerNomeLoja(nomeLoja, sizeof(nomeLoja));

    // Monta o caminho da pasta de backup desta loja
    char caminhoPasta[250];
    snprintf(caminhoPasta, sizeof(caminhoPasta), "backup/%s", nomeLoja);

    // Cria a subpasta se não existir
    if (mkdir(caminhoPasta, 0777) == -1)
    {
        // Pasta já existe — tudo bem, só vai sobrescrever os arquivos
    }

    // Monta os caminhos completos dos arquivos de destino
    char destConfig[350], destMotos[350];
    snprintf(destConfig, sizeof(destConfig), "%s/config.json", caminhoPasta);
    snprintf(destMotos,  sizeof(destMotos),  "%s/motos.txt",   caminhoPasta);

    printf("\n----- CRIANDO BACKUP -----\n");
    printf("Loja  : %s\n", nomeLoja);
    printf("Pasta : %s\n\n", caminhoPasta);

    copiarArquivo("config/config.json", destConfig);
    copiarArquivo("estoque/motos.txt",  destMotos);

    printf("\nBackup concluido!\n");
}

// RESTAURAR BACKUP
// Lista as pastas dentro de backup/, o usuário escolhe qual restaurar
// e os arquivos são copiados de volta para config/ e estoque/
void restaurarBackup()
{
    DIR *dir = opendir("backup");
    if (dir == NULL)
    {
        printf("\nPasta backup/ nao encontrada.\n");
        return;
    }

    // Lê os nomes das subpastas (uma por loja)
    char lojas[50][256];
    int totalLojas = 0;
    struct dirent *entrada;

    printf("\n----- BACKUPS DISPONIVEIS -----\n");

    while ((entrada = readdir(dir)) != NULL)
    {
        // Ignora as entradas especiais . e ..
        if (entrada->d_name[0] == '.') continue;

        // Verifica se é realmente uma pasta usando stat()
        char caminho[300];
        snprintf(caminho, sizeof(caminho), "backup/%s", entrada->d_name);

        struct stat st;
        if (stat(caminho, &st) == 0 && S_ISDIR(st.st_mode))
        {
            snprintf(lojas[totalLojas], 256, "%s", entrada->d_name);
            printf("  %d. %s\n", totalLojas + 1, lojas[totalLojas]);
            totalLojas++;
        }
    }

    closedir(dir);

    if (totalLojas == 0)
    {
        printf("Nenhum backup encontrado. Crie um backup primeiro.\n");
        return;
    }

    int escolha;
    printf("\nQual backup deseja restaurar? (1-%d): ", totalLojas);
    scanf("%d", &escolha);

    if (escolha < 1 || escolha > totalLojas)
    {
        printf("Opcao invalida.\n");
        return;
    }

    char caminhoPasta[300];
    snprintf(caminhoPasta, sizeof(caminhoPasta), "backup/%s", lojas[escolha - 1]);

    char origemConfig[400], origemMotos[400];
    snprintf(origemConfig, sizeof(origemConfig), "%s/config.json", caminhoPasta);
    snprintf(origemMotos,  sizeof(origemMotos),  "%s/motos.txt",   caminhoPasta);

    printf("\nRestaurando backup de '%s'...\n\n", lojas[escolha - 1]);
    copiarArquivo(origemConfig, "config/config.json");
    copiarArquivo(origemMotos,  "estoque/motos.txt");

    printf("\nBackup restaurado com sucesso!\n");
    printf("Reinicie o sistema para aplicar as configuracoes.\n");
}
