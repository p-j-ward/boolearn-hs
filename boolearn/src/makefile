all: layered data data_cellauto train


layered: layered.c
	gcc -o layered layered.c -Wall -I. -lm
	
data: data.c net.c
	gcc -o data data.c net.c -Wall -I.
	
data_cellauto: data_cellauto.c
	gcc -o data_cellauto data_cellauto.c -Wall -I.
	
train: train.c net.c
	gcc -O3 -o train train.c net.c -Wall -I. -lm
