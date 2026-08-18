##
# @file
# @version 0.1

CC      = gcc
CFLAGS  = -Wall -Wextra -O2
CPPFLAGS= -Iinclude
BUILDDIR = build

SRC = abox.c \
      applets/cat.c \
	  applets/echo.c \
      applets/ls.c \
      applets/true.c \
      applets/false.c \
      applets/rm.c \
      applets/touch.c \
	  applets/mkdir.c \
	  applets/pwd.c \
	  applets/cp.c \
	  applets/wc.c

OBJ = $(patsubst %.c,$(BUILDDIR)/%.o,$(SRC))
TARGET = $(BUILDDIR)/abox

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(OBJ) -o $@

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)

# end