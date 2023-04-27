CC = gcc
CFLAGS = -Wall -Werror -std=c99 -Iinclude

SRCS = src/main.c \
       src/csv.c \
       src/api.c

OBJS = $(SRCS:.c=.o)

TARGET = batch-csv-processor

# Determine the platform
ifeq ($(shell uname -s),Darwin)
    # macOS
    LIBS = $(shell pkg-config --libs libcurl) -lcsv
else
    # Linux
    LIBS = -lcurl
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

