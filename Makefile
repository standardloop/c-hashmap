include Makefile.properties

all: build run

clean:
	rm main

build:
	@$(CC) $(CC_FLAGS) \
	main.c \
	hashmap.c \
	-o $(EXECUTABLE_NAME)

run:
	@./$(EXECUTABLE_NAME)

check_leaks:
	@leaks --atExit -- ./$(EXECUTABLE_NAME)
