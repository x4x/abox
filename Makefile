##
# @file
# @version 0.1

CC      = gcc
CFLAGS  = -Wall -Wextra -O2
CPPFLAGS= -Iinclude

SRC = abox.c \
      applets/cat.c \
      applets/ls.c \
      applets/true.c \
      applets/false.c \
      applets/rm.c

OBJ = $(SRC:.c=.o)

abox: $(OBJ)
	$(CC) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -f abox $(OBJ)

# end