CC := gcc
CFLAGS := -Wall -Wextra -O3
LDFLAGS := -lm

TARGET := cryptool

SRCS :=   des.c 2des.c 3des.c encryption.c modes.c patternLeak.c MITMattack.c desKnownTextAttack.c driver2.c aes.c conversions.c randomKey.c authenticationModes.c sbox.c
OBJS := $(SRCS:.c=.o)


all: $(TARGET)


$(TARGET): $(OBJS)
	@echo "Linking..."
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)


%.o: %.c
	@echo "Compiling..."
	$(CC) $(CFLAGS) -c $< -o $@
