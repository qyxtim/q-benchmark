# compilers and constant flags
CC = g++
CFLAGS = -Wall -Wextra -Werror -Wreturn-type -Wunreachable-code -Wunused-parameter -Wno-unused-command-line-argument -std=c++17

# constants
SRCDIR = src
TESTDIR = test
EXT = cpp
DEPENDENCY =
INC = -I include $(DEPENDENCY)

BASE_BUILDDIR = build
BASE_TARGETDIR = bin
TARGETDIR = $(BASE_TARGETDIR)/release
BUILDDIR = $(BASE_BUILDDIR)/release
CDFLAGS = -O2 -Wno-format-security
LDFLAGS =

# debug mode
ifeq ($(debug), 1)
	TARGETDIR = $(BASE_TARGETDIR)/debug
	BUILDDIR = $(BASE_BUILDDIR)/debug
	CDFLAGS = -g -fsanitize=address -fno-omit-frame-pointer
	LDFLAGS = -fsanitize=address
endif


SOURCES = $(wildcard $(SRCDIR)/*.$(EXT) $(TESTDIR)/*.$(EXT))
SRC_OBJECTS = $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(EXT)=.o))
TEST_OBJECTS = $(patsubst $(TESTDIR)/%, $(BUILDDIR)/%, $(SRC_OBJECTS))
OBJECTS = $(TEST_OBJECTS)


all: compile link


compile:
	@echo "===> Compiling"

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(EXT)
	@mkdir -p $(BUILDDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(CDFLAGS) $(INC)

$(BUILDDIR)/%.o: $(TESTDIR)/%.$(EXT)
	@mkdir -p $(BUILDDIR)
	$(CC) -o $@ -c $< $(CFLAGS) $(CDFLAGS) $(INC)


link: $(OBJECTS)
	@echo "===> Linking"
	@mkdir -p $(TARGETDIR)
	$(eval MAINOBJECTS = $(shell nm -A $(BUILDDIR)/* | grep 'T main\|T _main' | cut -d ':' -f1))
	@$(foreach MAIN, $(MAINOBJECTS), \
		$(eval TARGET = $(subst $(BUILDDIR), $(TARGETDIR), $(MAIN))) \
		$(eval LINK = $(filter-out $(MAINOBJECTS), $(OBJECTS))) \
		echo "$(CC) -o $(TARGET:.o=) $(MAIN) $(LINK) $(LDFLAGS) $(DEPENDENCY)" ; \
		$(CC) -o $(TARGET:.o=) $(MAIN) $(LINK) $(LDFLAGS) $(DEPENDENCY); \
	)


clean:
	@echo "===> Cleaning"
	@$(RM) -r $(BASE_BUILDDIR) $(BASE_TARGETDIR)

.PHONY: clean