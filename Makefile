CXX = g++
# CXXFLAGS = $(shell python3-config --cflags)
# LDFLAGS = -lpython$(shell python3 -V 2>&1 | grep -oP '\d+\.\d+') -ldl -lm -lftxui-component -lftxui-dom -lftxui-screen
LDFLAGS = -lftxui-component -lftxui-dom -lftxui-screen -lmpv

SRC_DIR = src
BUILD_DIR = build

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC))
TARGET = $(BUILD_DIR)/main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
	# rm -rf $(SRC_DIR)/__pycache__
run:
	$(TARGET) /home/$(shell whoami)/Music/iknow 
