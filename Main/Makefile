CC := gcc
CFLAGS := -Wall -Wextra
LDFLAGS := -lm

TARGET := driver

SRCS :=   des.c 2des.c 3des.c encryption.c modes.c patternLeak.c MITMattack.c desKnownTextAttack.c driver.c
OBJS := $(SRCS:.c=.o)


all: $(TARGET)


$(TARGET): $(OBJS)
	@echo "Linking..."
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)


%.o: %.c
	@echo "Compiling..."
	$(CC) $(CFLAGS) -c $< -o $@
