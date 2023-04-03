
CC = gcc
CPPC = g++
# CC = arm-none-linux-gnueabi-gcc
# CPPC = arm-none-linux-gnueabi-g++

OBJ_PATH=./obj
SRC_PATH=./src
INC_PATH=./inc
OUT_PATH=./install/
STATIC_LIB_PATH=./lib/a/
SHARED_LIB_PATH=./lib/so/

STATIC_LIB_INC=./lib/inc
SHARED_LIB_INC=./lib/inc
RPATH=./lib/inc

TARGET=$(OUT_PATH)fakeData

# LIB=-lm -lpthread -lrt -lcurl -leasylog -lssl -lcrypto -L $(STATIC_LIB_PATH) -L $(SHARED_LIB_PATH) -Wl,-rpath=$(RPATH)
LIB=-lm -lpthread -lrt -ldl -L $(STATIC_LIB_PATH) -L $(SHARED_LIB_PATH) -Wl,-rpath=$(RPATH)
CFLAGS= -g -I $(INC_PATH) -I $(STATIC_LIB_INC) -I $(SHARED_LIB_INC) $(LIB)
CPPFLAGS= -g  -I $(INC_PATH) -I $(STATIC_LIB_INC) -I $(SHARED_LIB_INC) $(LIB)

CSRCS=$(wildcard $(SRC_PATH)/*.c)
CPPSRCS=$(wildcard $(SRC_PATH)/*.cpp)

COBJS =$(patsubst $(SRC_PATH)/%.c, $(OBJ_PATH)/%.o, $(CSRCS))
CPPOBJS =$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(CPPSRCS))

.PHONY:all
all:$(TARGET)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC)  -c $< -o $@ $(CFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp  
	$(CPPC)  -c $< -o $@ $(CPPFLAGS)

$(TARGET): $(COBJS) $(CPPOBJS)
	$(CPPC) -o  $@ $(COBJS) $(CPPOBJS) $(CFLAGS) $(CPPFLAGS)

.PHONY:clean
clean:
	$(RM) $(TARGET) $(COBJS) $(CPPOBJS)
