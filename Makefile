CC = gcc
CFLAGS = -Wall -Iinclude  	# 	Show all warnings & include headers from include/ folder

SRC = src/main.c src/task.c src/scheduler.c src/ipc.c src/uart.c src/semaphore.c src/interrupt.c src/timer.c		# Source files to compile
OUT = rtos
LOG_SRC = src/log.c
TEST_BINS = test_scheduler test_scheduler_priority test_ipc test_interrupt_timer test_semaphore test_uart test_log
ifeq ($(OS),Windows_NT)
CLEAN_FILES = $(OUT).exe $(addsuffix .exe,$(TEST_BINS))
CLEAN_COMMAND = del /Q
else
CLEAN_FILES = $(OUT) $(TEST_BINS)
CLEAN_COMMAND = rm -f
endif

all: $(OUT)			# 	Compiles the files into an output file called rtos
$(OUT): $(SRC) $(LOG_SRC)
	        $(CC) $(CFLAGS) -o $(OUT) $(SRC) src/log.c

clean: 	# 	Optional target to remove the executable
	-$(CLEAN_COMMAND) $(CLEAN_FILES)

test: $(TEST_BINS)
	./test_scheduler
	./test_scheduler_priority
	./test_ipc
	./test_interrupt_timer
	./test_semaphore
	./test_uart
	./test_log

test_scheduler: tests/test_scheduler.c src/task.c src/scheduler.c src/timer.c
	$(CC) $(CFLAGS) -o $@ $^

test_scheduler_priority: tests/test_scheduler_priority.c src/task.c src/scheduler.c src/timer.c
	$(CC) $(CFLAGS) -o $@ $^

test_ipc: tests/test_ipc.c src/ipc.c
	$(CC) $(CFLAGS) -o $@ $^

test_interrupt_timer: tests/test_interrupt_timer.c src/interrupt.c src/timer.c
	$(CC) $(CFLAGS) -o $@ $^

test_semaphore: tests/test_semaphore.c src/semaphore.c src/task.c src/scheduler.c src/timer.c
	$(CC) $(CFLAGS) -o $@ $^

test_uart: tests/test_uart.c src/uart.c
	$(CC) $(CFLAGS) -o $@ $^

test_log: tests/test_log.c src/log.c
	$(CC) $(CFLAGS) -o $@ $^