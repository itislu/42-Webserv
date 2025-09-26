# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dpotsch <poetschdavid@gmail.com>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/07 10:39:24 by dpotsch           #+#    #+#              #
#    Updated: 2025/09/28 12:08:35 by dpotsch          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv
BUILD_DIR := build


# **************************************************************************** #
.PHONY: all
all:
	@if [ ! -d build ]; then \
		$(MAKE) -s config; \
	fi
	@$(MAKE) -s build

$(NAME): all

.PHONY: build
build:
	@cmake --build build -- --no-print-directory

.PHONY: re
re: fclean all



# **************************************************************************** #
.PHONY: config
config: fclean
	@mkdir -p $(BUILD_DIR)
	@(cd $(BUILD_DIR) && cmake ..)

.PHONY: config_debug
config_debug: fclean
	@mkdir -p $(BUILD_DIR)
	@(cd $(BUILD_DIR) && cmake -D CMAKE_BUILD_TYPE=Debug ..)

.PHONY: config_ubsan
config_ubsan: fclean
	@mkdir -p $(BUILD_DIR)
	@(cd $(BUILD_DIR) && cmake -D CMAKE_BUILD_TYPE=Debug -D ENABLE_UBSAN=ON ..)



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
ARGS ?= "assets/config.json"

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
	(cd $(BUILD_DIR) && ctest $(ARGS))

.PHONY: testv
testv: all
	(cd $(BUILD_DIR) && ctest -T memcheck)

include utils.mk
