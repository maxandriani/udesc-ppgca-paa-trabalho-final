CC=gcc
CFLAGS= -g -o0 -Wall
SRCDIR=./src
BINDIR=./bin
PROGRAM_NAME=program

# Generates bin folder and the main program
all: $(BINDIR) $(BINDIR)/$(PROGRAM_NAME)

# Delete binary files
clean:
	rm -rf $(BINDIR) ./src/*.o

# Clean project folders before run a full build
ci: clean all

# Compile all C sorce files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR):
	mkdir $(BINDIR)

# Dependencies. Register here all .c file dependency
# $(SRCDIR)/dependency.o:	$(SRCDIR)/req_1.o $(SRCDIR)/req_2.o ...

# Main build dependencies
$(BINDIR)/$(PROGRAM_NAME): $(SRCDIR)/dep.o
	$(CC) $(CFLAGS) $? $(SRCDIR)/main.c -o $(BINDIR)/$(PROGRAM_NAME)