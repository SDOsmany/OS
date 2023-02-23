semaphores: semaphores.c
		gcc semaphores.c -o semaphores -lpthread
main: main.c
		gcc main.c -o main -lpthread
