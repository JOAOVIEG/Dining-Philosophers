NAME = philo

SRC_DIR = src

SRCS = $(SRC_DIR)/main.c \
		$(SRC_DIR)/parsing_and_init.c \
		$(SRC_DIR)/philo_dinner.c \
		$(SRC_DIR)/time.c \
		$(SRC_DIR)/action_routines.c \
		$(SRC_DIR)/diner_utilis.c \
		$(SRC_DIR)/diner_utilis_1.c \
		$(SRC_DIR)/ft_atoi_utilis.c \

OBJS := $(SRCS:%.c=%.o)

CFLAGS = -Wall -Wextra -Werror -g -pthread -I./inc -fsanitize=thread

all: $(NAME)

$(NAME): $(OBJS)
	cc $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean bonus fclean re