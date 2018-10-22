CC:=arm-linux-gnueabihf-g++
TARGET:=VoiceSpirit
SRC_DIR:=source
OBJ_DIR:=build/object
OUT_DIR:=build/output
SOURCES:=$(wildcard $(SRC_DIR)/*.cpp)
INC_DIR:= \
	-I./include \
	-I./thirdparty/library/include \

OBJECTS:=$(addprefix $(OBJ_DIR)/,$(patsubst %.cpp,%.o,$(notdir $(SOURCES))))

ifeq ($(DEBUG), 1)#debug version, DEBUG:=1
CC_FLAGS:=$(INC_DIR) -c -g -std=c++14
TARGET:=$(addprefix $(TARGET),_debug)
else#release version
CC_FLAGS:=$(INC_DIR) -c -std=c++14
endif

LINK_FLAGS:= \
	-L./thirdparty/library \
	-lsnowboy-detect -lportaudio -lasound -pthread \
	-L./thirdparty/library/atlas \
	-lblas

$(OUT_DIR)/$(TARGET):$(OBJECTS)
	@-[ -d $(OUT_DIR) ] || mkdir -p $(OUT_DIR)
	$(CC) $^ $(LINK_FLAGS) -o $@
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	@-[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	$(CC) $(CC_FLAGS) $< -o $@
.PHONY:clean
clean:
	rm -rf $(OBJ_DIR) $(OUT_DIR)
.PHONY:debug
debug:
	@make DEBUG=1
test:
	@echo $(SOURCES)
	@echo $(OBJECTS)
	@echo $(TARGET)
