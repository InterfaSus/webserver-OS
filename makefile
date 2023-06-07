port = 4000

make run:
	gcc -o main main.c src/*.c -I ./include && ./main $(port) $(folder)