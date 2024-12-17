# Nome do ficheiro fonte e executável
SRC = file.cpp
TARGET = 3

# Flags do compilador
CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall
LDFLAGS = -lm

# Regra principal
all: $(TARGET)

# Compilação
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

# Executar o programa
run: $(TARGET)
	./$(TARGET)

# Limpar ficheiros gerados
clean:
	rm -f $(TARGET)

# Fim do Makefile
