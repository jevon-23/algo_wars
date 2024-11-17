IDIR=-I./src/include
GAME_IDIR=-I./game_src/include
CC=gcc
CFLAGS=-g$(DIR)

SRC=src
GAME_SRC=game_src

ODIR=obj

MAIN = $(SRC)/main.c

BASIC_GUESS=$(GAME_SRC)/basic_guess/basic_guess_main.c
BASIC_GUESS_INC=$(GAME_IDIR)/basic_guess/
BASIC_GUESS_SRC=$(MAIN) $(BASIC_GUESS)
BASIC_GUESS_BIN=$(ODIR)/basic_guess
BASIC_GUESS_CFLAGS=$(CFLAGS) -DBG=1

main:
	$(CC) -o $(ODIR)/ai_war $(MAIN)

bg:
	$(CC) $(BASIC_GUESS_CFLAGS) -o $(BASIC_GUESS_BIN) $(BASIC_GUESS_INC) $(BASIC_GUESS_SRC)
