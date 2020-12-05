###############################################################################
############################# Makefile Template ###############################
###############################################################################
# Test name
APP_NAME = serialService

# Compiler settings - Can be customized
CC = gcc
EXT = .c
CFLAGS = -std=c11 -g -pedantic -O0 
LDFLAGS = -pthread

###############################################################################
################# Do not change anything from here downwards! #################
###############################################################################

# Directories of our project
SRCDIR = src
INCDIR = inc

# Directories that will be created to store our build output
BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
DEPDIR = $(BUILDDIR)/dep
DOXDIR = $(BUILDDIR)/dox

# Variables to clean
RM = rm -rf
LOGS = *.log

###############################################################################
################# Do not change anything from here downwards! #################
###############################################################################

# Add our current directory
DIRS += ./

# Generate the paths of every directory holding .c and .h
SRCDIRS = $(foreach dir, $(DIRS), $(addsuffix /$(SRCDIR), $(dir) ) )
INCDIRS = $(foreach dir, $(DIRS), $(addsuffix /$(INCDIR), $(dir) ) )

# Include all headers
INCS = $(foreach dir, $(INCDIRS), $(addprefix -I, $(dir) ) )

# List every .c file 
SRCS = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*$(EXT) ) )

# List every object file that will be needed and store it in the 
# user defined directory: OBJDIR 
OBJS := $(addprefix $(OBJDIR)/, $(notdir $(SRCS:$(EXT)=.o) ) )

# List every dependency file that will be needed and store it in the 
# user defined directory: DEPDIR
DEPS := $(addprefix $(DEPDIR)/, $(notdir $(SRCS:$(EXT)=.d) ) )

# Add all the directories that hold source files to VPATH. 
# This allows the makefile to search for the source files in those directories 
VPATH = $(SRCDIRS)

###############################################################################
###############################################################################
###############################################################################

all: $(APP_NAME) doc

###################### TARGET to run the testing ##############################

doc: Doxyfile
	@echo "\e[104mGenerating documentation with doxygen...\033[0m"
	@mkdir -p $(DOXDIR)
	@DOXYGEN_INPUT="$(SRCDIRS)$(INCDIRS)" DOXYDIR=$(DOXDIR) doxygen $<
	@echo "\e[102mDocumentation done.\033[0m"
	@google-chrome $(DOXDIR)/html/index.html
	@echo "$(SRCDIRS) $(INCDIRS)"


# Links the testing application
$(APP_NAME): $(OBJS)
	@echo "\e[104mLinking testing application...\033[0m"
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Creates the dependecy rules for the application
$(DEPDIR)/%.d: %$(EXT)
	@echo "\e[104mCreating dependecy rules...\033[0m"
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCS) $< -MM -MT $(@:%.d=%.o) >$@

-include $(DEPS)

# Building rule for .o app files and its .c/in combination with all .h
$(OBJDIR)/%.o: %$(EXT)
	@echo "\e[104mBuilding object files...\033[0m"
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCS) -o $@ -c $<

###################### TARGET to clean project ################################
.PHONY: clean
clean:
	$(RM) $(APP_NAME) $(BUILDDIR)
	@echo Cleaning done!
