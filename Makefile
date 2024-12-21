IDIR=-I./src/include
GAME_IDIR=-I./game_src/include
NETWORK_IDIR=-I./network/include
CC=gcc
CFLAGS=-g$(DIR)
CFLAGS_SHARED=-g$(DIR) -shared -fPIC

SRC=src
GAME_SRC=game_src
SERVER_SRC_DIR=network/server
CLIENT_SRC_DIR=network/client
LOBBY_SRC_DIR=network/lobby

ODIR=obj

MAIN = $(SRC)/main.c

# Basic guess variables
BASIC_GUESS_DELIVERABLE=$(GAME_SRC)/basic_guess/deliver_basic_guess.c
BASIC_GUESS_PLAYER=$(GAME_SRC)/basic_guess/player.c
BASIC_GUESS_MAIN=$(GAME_SRC)/basic_guess/basic_guess_main.c
BASIC_GUESS_TEST_GAME_P1=$(GAME_SRC)/basic_guess/test_game/p1_algo.c
BASIC_GUESS_TEST_GAME_P2=$(GAME_SRC)/basic_guess/test_game/p2_algo.c
BASIC_GUESS_SRC=$(MAIN) \
				$(BASIC_GUESS_MAIN) \
				$(BASIC_GUESS_DELIVERABLE) \
				$(BASIC_GUESS_PLAYER) \
				$(BASIC_GUESS_TEST_GAME_P1) \
				$(BASIC_GUESS_TEST_GAME_P2)

BASIC_GUESS_INC=$(GAME_IDIR)/basic_guess/
BASIC_GUESS_BIN_DIR=$(ODIR)/basic_guess
BASIC_GUESS_BIN=$(BASIC_GUESS_BIN_DIR)/basic_guess
# BASIC_GUESS_DELIVERABLE_BIN=$(BASIC_GUESS_BIN_DIR)/basic_guess_deliverable.so
BASIC_GUESS_CFLAGS=$(CFLAGS) -DBG=1

# Server variables
SERVER_MAIN=$(SERVER_SRC_DIR/server.c)
SERVER_INC=$(NETWORK_IDIR)/server
SERVER_BIN=$(ODIR)/server

main:
	$(CC) -o $(ODIR)/ai_war $(MAIN)

bg:
	# Deliverable
	# $(CC) $(BASIC_GUESS_CFLAGS) -o $(BASIC_GUESS_DELIVERABLE_BIN) $(BASIC_GUESS_INC) $(BASIC_GUESS_DELIVERIABLE)
	$(CC) $(BASIC_GUESS_CFLAGS) -o $(BASIC_GUESS_BIN) $(BASIC_GUESS_INC) $(BASIC_GUESS_SRC)

server:
	$(CC) $(CFLAGS) -o $(SERVER_BIN) $(SERVER_INC) $(SERVER_MAIN)




clean:
	rm -r $(ODIR)/*
