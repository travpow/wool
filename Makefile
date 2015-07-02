
make: all

CC = gcc
BRIDGE_FLAGS = -undefined dynamic_lookup -dynamiclib -fPIC -shared
ERL_INC = /usr/local/Cellar/erlang/R14B02/lib/erlang/usr/include/

BIN=bin
TARGETS = \
          $(NIF) \
          $(ERL_BRIDGE) \

NIF = wobridge.so
ERL_BRIDGE = $(ERL_SRCS:.erl=.beam)

NIF_SRCS = \
		   WoErlangBridge.cpp \
		   WoMessage.cpp \

ERL_SRCS = \
           wo_bridge.erl \

$(BIN):
	@mkdir $(BIN)

$(ERL_BRIDGE): $(ERL_SRCS) | $(BIN)
	@erlc -o $(BIN) $(ERL_SRCS)

$(NIF): $(NIF_SRCS) | $(BIN)
	@$(CC) $(BRIDGE_FLAGS) -o $(BIN)/$(NIF) -I $(ERL_INC) $(NIF_SRCS)

all: $(TARGETS)
clean:
	@rm $(BIN)/*
