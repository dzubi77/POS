CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -pthread

SRC = main.c \
      server/moderator.h server/moderator.c \
      server/sockety/passive_socket.h server/sockety/passive_socket.c \
      server/sockety/active_socket.h server/sockety/active_socket.c \
      server/sockety/char_buffer.h server/sockety/char_buffer.c \
      server/sockety/linked_list.h \
      server/hrac.h server/hrac.c \
      server/kviz.h server/kviz.c \
      klient/sockety/client_socket.h klient/sockety/client_socket.c \
      klient/klient.c klient/klient.h \
      server/server.h server/server.c

OBJ = $(SRC:.c=.o)

EXECUTABLE = POS_SP

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)