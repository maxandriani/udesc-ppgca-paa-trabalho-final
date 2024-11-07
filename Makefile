CC=gcc
CFLAGS= -g -o0 -Wall
SRCDIR=./src
BINDIR=./bin
PROGRAM_NAME=weblist
TEST_NAME=weblist-tests

# Generates bin folder and the main program
all: $(BINDIR) $(BINDIR)/$(PROGRAM_NAME) $(BINDIR)/$(TEST_NAME)

# Delete binary files
clean:
	rm -rf $(BINDIR) ./src/*.o

# Clean project folders before run a full build
ci: clean all

tests: clean $(BINDIR) $(BINDIR)/$(TEST_NAME)
	$(BINDIR)/$(TEST_NAME)

# Compile all C sorce files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR):
	mkdir $(BINDIR)

# Dependencies. Register here all .c file dependency
# $(SRCDIR)/dependency.o:	$(SRCDIR)/req_1.o $(SRCDIR)/req_2.o ...
$(SRCDIR)/weblist.o: $(SRCDIR)/DDLL.o

# Main build dependencies
$(BINDIR)/$(PROGRAM_NAME): $(SRCDIR)/DDLL.o $(SRCDIR)/weblist.o
	$(CC) $(CFLAGS) $? $(SRCDIR)/main.c -o $(BINDIR)/$(PROGRAM_NAME) -lm

$(BINDIR)/$(TEST_NAME): $(SRCDIR)/DDLL.o $(SRCDIR)/weblist.o
	$(CC) $(CFLAGS) $? $(SRCDIR)/tests.c -o $(BINDIR)/$(TEST_NAME) -lm