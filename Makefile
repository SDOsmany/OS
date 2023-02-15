basic_threads: basic_threads.c
		cc basic_threads.c -o basic_threads -lpthread
main: main.c
		cc main.c -o main -lpthread