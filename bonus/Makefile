# Color Definitions
RESET       := $(shell printf "\033[0m")
BOLD        := $(shell printf "\033[1m")
DIM         := $(shell printf "\033[2m")

FG_BLACK    := $(shell printf "\033[30m")
FG_RED      := $(shell printf "\033[31m")
FG_GREEN    := $(shell printf "\033[32m")
FG_YELLOW   := $(shell printf "\033[33m")
FG_BLUE     := $(shell printf "\033[34m")
FG_MAGENTA  := $(shell printf "\033[35m")
FG_CYAN     := $(shell printf "\033[36m")
FG_WHITE    := $(shell printf "\033[37m")

# Project Settings
TARGET       := bot
CXX          := c++
CURL_INSTALL_DIR := /sgoinfre/goinfre/Perso/asuc/.local
CXXFLAGS     := -Wall -Wextra -g -Werror -std=c++98 -I$(CURL_INSTALL_DIR)/include
LDFLAGS      := -L$(CURL_INSTALL_DIR)/lib
LDLIBS       := -lcurl
BUILD_DIR    := .build
OBJ_DIR      := $(BUILD_DIR)/obj
DEP_DIR      := $(BUILD_DIR)/dep

# Source and Object Files
SRCS         := main.cpp Bot.cpp
OBJS         := $(addprefix $(OBJ_DIR)/,$(SRCS:.cpp=.o))
DEPS         := $(addprefix $(DEP_DIR)/,$(SRCS:.cpp=.d))

# Default Target
all: $(TARGET)

# Debug Target
debug: CXXFLAGS += -gdwarf-4 -Wunused -Wconversion -Wfloat-equal -Wnon-virtual-dtor \
                   -Wpointer-arith -Wstrict-aliasing=2 -Wunreachable-code -Wnull-dereference \
                   -Wnonnull -Wpedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy \
                   -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations \
                   -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual \
                   -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo \
                   -Wstrict-overflow=5 -Wswitch-default -Wundef
debug: CXXFLAGS += $(shell if $(CXX) --version 2>&1 | grep -q 'clang'; \
                          then echo "-Weverything -Wno-suggest-destructor-override -Wno-suggest-override -Wno-padded"; \
                          else echo "-Wlogical-op -Wstrict-null-sentinel"; fi)
debug: re

# Build Directories
$(OBJ_DIR) $(DEP_DIR):
	@mkdir -p $@

# Object File Compilation
CURR       = 0
TOTAL      = $(words $(OBJS))
TOTAL_LEN  = $(shell echo $(TOTAL) | wc -c)
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR) $(DEP_DIR)
	@if [ $(CURR) -eq 0 ]; then \
		printf "$(BOLD)$(FG_CYAN)Compiling object files for $(TARGET)...$(RESET)\n"; \
	fi
	$(eval CURR=$(shell echo $$(($(CURR)+1))))
	$(eval PERCENTAGE=$(shell echo $$(($(CURR)*100/$(TOTAL)))))
	@printf "$(FG_GREEN)\033[2K[$(shell printf "% 3s" "$(PERCENTAGE)")%%] $(shell printf "%*d/%d" $(TOTAL_LEN) $(CURR) $(TOTAL)) Compiling $<\r$(RESET)"
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEP_DIR)/$*.d)
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MMD -MP -MF $(DEP_DIR)/$*.d -c $< -o $@

# Linking
$(TARGET): $(OBJS)
	@printf "$(FG_GREEN)\033[2K[100%%] Linking $(FG_CYAN)$(TARGET)$(RESET)\n"
	@$(CXX) $(CXXFLAGS) $(LDFLAGS)  -o $@ $^ $(LDLIBS)
	@printf "$(BOLD)$(FG_GREEN)Compilation of $(FG_CYAN)$(TARGET)$(FG_GREEN) completed.$(RESET)\n"

# Include Dependencies
-include $(DEPS)

# Clean Targets
clean:
	@rm -rf $(BUILD_DIR)
	@printf "$(FG_RED)Cleaned object and dependency files.$(RESET)\n"

fclean: clean
	@rm -f $(TARGET)
	@printf "$(FG_RED)Removed executable $(FG_CYAN)$(TARGET)$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re debug
