CXX:=arm-linux-gnueabihf-g++
CC:=$(CXX)
AR:=arm-linux-gnueabihf-ar

TARGET:=VoiceSpirit
SRC_DIR:=source
OBJ_DIR:=build/object
OUT_DIR:=build/output
SOURCES:=$(wildcard $(SRC_DIR)/*.cpp)
#thirdparty google start
GOOGLEAPIS_API_DIR:=thirdparty/google/api
GOOGLEAPIS_API_SRCS:=$(wildcard $(GOOGLEAPIS_API_DIR)/*.pb.cc)

GOOGLEAPIS_TYPE_DIR:=thirdparty/google/type
GOOGLEAPIS_TYPE_SRCS:=$(wildcard $(GOOGLEAPIS_TYPE_DIR)/*.pb.cc)

GOOGLEAPIS_RPC_DIR:=thirdparty/google/rpc
GOOGLEAPIS_RPC_SRCS:=$(wildcard $(GOOGLEAPIS_RPC_DIR)/*.pb.cc)

GOOGLEAPIS_SRCS:=$(GOOGLEAPIS_API_SRCS) $(GOOGLEAPIS_TYPE_SRCS) $(GOOGLEAPIS_RPC_SRCS)
GOOGLEAPIS_OBJS:=$(GOOGLEAPIS_SRCS:.cc=.o)

GOOGLEAPIS_ASSISTANT_DIR:=thirdparty/google/assistant/embedded/v1alpha2
GOOGLEAPIS_ASSISTANT_SRCS:=$(wildcard $(GOOGLEAPIS_ASSISTANT_DIR)/*.pb.cc)
GOOGLEAPIS_ASSISTANT_OBJS:=$(GOOGLEAPIS_ASSISTANT_SRCS:.cc=.o)

GRPC_GRPCPP_CFLAGS=`pkg-config --cflags grpc++ grpc`
#GRPC_GRPCPP_LDFLAGS=`pkg-config --libs grpc++ grpc`

#thirdparty google end
INC_DIR:= \
	-I./include \
	-I./thirdparty/library/include \
	-I./thirdparty \

OBJECTS:=$(addprefix $(OBJ_DIR)/,$(patsubst %.cpp,%.o,$(notdir $(SOURCES))))

ifeq ($(DEBUG), 1)#debug version, DEBUG:=1
CXXFLAGS:=-c -g -std=c++14
TARGET:=$(addprefix $(TARGET),_debug)
else#release version
CXXFLAGS:=-c -O2 -std=c++14
endif

CXXFLAGS += $(GRPC_GRPCPP_CFLAGS)
CPPFLAGS:=$(INC_DIR)

#LDFLAGS:= $(GRPC_GRPCPP_LDLAGS)

LDFLAGS:= \
	-L./thirdparty/library/static_lib \
	-lgrpc++ -lgrpc -lgrpc++_reflection -lgrpc_cronet -lgpr -lprotobuf -ldl -lares\
	-L./thirdparty/library \
	-lsnowboy-detect -lportaudio -lasound \
	-L./thirdparty/library/atlas \
	-lblas \
	-lz \
	-pthread

$(OUT_DIR)/$(TARGET):$(OBJECTS) $(GOOGLEAPIS_ASSISTANT_OBJS) googleapis.ar
	@-[ -d $(OUT_DIR) ] || mkdir -p $(OUT_DIR)
	@echo "Linking: $@"
	@$(CXX) $^ $(LDFLAGS) -o $@
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	@-[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	@echo "Compiling: $< -> $@"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -o $@
# $(GOOGLEAPIS_ASSISTANT_OBJS):$(GOOGLEAPIS_ASSISTANT_SRCS)
# 	@echo "Compiling: $< -> $@"
# 	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -o $@

.PHONY:clean
clean:
	rm -rf $(OBJ_DIR) $(OUT_DIR) $(GOOGLEAPIS_ASSISTANT_OBJS)
.PHONY:dclean
dclean:
	rm -rf $(OBJ_DIR) $(OUT_DIR) $(GOOGLEAPIS_ASSISTANT_OBJS)
	rm -f $(GOOGLEAPIS_OBJS)
	rm -f googleapis.ar
.PHONY:debug
debug:
	@$(MAKE) DEBUG=1
.PHONY:test
test:
	@echo "CXX:"
	@echo $(CXX)
	@echo "SOURCES:"
	@echo $(SOURCES)
	@echo "OBJECTS:"
	@echo $(OBJECTS)
	@echo "TARGET:"
	@echo $(TARGET)
	@echo "GOOGLEAPIS_ASSISTANT_OBJS:"
	@echo $(GOOGLEAPIS_ASSISTANT_OBJS)
	@echo "GOOGLEAPIS_API_SRCS:"
	@echo $(GOOGLEAPIS_API_SRCS)

googleapis.ar: $(GOOGLEAPIS_OBJS)
	@echo "Packaging: $< -> $@"
	@$(AR) r $@ $?
