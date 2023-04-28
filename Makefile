TARGET  = beale

SRCDIR = ./src/
INCDIR = ./include/

CC			= gcc
CFILES		= $(wildcard $(SRCDIR)/*.c)
CFLAGS		= -Wall -Wextra -pedantic -std=c99
LDFLAGS		= -I$(INCDIR)
OBJFILES	= $(patsubst %.c, %.o, $(CFILES))

all: $(TARGET)

$(TARGET) : $(OBJFILES)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

clean:
	rm -f $(TARGET) $(OBJFILES)

purge: 
	rm -f $(TARGET) $(OBJFILES) *.txt
