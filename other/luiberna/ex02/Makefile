# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: luiberna <luiberna@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 12:32:15 by luiberna          #+#    #+#              #
#    Updated: 2025/09/15 18:56:50 by luiberna         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = PmergeMe

SRCS = main.cpp PmergeMe.cpp

CXX = c++

OBJS = $(SRCS:.cpp=.o)

CPPFLAGS = -std=c++98 -Wall -Wextra -Werror

RM = rm -f

GREEN = \033[1;32m
YELLOW = \033[1;33m
RED = \033[1;31m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Compiling $(NAME)...$(RESET)"
	@$(CXX) $(CPPFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled successfully!$(RESET)"

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@$(RM) $(OBJS)

fclean: clean
	@echo "$(RED)Cleaning executable...$(RESET)"
	@$(RM) $(NAME)
	@$(RM) *_shrubbery

re: fclean $(NAME)

.PHONY: all clean fclean re