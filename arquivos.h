#ifndef ARQUIVOS_H
#define ARQUIVOS_H

void criarEstruturaPastas();
void gerarConfigJson();
void mostrarConfigJson();
void verificarLogAtivo();
void criarBackup();
void restaurarBackup();

// Lê o nome da loja do config.json e guarda em saida
void lerNomeLoja(char *saida, int tamanho);

// Usada internamente pelo log.c
int logEstaAtivo();

#endif
