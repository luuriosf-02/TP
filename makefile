# ===========================
# Compiladores y flags
# ===========================
CC  = gcc
CXX = g++
CFLAGS = -Wall -g
CXXFLAGS = -Wall -g

# ===========================
# Directorios
# ===========================
SRC_DIR_CAP1 = SRC/Cap1
BIN_DIR_CAP1 = bin/Cap1

SRC_DIR_CAP2 = SRC/Cap2
BIN_DIR_CAP2 = bin/Cap2

SRC_DIR_CAP3 = SRC/Cap3
BIN_DIR_CAP3 = bin/Cap3

SRC_DIR_CAP4 = SRC/Cap4
BIN_DIR_CAP4 = bin/Cap4

# ===========================
# CAPÍTULO 1 (ejecutable único)
# ===========================
CAP1_SRC_C   = $(SRC_DIR_CAP1)/Listing1.1.c
CAP1_SRC_CPP = $(SRC_DIR_CAP1)/Listing1.2.cpp
CAP1_HDR     = $(SRC_DIR_CAP1)/Listing1.3.hpp

CAP1_OBJ = $(BIN_DIR_CAP1)/Listing1.1.o $(BIN_DIR_CAP1)/Listing1.2.o
CAP1_BIN = $(BIN_DIR_CAP1)/programa_cap1

# ===========================
# CAPÍTULO 2 (cada listing = ejecutable)
# ===========================
CAP2_SRC  = $(wildcard $(SRC_DIR_CAP2)/Listing2*.c)
CAP2_BINS = $(patsubst $(SRC_DIR_CAP2)/%.c,$(BIN_DIR_CAP2)/%,$(CAP2_SRC))

# Listados especiales con libtiff
TIFF_SRC  = Listing2.7.c Listing2.8.c Listing2.9.c
TIFF_BINS = $(patsubst %.c,$(BIN_DIR_CAP2)/%,$(TIFF_SRC))

# ===========================
# CAPÍTULO 3 (cada listing = ejecutable)
# ===========================
CAP3_SRC  = $(wildcard $(SRC_DIR_CAP3)/*.c)
CAP3_BINS = $(patsubst $(SRC_DIR_CAP3)/%.c,$(BIN_DIR_CAP3)/%,$(CAP3_SRC))

# ===========================
# CAPÍTULO 4 (cada listing = ejecutable)
# ===========================
CAP4_SRC  = $(wildcard $(SRC_DIR_CAP4)/*.c)
CAP4_BINS = $(patsubst $(SRC_DIR_CAP4)/%.c,$(BIN_DIR_CAP4)/%,$(CAP4_SRC))

# ===========================
# OBJETIVO PRINCIPAL
# ===========================
all: cap1 cap2 cap3 cap4

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
cap2: $(BIN_DIR_CAP2) $(CAP2_BINS) $(BIN_DIR_CAP2)/test.txt

$(BIN_DIR_CAP2):
	@mkdir -p $(BIN_DIR_CAP2)

$(BIN_DIR_CAP2)/%: $(SRC_DIR_CAP2)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(TIFF_BINS): $(BIN_DIR_CAP2)/%: $(SRC_DIR_CAP2)/%.c
	$(CC) $(CFLAGS) $< -o $@ -ltiff

$(BIN_DIR_CAP2)/test.txt: $(SRC_DIR_CAP2)/test.txt
	@mkdir -p $(BIN_DIR_CAP2)
	cp $< $@

# ---------------- CAPÍTULO 3 ----------------
cap3: $(CAP3_BINS)

$(BIN_DIR_CAP3)/%: $(SRC_DIR_CAP3)/%.c
	@mkdir -p $(BIN_DIR_CAP3)
	$(CC) $(CFLAGS) $< -o $@

# ---------------- CAPÍTULO 4 ----------------
cap4: $(CAP4_BINS)

$(BIN_DIR_CAP4)/%: $(SRC_DIR_CAP4)/%.c
	@mkdir -p $(BIN_DIR_CAP4)
	$(CC) $(CFLAGS) $< -o $@

# ---------------- LIMPIEZA ----------------
clean:
	rm -rf $(BIN_DIR_CAP1) $(BIN_DIR_CAP2) $(BIN_DIR_CAP3) $(BIN_DIR_CAP4)

