# ===============================
# Compiladores y flags
# ===============================
CC  = gcc
CXX = g++
CFLAGS = -Wall -g
CXXFLAGS = -Wall -g

# ===============================
# Directorios
# ===============================
SRC_DIR_CAP1 = SRC/Cap1
BIN_DIR_CAP1 = bin/Cap1

SRC_DIR_CAP2 = SRC/Cap2
BIN_DIR_CAP2 = bin/Cap2

SRC_DIR_CAP3 = SRC/Cap3
BIN_DIR_CAP3 = bin/Cap3

SRC_DIR_CAP4 = SRC/Cap4
BIN_DIR_CAP4 = bin/Cap4

SRC_DIR_CAP5 = SRC/Cap5
BIN_DIR_CAP5 = bin/Cap5

# ===============================
# CAPÍTULO 1 (ejecutable único)
# ===============================
CAP1_SRC_C   = $(SRC_DIR_CAP1)/Listing1.1.c
CAP1_SRC_CPP = $(SRC_DIR_CAP1)/Listing1.2.cpp
CAP1_HDR     = $(SRC_DIR_CAP1)/Listing1.3.hpp

CAP1_OBJ = $(BIN_DIR_CAP1)/Listing1.1.o $(BIN_DIR_CAP1)/Listing1.2.o
CAP1_BIN = $(BIN_DIR_CAP1)/programa_cap1

# ===============================
# CAPÍTULO 2
# ===============================
CAP2_SRC = $(wildcard $(SRC_DIR_CAP2)/Listing2*.c)

# Ejecutables (excepto 2.7 y 2.9 que son especiales)
CAP2_EXE = $(patsubst $(SRC_DIR_CAP2)/%.c,$(BIN_DIR_CAP2)/%,$(filter-out $(SRC_DIR_CAP2)/Listing2.7.c $(SRC_DIR_CAP2)/Listing2.8.c $(SRC_DIR_CAP2)/Listing2.9.c,$(CAP2_SRC)))

# Librería del 2.7
CAP2_LIB = $(BIN_DIR_CAP2)/libtest.a

# ===============================
# CAPÍTULO 3
# ===============================
CAP3_SRC  = $(wildcard $(SRC_DIR_CAP3)/*.c)
CAP3_BINS = $(patsubst $(SRC_DIR_CAP3)/%.c,$(BIN_DIR_CAP3)/%,$(CAP3_SRC))

# ===============================
# CAPÍTULO 4
# ===============================
CAP4_SRC_C   = $(wildcard $(SRC_DIR_CAP4)/*.c)
CAP4_SRC_CPP = $(wildcard $(SRC_DIR_CAP4)/*.cpp)
CAP4_BINS_C  = $(patsubst $(SRC_DIR_CAP4)/%.c,$(BIN_DIR_CAP4)/%,$(CAP4_SRC_C))
CAP4_BINS_CPP= $(patsubst $(SRC_DIR_CAP4)/%.cpp,$(BIN_DIR_CAP4)/%,$(CAP4_SRC_CPP))

# ===============================
# CAPÍTULO 5
# ===============================
CAP5_SRC  = $(wildcard $(SRC_DIR_CAP5)/*.c)
CAP5_BINS = $(patsubst $(SRC_DIR_CAP5)/%.c,$(BIN_DIR_CAP5)/%,$(CAP5_SRC))

# ===============================
# OBJETIVO PRINCIPAL
# ===============================
all: cap1 cap2 cap3 cap4 cap5

# ---------------- CAPÍTULO 1 ----------------
cap1: $(CAP1_BIN)

$(CAP1_BIN): $(CAP1_OBJ)
	@mkdir -p $(BIN_DIR_CAP1)
	$(CXX) -o $@ $(CAP1_OBJ)

$(BIN_DIR_CAP1)/Listing1.1.o: $(CAP1_SRC_C) $(CAP1_HDR)
	@mkdir -p $(BIN_DIR_CAP1)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR_CAP1)/Listing1.2.o: $(CAP1_SRC_CPP) $(CAP1_HDR)
	@mkdir -p $(BIN_DIR_CAP1)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ---------------- CAPÍTULO 2 ----------------
cap2: $(CAP2_EXE) $(CAP2_LIB) $(BIN_DIR_CAP2)/Listing2.8 $(BIN_DIR_CAP2)/Listing2.9 copy_test copy_tiff

# Regla genérica (todos menos 2.7, 2.8 y 2.9)
$(BIN_DIR_CAP2)/Listing2.%: $(SRC_DIR_CAP2)/Listing2.%.c | $(BIN_DIR_CAP2)
	$(CC) $(CFLAGS) $< -o $@

# Librería de 2.7
$(CAP2_LIB): $(SRC_DIR_CAP2)/Listing2.7.c | $(BIN_DIR_CAP2)
	$(CC) $(CFLAGS) -c $< -o $(BIN_DIR_CAP2)/Listing2.7.o
	ar cr $@ $(BIN_DIR_CAP2)/Listing2.7.o

# Programa 2.8 (enlaza con la librería del 2.7)
$(BIN_DIR_CAP2)/Listing2.8: $(SRC_DIR_CAP2)/Listing2.8.c $(CAP2_LIB) | $(BIN_DIR_CAP2)
	$(CC) $(CFLAGS) $< -L$(BIN_DIR_CAP2) -ltest -o $@

# Programa 2.9 (requiere libtiff)
$(BIN_DIR_CAP2)/Listing2.9: $(SRC_DIR_CAP2)/Listing2.9.c | $(BIN_DIR_CAP2)
	$(CC) $(CFLAGS) $< -ltiff -o $@

# Copiar archivos necesarios para 2.6 y 2.9
copy_test: | $(BIN_DIR_CAP2)
	cp $(SRC_DIR_CAP2)/test.txt $(BIN_DIR_CAP2)/

copy_tiff: | $(BIN_DIR_CAP2)
	@if [ -f $(SRC_DIR_CAP2)/prueba2.9lp.tiff ]; then cp $(SRC_DIR_CAP2)/prueba2.9lp.tiff $(BIN_DIR_CAP2)/; fi

# ---------------- CAPÍTULO 3 ----------------
cap3: $(CAP3_BINS)

$(BIN_DIR_CAP3)/%: $(SRC_DIR_CAP3)/%.c | $(BIN_DIR_CAP3)
	$(CC) $(CFLAGS) $< -o $@

# ---------------- CAPÍTULO 4 ----------------
cap4: $(CAP4_BINS_C) $(CAP4_BINS_CPP)

$(BIN_DIR_CAP4)/%: $(SRC_DIR_CAP4)/%.c | $(BIN_DIR_CAP4)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR_CAP4)/%: $(SRC_DIR_CAP4)/%.cpp | $(BIN_DIR_CAP4)
	$(CXX) $(CXXFLAGS) $< -o $@

# ---------------- CAPÍTULO 5 ----------------
cap5: $(CAP5_BINS)

$(BIN_DIR_CAP5)/%: $(SRC_DIR_CAP5)/%.c | $(BIN_DIR_CAP5)
	$(CC) $(CFLAGS) $< -o $@

# ---------------- DIRECTORIOS ----------------
$(BIN_DIR_CAP1):
	mkdir -p $(BIN_DIR_CAP1)

$(BIN_DIR_CAP2):
	mkdir -p $(BIN_DIR_CAP2)

$(BIN_DIR_CAP3):
	mkdir -p $(BIN_DIR_CAP3)

$(BIN_DIR_CAP4):
	mkdir -p $(BIN_DIR_CAP4)

$(BIN_DIR_CAP5):
	mkdir -p $(BIN_DIR_CAP5)

# ---------------- LIMPIEZA ----------------
clean:
	rm -rf bin/Cap1 bin/Cap2 bin/Cap3 bin/Cap4 bin/Cap5

