include config.mk

STRUCTCEY_SOURCE_FILES = $(wildcard src/**/*.c) $(wildcard src/*.c)
STRUCTCEY_OBJECT_FILES = $(patsubst %.c,%.o,$(STRUCTCEY_SOURCE_FILES))
LIBSTRUCTCEY_OBJECT_FILES = $(filter-out src/main.o,$(STRUCTCEY_OBJECT_FILES))

.PHONY: main
main: check_error structcey

structcey: $(STRUCTCEY_OBJECT_FILES) libstructcey.a
	$(CC) $(CFLAGS) $(STRUCTCEY_OBJECT_FILES) -o $@

.PHONY: test
test: workshy
	make -C ./tests

libstructcey.a: $(LIBSTRUCTCEY_OBJECT_FILES)
	ar -crs $@ $^

.PHONY: workshy
workshy:
	make -C ./libs/workshy

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: check_error
check_error:
ifeq ($(IS_ERROR),1)
	$(info $(ERROR_TEXT))
else
endif

.PHONY: clean
clean:
	-find . -type f -name "*.o" -delete
	-rm structcey
	-rm *.a

	-make -C ./libs/workshy clean
	-make -C ./tests clean