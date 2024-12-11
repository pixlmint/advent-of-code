test:
	./aoc_helper.sh test

create:
	./aoc_helper.sh create

run:
	./aoc_helper.sh run

debug_test:
	./aoc_helper.sh debug_test

valgrind:
	./aoc_helper.sh valgrind

valgrindr:
	./aoc_helper.sh valgrind_run

.PHONY: test create
