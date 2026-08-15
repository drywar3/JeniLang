
BUILD_DIR   ?= build
BIN_DIR     ?= bin
SOURCE_DIR  ?= src
INCLUDE_DIR ?= src

CXX_COMPILER ?= clang++
CXX_FLAGS = -std=c++26 -Wno-c99-designator
CXX_OPT_FLAGS    ?= -Wall -Wextra -Werror -I$(INCLUDE_DIR) \
				-Wno-unused-parameter \
				-Wno-unused-function \
				-Wno-unused-variable \
				-Wno-unused-private-field -g

CXX_FLAGS += $(CXX_OPT_FLAGS)

SOURCES != find $(SOURCE_DIR) -type f -name '*.cpp'
OBJECTS =  $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
DEPS = $(OBJECTS:.o=.o.d)

TARGET = $(BIN_DIR)/jenic
all: $(TARGET)

-include $(DEPS)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	@echo "[x]	$(basename $@)"
	@$(CXX_COMPILER) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "[x]	$(basename $@)"
	@$(CXX_COMPILER) -c -o $@ $< $(CXX_FLAGS) -MMD -MF $@.d
