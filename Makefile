CC       = gcc
FLAGS    = -Wall -Wextra -g

INCLUDES = -Iinclude
TARGET   = concessionaria
SRC_DIR  = src
OBJ_DIR  = obj

SRCS     = main.c \
           $(SRC_DIR)/arquivos.c \
           $(SRC_DIR)/estoque.c \
           $(SRC_DIR)/log.c \
           $(SRC_DIR)/menu.c \
           $(SRC_DIR)/permissoes.c

OBJS     = $(OBJ_DIR)/main.o \
           $(OBJ_DIR)/arquivos.o \
           $(OBJ_DIR)/estoque.o \
           $(OBJ_DIR)/log.o \
           $(OBJ_DIR)/menu.o \
           $(OBJ_DIR)/permissoes.o

all: $(OBJ_DIR) $(TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(TARGET)

# Regra para main.c que está na raiz
$(OBJ_DIR)/main.o: main.c
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

# Regra para todos os .c dentro de src/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -rf $(OBJ_DIR)

.PHONY: all run clean