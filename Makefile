WASM_COMPONENTS=btl fz hello pt
WASI_SDK_ROOT=

DIST_DIR=dist
CLIENT_DIR=client
SRC_DIR=src
CONF_DIR=conf
NO_CGI=0
WASI_SDK_BIN_PATH=$(WASI_SDK_ROOT)/bin
CLANG_OPTIONS=--sysroot $(WASI_SDK_ROOT)/share/wasi-sysroot
CLANG++_OPTIONS=$(CLANG_OPTIONS) --std c++23 -fno-exceptions
COMPILED_WASM_COMPONENTS=$(addprefix $(DIST_DIR)/, $(addsuffix .wasm, $(WASM_COMPONENTS)))
COMPONENT_CONFIGS=$(addprefix $(CONF_DIR)/, $(addsuffix .toml, $(WASM_COMPONENTS)))

all: build

build: prepare-build build-config $(COMPILED_WASM_COMPONENTS)

build-config: $(COMPONENT_CONFIGS)
	cat $^ >> $(DIST_DIR)/spin.toml

prepare-build:
	mkdir -p $(DIST_DIR)
	cp spin.toml $(DIST_DIR)

$(DIST_DIR)/%.wasm: $(SRC_DIR)/%.c
	$(WASI_SDK_BIN_PATH)/clang $(CLANG_OPTIONS) $< -o $@

$(DIST_DIR)/%.wasm: $(SRC_DIR)/%.cpp
	$(WASI_SDK_BIN_PATH)/clang++ $(CLANG++_OPTIONS) $< -o $@

serve-api:
	cd $(DIST_DIR) && \
	spin up

serve-client:
	cd $(CLIENT_DIR) && \
	python -m http.server --bind 0.0.0.0

deploy: build
	cd $(DIST_DIR) && \
	spin deploy

clean:
	rm -rf $(DIST_DIR)