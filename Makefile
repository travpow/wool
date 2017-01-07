
include std_makevars

TARGETS = \
          $(NIF) \
          $(ERL_BRIDGE) \
		  $(ERL_BRIDGE_STATIC) \

NIF = wobridge.so
ERL_BRIDGE = $(ERL_SRCS:.erl=.beam)
ERL_BRIDGE_STATIC = wobridge_static

SRCS = \
	   src/WoErlangBridge.cpp \
       src/WoMessage.cpp \
	   src/WoShmQueue.cpp \
	   src/WoShmAllocator.cpp \

ERL_SRCS = \
       erl/wo_bridge.erl \

$(ERL_BRIDGE): $(ERL_SRCS) | $(BIN)
	$(ERLC) -o $(BIN) $(ERL_SRCS)

$(NIF): $(SRCS) | $(BIN)
	$(CC) $(BRIDGE_FLAGS) -o $(BIN)/$(NIF) -I $(ERL_INC) $(SRCS) 

$(ERL_BRIDGE_STATIC): $(SRCS) | $(BIN)
	$(CC) $(CC_FLAGS) -o $(BIN)/$(ERL_BRIDGE_STATIC) -I $(ERL_INC) $(SRCS)


include std_makevars
