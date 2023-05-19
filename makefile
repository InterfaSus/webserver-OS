port = 4000
folder = /home/fernando

make run:
	gcc -o main main.c src/*.c -I ./include && ./main $(port) $(folder)