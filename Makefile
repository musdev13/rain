CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -I./src
LDFLAGS = -lftxui-component -lftxui-dom -lftxui-screen -lmpv -ltag -lcurl

SRC_DIRS := src src/panels src/buttonOptions src/spotify src/soundcloud src/ytm src/osearch
BUILD_DIR := build
TARGET := $(BUILD_DIR)/rain

SRC := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))

OBJ := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRC))

DIRS := $(sort $(dir $(OBJ)))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run:
	$(TARGET) /home/$(shell whoami)/Music/test

install:
	install -Dm755 $(TARGET) /usr/bin/rain
