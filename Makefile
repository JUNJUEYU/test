# CC = gcc
# CXXC = g++
CC = arm-none-linux-gnueabi-gcc
CXXC = arm-none-linux-gnueabi-g++

OBJ_PATH=./obj
SRC_PATH=./src
INC_PATH=./inc
OUT_PATH=./install/
STATIC_LIB_PATH=	-L ./lib
SHARED_LIB_PATH=	-L ./lib \
					-L /home/yjj/library/paho.mqtt.c-master/install/feiling/lib \
					-L /home/yjj/work/gw/lib/libcurl/lib \
					-L /home/yjj/library/src/openssl-1.1.0l/install/lib \
					-L  /home/yjj/work/gw/lib/libcurl/lib  -Wl,-rpath=$(RPATH)

STATIC_LIB_INC=		-I ./lib/inc
SHARED_LIB_INC=		-I ./lib/inc \
					-I /home/yjj/library/paho.mqtt.c-master/install/feiling/include\
					-I /home/yjj/work/gw/lib/libcurl/include \
					-I /home/yjj/library/src/openssl-1.1.0l/install/include \
					-I /home/yjj/work/gw/lib/libcurl/include

RPATH=./lib/inc

TARGET=$(OUT_PATH)gw



LIB=-lm -lpthread -lrt -lcurl -leasylog -lssl -lcrypto -lpaho-mqtt3as -lpaho-mqtt3c $(STATIC_LIB_PATH)  $(SHARED_LIB_PATH)
# LIB=-lm -lpthread -lrt -ldl -lpaho-mqtt3as -L $(STATIC_LIB_PATH) -L $(SHARED_LIB_PATH) -Wl,-rpath=$(RPATH)
CFLAGS= -g -I $(INC_PATH)  $(STATIC_LIB_INC)  $(SHARED_LIB_INC) $(LIB)
CXXFLAGS= -g3  -I $(INC_PATH)  $(STATIC_LIB_INC)  $(SHARED_LIB_INC) $(LIB) 

CSRCS=$(wildcard $(SRC_PATH)/*.c)
CXXSRCS=$(wildcard $(SRC_PATH)/*.cpp)

COBJS =$(patsubst $(SRC_PATH)/%.c, $(OBJ_PATH)/%.o, $(CSRCS))
CXXOBJS =$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(CXXSRCS))

.PHONY:all
all:$(TARGET)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC)  -c $< -o $@ $(CFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp  
	$(CXXC)  -c $< -o $@ $(CXXFLAGS)

$(TARGET): $(COBJS) $(CXXOBJS)
	$(CXXC) -o  $@ $(CFLAGS) $(CXXFLAGS) $(COBJS) $(CXXOBJS) 


.PHONY:install
install:
	cp $(TARGET) $(OUT_PATH)


TEST=./test/out/test
TEST_OBJ_PATH=./test/obj
TEST_SRC_PATH=./test/src
TEST_INC_PATH=./test/inc
TEST_CXXSRCS=$(wildcard $(TEST_SRC_PATH)/*.cpp) 
TEST_COBJS =$(patsubst $(TEST_SRC_PATH)/%.cpp, $(TEST_OBJ_PATH)/%.o, $(TEST_CXXSRCS))

.PHONY:test
test:$(TEST)
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC)  -c $< -o $@ $(CFLAGS) -I $(TEST_INC_PATH)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp  
	$(CXXC)  -c $< -o $@ $(CXXFLAGS) -I $(TEST_INC_PATH)

$(TEST_OBJ_PATH)/%.o: $(TEST_SRC_PATH)/%.cpp  
	$(CXXC)  -c $< -o $@ $(CXXFLAGS) -I $(TEST_INC_PATH)

$(TEST): $(COBJS) $(CXXOBJS) $(TEST_COBJS)
	$(CXXC) -o  $@ $(CFLAGS) $(CXXFLAGS) $(COBJS) $(CXXOBJS)  $(TEST_COBJS)



.PHONY:clean
clean:
	$(RM) $(TARGET) $(COBJS) $(CXXOBJS) $(TEST) $(TEST_COBJS)
