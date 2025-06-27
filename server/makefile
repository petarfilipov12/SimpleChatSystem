CC = g++
CFLAGS = -std=c++11 -g

PROGRAM_NAME = SimpleChatServer

BUILD_DIR = build
CODE_DIR = code

INCLUDES = $(sort $(shell find $(CODE_DIR) -type f -name '*.h' -printf "%h\n"))
INC_FLAGS = $(patsubst %, -I%, $(INCLUDES))

CODE_SOURCES = $(shell find $(CODE_DIR) -type f -name '*.cpp')
OBJS = $(subst $(CODE_DIR), $(BUILD_DIR), $(patsubst %.cpp, %.o, $(CODE_SOURCES)))
DEPENDS = $(patsubst %.cpp, %.o, $(OBJS))


$(PROGRAM_NAME): $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(PROGRAM_NAME) $^

$(BUILD_DIR):
	mkdir -pv $(BUILD_DIR)

-include $(patsubst %.o, %.d, $(OBJS))

$(BUILD_DIR)/%.o: $(CODE_DIR)/%.cpp
	mkdir -pv $(dir $@)
	$(CC) $(CFLAGS) $(INC_FLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/%.o: $(CORE_DIR)/%.c
	mkdir -pv $(dir $@)
	$(CC) $(CFLAGS) $(INC_FLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean $(PROGRAM_NAME)

run:
	./$(PROGRAM_NAME)

print:
	echo $(INCLUDES)
	echo $(OBJS)
