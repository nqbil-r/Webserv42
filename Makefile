NAME		= webserv

CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -I. -Iclass

SRCS		= $(shell find srcs -type f -name "*.cpp")

OBJ_DIR		= .obj
DEP_DIR		= .deps

OBJS		= $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))
DEPS		= $(patsubst %.cpp, $(DEP_DIR)/%.d, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "[✓] $(NAME) compilé avec succès !"

$(OBJ_DIR)/%.o: ./%.cpp
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(patsubst $(OBJ_DIR)/%.o, $(DEP_DIR)/%.d, $@))
	$(CXX) $(CXXFLAGS) -MMD -MP -MF $(patsubst $(OBJ_DIR)/%.o, $(DEP_DIR)/%.d, $@) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(DEP_DIR)
	@echo "[-] Dossiers $(OBJ_DIR) et $(DEP_DIR) supprimés."

fclean: clean
	rm -f $(NAME)
	@echo "[-] Exécutable $(NAME) supprimé."

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re