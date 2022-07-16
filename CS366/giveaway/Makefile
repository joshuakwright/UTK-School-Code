CC	:= gcc
CFLAGS	:= -ggdb -Wall -fno-stack-protector -m32
RM	:= rm -f

sources	:= giveaway.c exploit.c
targets	:= giveaway exploit

all: $(targets)

clean:
	$(RM) $(targets) $(source:.c=.o)

