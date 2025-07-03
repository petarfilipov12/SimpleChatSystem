CC = g++
CFLAGS = -std=c++11 -g

PROGRAM_NAME = SimpleChatServer

BUILD_DIR = build
CODE_DIR = code

INCLUDES = $(sort $(shell find $(CODE_DIR) -type f -name '*.h' -printf "%h\n"))
INC_FLAGS = $(patsubst %, -I%, $(INCLUDES))

CPP_SOURCES = $(shell find $(CODE_DIR) -type f -name '*.cpp')
#CC_SOURCES = $(shell find $(CODE_DIR) -type f -name '*.cc')
OBJS = $(subst $(CODE_DIR), $(BUILD_DIR), $(patsubst %.cpp, %.o, $(CPP_SOURCES)))
#OBJS += $(subst $(CODE_DIR), $(BUILD_DIR), $(patsubst %.cc, %.o, $(CC_SOURCES)))

$(PROGRAM_NAME): $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(PROGRAM_NAME) $^ -lprotobuf

$(BUILD_DIR):
	mkdir -pv $(BUILD_DIR)

-include $(patsubst %.o, %.d, $(OBJS))

# $(BUILD_DIR)/%.o: $(CORE_DIR)/%.cc
# 	mkdir -pv $(dir $@)
# 	$(CC) $(CFLAGS) $(INC_FLAGS) -MMD -MP -c $< -o $@ -lprotobuf

$(BUILD_DIR)/%.o: $(CODE_DIR)/%.cpp
	mkdir -pv $(dir $@)
	$(CC) $(CFLAGS) $(INC_FLAGS) -MMD -MP -c $< -o $@ -lprotobuf

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean $(PROGRAM_NAME)

run:
	./$(PROGRAM_NAME)

print:
	echo $(CPP_SOURCES)
	echo $(CC_SOURCES)
	echo $(OBJS)
