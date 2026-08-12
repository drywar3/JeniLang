
BUILD_DIR   ?= build
BIN_DIR     ?= bin
SOURCE_DIR  ?= src
INCLUDE_DIR ?= src

CXX_COMPILER ?= clang++
CXX_FLAGS    ?= -Wall -Wextra -Werror -std=c++26 -I$(INCLUDE_DIR) \
				-Wno-unused-parameter \
				-Wno-unused-function \
				-Wno-unused-variable \
				-Wno-unused-private-field -g

SOURCES != find $(SOURCE_DIR) -type f -name '*.cpp'
OBJECTS =  $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
DEPS = $(OBJECTS:.o=.o.d)

TARGET = $(BIN_DIR)/aetherc
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
