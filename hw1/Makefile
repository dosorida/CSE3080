cc = gcc

all: hw1_naive hw1_kmp

hw1_naive: hw1_naive.c
	$(cc) -o hw1_naive hw1_naive.c
hw1_kmp: hw1_kmp.c
	$(cc) -o hw1_kmp hw1_kmp.c

.PHONY : clean
clean :
	rm -rf hw1_naive hw1_kmp
