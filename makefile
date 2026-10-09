# ==========================================
# Makefile para Asteroids
# ==========================================

# 1. Compilador y banderas
CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++14

# 2. Definición de directorios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# 3. Dónde buscar los .h
INCLUDES = -I$(INC_DIR) -I.

# 4. Librerías de Raylib
LDFLAGS = ./lib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11

# 5. Detección automática de archivos
# Encuentra todos los .cpp en la carpeta src/
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Transforma los nombres de src/archivo.cpp a obj/archivo.o
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# 6. Nombre del ejecutable
EXEC = ./bin/juego_asteroids

# ==========================================
# Reglas
# ==========================================

all: $(EXEC)

# Enlazado final
$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC) $(LDFLAGS)

# Compilación de cada .cpp a .o
# El "| $(OBJ_DIR)" asegura que la carpeta obj exista antes de compilar
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Regla para crear la carpeta obj/ si por algún casual no existe
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Limpieza profunda
clean:
	rm -rf $(OBJ_DIR) $(EXEC)
