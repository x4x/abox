##
# @file
# @version 0.1

CC      = gcc
CFLAGS  = -std=c17 -Wall -Wextra -O2
#CFLAGS  = -std=c99 -Wall -Wextra -O2
#CPPFLAGS= -Iinclude
CPPFLAGS = -Iinclude -D_POSIX_C_SOURCE=200809L  
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
	  applets/sync.c \
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