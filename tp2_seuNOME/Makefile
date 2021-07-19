###############################################################################
########################### Parametros de Projeto #############################
###############################################################################

# TODO: altere a extensão dos arquivos (c ou cpp)
EXT = cpp
# TODO: altere a lista de forma que contenha todos os arquivos .cpp
#       do seu projeto (sem a extensão), que deve estar no diretório 
#       src/
SRCS_LIGADOR = $(patsubst %,%.$(EXT),main ligador)
OBJS_LIGADOR = $(patsubst %,build/%.o,$(basename $(SRCS_LIGADOR)))

###############################################################################
########################### Parametros de Ambiente ############################
###############################################################################

CC = gcc
CXX = g++
CCFLAGS = -Wall
CXXFLAGS = -Wall -g
CPPFLAGS = -Iinclude/

###############################################################################
################################ Executáveis ##################################
###############################################################################

all: bin/ligador

ifeq ($(EXT), c)
bin/ligador: $(OBJS_LIGADOR)
	@echo "+ Compilando programa \"$@\""
	@$(CC) $(CCFLAGS) $(OBJS_LIGADOR) -o bin/ligador
else
bin/ligador: $(OBJS_LIGADOR)
	@echo "+ Compilando programa \"$@\""
	@$(CXX) $(CXXFLAGS) $(OBJS_LIGADOR) -o bin/ligador
endif

###############################################################################
################################## Objetos ####################################
###############################################################################

ifeq ($(EXT), c)
build/%.o: src/%.c
	@echo "- Compilando objeto \"$@\""
	@$(CC) $(CPPFLAGS) $(CCFLAGS) -c $< -o $@
else
build/%.o: src/%.cpp
	@echo "- Compilando objeto \"$@\""
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
endif

###############################################################################
################################# Auxiliares ##################################
###############################################################################

clean:
	rm -f bin/* $(OBJS_LIGADOR)
