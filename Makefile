BUILD_DIR=build
BUILDTYPE?=Release
#MAKE=ninja

all: build

build:
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR); cmake ../ -DCMAKE_BUILD_TYPE=$(BUILDTYPE) -G'MSYS Makefiles'  -DDISABLE_WASM=ON
	$(MAKE) -C $(BUILD_DIR) -j8

# -GNinja
# -DVERBOSE=true -DCMAKE_VERBOSE_MAKEFILE=true
# -DDISABLE_WASM=ON 
# -G'MSYS Makefiles' forces msys (cmake) to generate makefiles and use gcc instead of msbuild on windows

install:
	@$(MAKE) -C $(BUILD_DIR) install

clean:
	@$(MAKE) -C $(BUILD_DIR) clean

distclean:
	@rm -rf $(BUILD_DIR)

format:
	clang-format -i src/*.{c,h}

test:
	./$(BUILD_DIR)/tjse tests/run.js tests/

test-advanced:
	cd tests/advanced && npm install
	./$(BUILD_DIR)/tjse tests/run.js tests/advanced/

.PHONY: all build install clean distclean format test test-advanced
