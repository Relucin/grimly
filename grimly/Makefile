INCLUDES	+= grimly queue solver main ft_putnbr_fd
INCLUDES	+= ft_atoi ft_isdigit ft_memset
INCLUDES	+= ft_strchr ft_strlen
INCLUDES	:= $(addprefix src/, $(INCLUDES))
INCLUDES	:= $(addsuffix .o, $(INCLUDES))
CC			:= gcc
FLAGS		+= -Wall -Wextra -Werror -Iincludes/
FLAGS		+= -g
NAME		:= grimly

.PHONY: clean fclean re all

all: $(NAME)

$(NAME): $(INCLUDES)
	@echo 'Building $(NAME)'
	@$(CC) $(FLAGS) $^ -o $@

%.o: %.c
	@$(CC) $(FLAGS) -c $^ -o $@

clean:
	@echo 'Removing $(NAME)--object files'
	@rm -rf $(INCLUDES)

fclean: clean
	@echo 'Removing $(NAME)'
	@rm -rf $(NAME)

re: fclean all
