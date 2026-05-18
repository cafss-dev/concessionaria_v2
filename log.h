#ifndef LOG_H
#define LOG_H

// Registra uma mensagem no arquivo logs/sistema.log com data e hora
// Só grava se logAtivo = 1 no config.json
void registrarLog(const char *mensagem);

#endif
