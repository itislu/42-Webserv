# **************************************************************************** #
# ROOT MAKEFILE
# **************************************************************************** #

NAME := webserv
BUILD_DIR := build



# **************************************************************************** #
PRESET ?= default
BUILD_DIR_PRESET := build/$(PRESET)

.PHONY: all
all:
	@if [ ! -d $(BUILD_DIR_PRESET) ]; then \
		$(MAKE) -s config; \
	fi
	@$(MAKE) -s build

$(NAME): all

.PHONY: build
build:
	@MAKEFLAGS=-s cmake --build --preset=$(PRESET) 

.PHONY: re
re: fclean all



# **************************************************************************** #
.PHONY: config
config:
	cmake --preset=$(PRESET)
	@$(MAKE) -s copy-compile-commands



# **************************************************************************** #
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	rm -rf .cache

.PHONY: fclean
fclean:
	rm -rf $(BUILD_DIR)
	rm -rf $(NAME)
	rm -rf .cache



# **************************************************************************** #
ARGS ?= "resources/config/test.conf"

VALGRINDFLAGS	=	--errors-for-leak-kinds=all \
								--leak-check=full \
								--show-error-list=yes \
								--show-leak-kinds=all \
								--trace-children=yes \
								--track-origins=yes \
								--track-fds=all

.PHONY: run
run: build
	@printf "$(SEPARATOR)\n"
	./$(NAME) $(ARGS)

.PHONY: runv
runv: build
	@printf "$(SEPARATOR)\n"
	valgrind $(VALGRINDFLAGS) ./$(NAME) $(ARGS)



# **************************************************************************** #
.PHONY: test
test: all
	(cd $(BUILD_DIR_PRESET) && ctest)

.PHONY: testv
testv: all
	(cd $(BUILD_DIR_PRESET) && ctest -T memcheck)



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
