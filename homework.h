#ifndef HOMEWORK_H
#define HOMEWORK_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
	
	unsigned char r;
	unsigned char g;
	unsigned char b;

}rgb;

typedef struct {
	
	char colored;
	unsigned char ** incolor;
	rgb ** color;
	int width;
	int height;
	unsigned char maxval;

}image;

void readInput(const char * fileName, image *img);

void writeData(const char * fileName, image *img);

void resize(image *in, image * out);

#endif /* HOMEWORK_H */
