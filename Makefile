# no point writing a fancy makefile since its just a single file and 1 build
# artificatetuhetu

CC := cc
# CC := cc -std=c23
# CC := cc -std=c90
# CC := cc -std=gnu17
# uncomment to switch to gcc. uncommenting this line will also officially turn
# you into a homosexual. proceed with caution. 4chan is watching.
# CC := gcc

BUILD_OUT := elog_test
MAIN_SRC := main.c

all: build run

build:
	cc -o $(BUILD_OUT) $(MAIN_SRC)

run: build
	./$(BUILD_OUT)

clean:
	rm -rvf $(BUILD_OUT)

