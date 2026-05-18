#include <stdio.h>
#include <time.h>
#include "log.h"
#include "arquivos.h"   // para logEstaAtivo()

void registrarLog(const char *mensagem)
{
    // Só registra se o usuário habilitou o log no config.json
    if (!logEstaAtivo()) return;

    FILE *log = fopen("logs/sistema.log", "a");
    if (log == NULL) return;

    // Captura a data e hora atual do sistema
    time_t agora = time(NULL);
    struct tm *t  = localtime(&agora);

    fprintf(log, "[%02d/%02d/%04d %02d:%02d:%02d] %s\n",
            t->tm_mday,
            t->tm_mon + 1,       // tm_mon começa em 0 (janeiro = 0)
            t->tm_year + 1900,   // tm_year conta a partir de 1900
            t->tm_hour,
            t->tm_min,
            t->tm_sec,
            mensagem);

    fclose(log);
}
