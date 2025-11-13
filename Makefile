# **************************************************************************** #
# ROOT MAKEFILE
# **************************************************************************** #

NAME := webserv
BUILD_DIR := build
export CXX := c++


# **************************************************************************** #
PRESET ?= default
BUILD_DIR_PRESET := build/$(PRESET)

.PHONY: all
all:
	@if [ ! -d $(BUILD_DIR_PRESET) ]; then \
		$(MAKE) --no-print-directory config; \
	fi
	@$(MAKE) --no-print-directory build

$(NAME): all

.PHONY: build
build:
	cmake --build --preset=$(PRESET) -j

.PHONY: re
re: fclean all



# **************************************************************************** #
.PHONY: config
config:
	cmake --preset=$(PRESET)
	@$(MAKE) --no-print-directory copy-compile-commands



# **************************************************************************** #
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	rm -rf .cache

.PHONY: fclean
fclean: clean
	rm -rf $(NAME)



# **************************************************************************** #
ARGS ?= "assets/config.json"

VALGRINDFLAGS := --errors-for-leak-kinds=all \
									--leak-check=full \
									--show-error-list=yes \
									--show-leak-kinds=all \
									--trace-children=yes \
									--track-origins=yes \
									--track-fds=all

.PHONY: run
run: all
	@printf "$(SEPARATOR)\n"
	./$(NAME) $(ARGS)

.PHONY: runv
runv: all
	@printf "$(SEPARATOR)\n"
	valgrind $(VALGRINDFLAGS) ./$(NAME) $(ARGS)



# **************************************************************************** #
.PHONY: test
test: all
	ctest --test-dir $(BUILD_DIR_PRESET) $(ARGS)

.PHONY: testv
testv: all
	ctest --test-dir $(BUILD_DIR_PRESET) -T memcheck $(ARGS)



# **************************************************************************** #
COPY_TARGET := build/compile_commands.json
copy-compile-commands:
	@if [ -f $(BUILD_DIR_PRESET)/compile_commands.json ]; then \
		echo "Copy $(BUILD_DIR_PRESET)/compile_commands.json -> $(COPY_TARGET)"; \
		cp $(BUILD_DIR_PRESET)/compile_commands.json $(COPY_TARGET); \
	else \
		echo "Error: $(BUILD_DIR_PRESET)/compile_commands.json not found. Did you run cmake --preset=$(PRESET)?"; \
		exit 1; \
	fi


include utils.mk
