CXX = g++
WINDRES = windres
SRC_DIRS := src src/panels src/buttonOptions src/spotify src/soundcloud src/ytm src/osearch
BUILD_DIR := build
TARGET_BASE := $(BUILD_DIR)/rain
SRC := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
OBJ_LINUX := $(patsubst %.cpp,$(BUILD_DIR)/linux/%.o,$(SRC))
OBJ_WIN := $(patsubst %.cpp,$(BUILD_DIR)/win/%.o,$(SRC))
RC_FILE := src/resource.rc
RC_OBJ := $(BUILD_DIR)/win/src/resource.o

# ======== Linux Build ========
.PHONY: linux win clean run install post_win

linux: CXXFLAGS = -std=c++20 -Wall -Wextra -I./src
linux: LDFLAGS = -lftxui-component -lftxui-dom -lftxui-screen -lmpv -ltag -lcurl -lssl -lcrypto
linux: TARGET = $(TARGET_BASE)
linux: $(TARGET_BASE)

$(TARGET_BASE): $(OBJ_LINUX)
	@echo "Linking Linux: $@"
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/linux/%.o: %.cpp
	@echo "Compiling Linux: $<"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ======== Windows Build ========
win: CXXFLAGS = -std=c++20 -Wall -Wextra -I./src -IC:/msys64/mingw64/include
win: LDFLAGS = -static -static-libgcc -static-libstdc++ \
                   -LC:/msys64/mingw64/lib \
                   -lftxui-component -lftxui-dom -lftxui-screen \
                   -ltag.dll -lmpv.dll \
                   -lcurl.dll \
                   -lws2_32 -lwldap32 -lwinmm -lole32 -luuid -lstrmiids
win: TARGET = $(TARGET_BASE).exe
win: post_win

$(TARGET_BASE).exe: $(OBJ_WIN) $(RC_OBJ)
	@echo "Linking Windows: $@"
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/win/%.o: %.cpp
	@echo "Compiling Windows: $<"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(RC_OBJ): $(RC_FILE)
	@echo "Compiling resource file: $<"
	@mkdir -p $(dir $@)
	$(WINDRES) $< -O coff -o $@

post_win: $(TARGET_BASE).exe
	@echo "Copying required DLLs..."
	@mkdir -p $(dir $(TARGET))
	@ldd $(TARGET) | grep "/mingw64/bin/" | awk '{print $$3}' | while read lib; do \
		cp "$$lib" "$(dir $(TARGET))"; \
	done
	@echo "Copying CA bundle..."
	@cp /etc/pki/ca-trust/extracted/openssl/ca-bundle.trust.crt $(dir $(TARGET))/ca-bundle.crt
	@echo "Windows build completed!"

# ======== Utility Targets ========
clean:
	rm -rf $(BUILD_DIR)

run: $(TARGET_BASE)
	$(TARGET_BASE) /home/$(shell whoami)/Music/iknow

install: $(TARGET_BASE)
	install -Dm755 $(TARGET_BASE) /usr/bin/rain
