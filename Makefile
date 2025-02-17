IDIR=-I./src/include
GAME_IDIR=-I./game_src/include
NETWORK_IDIR=-I./network/include
CC=gcc
CFLAGS=-g$(DIR)
CFLAGS_SHARED=-g$(DIR) -shared -fPIC

SRC=src
GAME_SRC=game_src
NETWORK_SRC_DIR=network
CLIENT_SRC_DIR=$(NETWORK_SRC_DIR)/client
LIB_SRC_DIR=$(NETWORK_SRC_DIR)/lib
LOBBY_SRC_DIR=$(NETWORK_SRC_DIR)/lobby
SERVER_SRC_DIR=$(NETWORK_SRC_DIR)/server

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

# Common Variables
COMMON_SRC=$(LIB_SRC_DIR)/common.c
COMMON_INC=$(NETWORK_IDIR)/lib/

# Lobby Variables
LOBBY_SRC=$(LOBBY_SRC_DIR)/lobby.c
LOBBY_INC=$(NETWORK_IDIR)/lobby

# Server variables
SERVER_MAIN=$(SERVER_SRC_DIR)/server.c
SERVER_INC=$(NETWORK_IDIR)/server \
		   $(LOBBY_INC) \
		   $(COMMON_INC)

SERVER_BIN=$(ODIR)/server

SERVER_SRC=$(SERVER_MAIN) \
			$(SERVER_SRC_DIR)/client.c \
			$(COMMON_SRC) \
			$(LOBBY_SRC)

SERVER_FLAGS=$(CC) \
			 $(CFLAGS) \
			 -DSERVER=1 \
			 -pthread


# Client variables
CLIENT_MAIN=$(CLIENT_SRC_DIR)/client.c
CLIENT_INC=$(NETWORK_IDIR)/client \
		   $(LOBBY_INC) \
		   $(COMMON_INC)

CLIENT_BIN=$(ODIR)/client
CLIENT_SRC=$(CLIENT_MAIN) \
		   $(COMMON_SRC) \
		   $(LOBBY_SRC)

CLIENT_FLAGS=$(CC) \
			 $(CFLAGS) \
			 -DCLIENT=1

main:
	$(CC) -o $(ODIR)/ai_war $(MAIN)

bg:
	# Deliverable
	# $(CC) $(BASIC_GUESS_CFLAGS) -o $(BASIC_GUESS_DELIVERABLE_BIN) $(BASIC_GUESS_INC) $(BASIC_GUESS_DELIVERIABLE)
	$(CC) $(BASIC_GUESS_CFLAGS) -o $(BASIC_GUESS_BIN) $(BASIC_GUESS_INC) $(BASIC_GUESS_SRC)

server:
	$(SERVER_FLAGS) -o $(SERVER_BIN) $(SERVER_INC) $(SERVER_SRC)

client:
	$(CLIENT_FLAGS) -o $(CLIENT_BIN) $(CLIENT_INC) $(CLIENT_SRC)

net: client server

clean:
	rm -r $(ODIR)/*
