# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dpotsch <poetschdavid@gmail.com>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/07 10:39:24 by dpotsch           #+#    #+#              #
#    Updated: 2025/09/24 14:51:07 by dpotsch          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv
BUILD_DIR := build


# **************************************************************************** #
all:
	@if [ ! -d build ]; then \
		$(MAKE) -s config; \
	fi
	@$(MAKE) -s build

$(NAME): all

build:
	@cmake --build build -- --no-print-directory

re: fclean all



# **************************************************************************** #
config: fclean
	@mkdir -p $(BUILD_DIR)
	@(cd $(BUILD_DIR) && cmake ..)

config_debug: fclean
	@mkdir -p $(BUILD_DIR)
	@(cd $(BUILD_DIR) && cmake -D CMAKE_BUILD_TYPE=Debug ..)

config_ubsan: fclean
	@mkdir -p $(BUILD_DIR)
	@(cd $(BUILD_DIR) && cmake -D CMAKE_BUILD_TYPE=Debug -D ENABLE_UBSAN=ON ..)



# **************************************************************************** #
clean:
	rm -rf $(BUILD_DIR)
	rm -rf .cache

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


run: build
	@printf "$(SEPARATOR)\n"
	./$(NAME) $(ARGS)

runv: build
	@printf "$(SEPARATOR)\n"
	valgrind $(VALGRINDFLAGS) ./$(NAME) $(ARGS)



# **************************************************************************** #
test: all
	(cd $(BUILD_DIR) && ctest $(ARGS))

testv: all
	(cd $(BUILD_DIR) && ctest -T memcheck)




# **************************************************************************** #
.PHONY: all $(NAME) \
				build \
				re \
				config \
				config_debug \
				config_ubsan \
				clean fclean \
				run \
				runv \
				test \
				testv


include utils.mk
