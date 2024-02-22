all: node_alloc

node_alloc: node_alloc.c
	gcc -o $@ node_alloc.c CuTest.c

clean:
	rm -f node_alloc 
	rm -f tests/*_diff* tests/*_actual*
