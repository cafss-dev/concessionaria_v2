CC     = gcc
FLAGS  = -Wall -Wextra
TARGET = concessionaria
SRCS   = main.c menu.c estoque.c arquivos.c log.c permissoes.c

all:
	$(CC) $(FLAGS) $(SRCS) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
