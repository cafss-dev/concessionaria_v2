CC       = gcc
FLAGS    = -Wall -Wextra -g

# Aponta para a pasta onde estão os arquivos .h
INCLUDES = -Iinclude

TARGET   = concessionaria

# Define as pastas do projeto
SRC_DIR  = src
OBJ_DIR  = obj

# Lista os arquivos fonte (adicionando main.c que está na raiz)
SRCS     = main.c \
           $(SRC_DIR)/arquivos.c \
           $(SRC_DIR)/estoque.c \
           $(SRC_DIR)/log.c \
           $(SRC_DIR)/menu.c \
           $(SRC_DIR)/permissoes.c

# Mapeia os arquivos .c para arquivos .o dentro da pasta obj/
OBJS     = main.o \
           $(SRC_DIR:$(SRC_DIR)=$(OBJ_DIR))/arquivos.o \
           $(SRC_DIR:$(SRC_DIR)=$(OBJ_DIR))/estoque.o \
           $(SRC_DIR:$(SRC_DIR)=$(OBJ_DIR))/log.o \
           $(SRC_DIR:$(SRC_DIR)=$(OBJ_DIR))/menu.o \
           $(SRC_DIR:$(SRC_DIR)=$(OBJ_DIR))/permissoes.o

# Alvo padrão
all: $(OBJ_DIR) $(TARGET)

# Cria a pasta obj se ela não existir
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Linkagem final do executável
$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(TARGET)

# Regra para compilar o main.c (que está na raiz)
main.o: main.c
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

# Regra para compilar os arquivos de dentro da pasta src/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

# Executa o programa
run: all
	./$(TARGET)

# Limpeza completa (apaga o executável e a pasta de objetos)
clean:
	rm -f $(TARGET) main.o
	rm -rf $(OBJ_DIR)

.PHONY: all run clean
