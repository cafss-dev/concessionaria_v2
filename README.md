# CONCESSIONARIA V2
- Sistema de concessionaria via terminal
- Faz Cadastramento, Reserva e Registro de Venda dos veículos automotivos da concessionária
- Projeto feito para trabalho universitário

## Requisitos
- GCC: `sudo apt install gcc`
- Make: `sudo apt install make`

## Como compilar
Para compilar é necessário ter o **GCC** e o **Make** instalados.

## Como executar
- Clonar o Projeto Localmente
```
    git clone https://github.com/cafss-dev/concessionaria
```
- Terminal
```
    cd concessionaria_v2
```
- MakeFile
```
    make
```
- Iniciar o Projeto
```
    ./concessionaria
```

## Estrutura do projeto
concessionaria_v2/
├── src/          # Código-fonte dos módulos
├── include/      # Cabeçalhos (.h)
├── data/
│   ├── config/   # Configurações do sistema
│   └── estoque/  # Dados do estoque
├── logs/         # Gerado em execução
├── main.c        # Ponto de entrada
├── Makefile      # Script de compilação
├── LICENSE
└── .gitignore

## Autor
- Carlos Augusto Ferreira Silva Sousa