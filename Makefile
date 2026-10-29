NAME		= webserv

CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -I. -Iclass

SRCS		= $(shell find . -type f -name "*.cpp")

OBJ_DIR		= .obj
DEP_DIR		= .deps

OBJS		= $(patsubst ./%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
DEPS		= $(patsubst ./%.cpp, $(DEP_DIR)/%.d, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "\033[0;32m[✓] $(NAME) compilé avec succès !\033[0m"

$(OBJ_DIR)/%.o: ./%.cpp
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(patsubst $(OBJ_DIR)/%.o, $(DEP_DIR)/%.d, $@))
	$(CXX) $(CXXFLAGS) -MMD -MP -MF $(patsubst $(OBJ_DIR)/%.o, $(DEP_DIR)/%.d, $@) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(DEP_DIR)
	@echo "\033[0;33m[-] Dossiers $(OBJ_DIR) et $(DEP_DIR) supprimés.\033[0m"

fclean: clean
	rm -f $(NAME)
	@echo "\033[0;33m[-] Exécutable $(NAME) supprimé.\033[0m"

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re