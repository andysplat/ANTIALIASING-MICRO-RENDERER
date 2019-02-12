#ifndef HOMEWORK_H1
#define HOMEWORK_H1
#include "math.h"
#include "pthread.h"
#include "stdlib.h"
#include "stdio.h"


typedef struct {

	unsigned char ** render;

}image;

void initialize(image *im);
void render(image *im);
void writeData(const char * fileName, image *img);

#endif /* HOMEWORK_H1 */
