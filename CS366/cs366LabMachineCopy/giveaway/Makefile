CC	:= gcc
CFLAGS	:= -ggdb -Wall -fno-stack-protector
RM	:= rm -f

sources	:= giveaway.c exploit.c
targets	:= giveaway exploit

all: $(targets)

clean:
	$(RM) $(targets) $(source:.c=.o)

