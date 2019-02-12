#include "homework.h"

int num_threads;
int resize_factor;
unsigned char **inc;
rgb **c;
int *thread_id;
int GK[3][3] = {
    {1, 2, 1},
    {2, 4, 2},
    {1, 2, 1}
};

void readInput(const char * fileName, image *img) {
	int o;
	char colored[2];
	FILE * f = fopen(fileName, "r");
	fscanf(f, "%s\n", colored);
	if(strcmp(colored, "P5") == 0)
		img->colored = 0;
	if(strcmp(colored, "P6") == 0)
		img->colored = 1;
	fscanf(f, "%d %d\n", &img->width, &img->height);
	fscanf(f, "%d\n", &img->maxval);
	if(img->colored) {
		img->color = (rgb**) malloc(img->height * sizeof(rgb*));
		for(o = 0; o < img->height; o++) {
			img->color[o] = (rgb*) malloc(img->width * sizeof(rgb));
			fread(img->color[o], 3, img->width, f);
		}
	}
	else {
		img->incolor = (unsigned char**) calloc(img->height, sizeof(unsigned char*));
		for(o = 0; o < img->height; o++) {
			img->incolor[o] = (unsigned char*) calloc(img->width, sizeof(unsigned char));
			fread(img->incolor[o], 1, img->width, f);
		}
	}
	fclose(f);
}

void writeData(const char * fileName, image *img) {
	int o;
	FILE * f = fopen(fileName, "w");
	if(img->colored) {
		fprintf(f, "%s\n%d %d\n%d\n", "P6", img->width, img->height, img->maxval);
		for(o = 0; o < img->height; o++) 
			fwrite(img->color[o], 3, img->width, f);
	}
        else {

		fprintf(f, "%s\n%d %d\n%d\n", "P5", img->width, img->height, img->maxval);
		for(o = 0; o < img->height; o++) 
			fwrite(img->incolor[o], sizeof(unsigned char), img->width, f);

	}
	fclose(f);
}

void* threadFunction(void *img) {
	image *out = *(image **) img;	
	int x, y, z, w;
	for(y = 0; y < num_threads; y++)
		if(thread_id[y] == 0) {
			thread_id[y] = 1;
			break;
		}
		
	for(x = y; x < out->height; x = x + num_threads)
		for(y = 0; y < out->width; y = y + 1)
			if(resize_factor % 2 == 0) {
				if(out->colored) {
					int r = 0;
					int g = 0;
					int b = 0;
					for(z = resize_factor * x; z < resize_factor * (x + 1); z++)
						for(w = resize_factor * y; w < resize_factor * (y + 1); w++) {
							r += c[z][w].r;
							g += c[z][w].g;
							b += c[z][w].b;	
						}
					out->color[x][y].r = (unsigned char) (r / (resize_factor * resize_factor));
					out->color[x][y].g = (unsigned char) (g / (resize_factor * resize_factor));
					out->color[x][y].b = (unsigned char) (b / (resize_factor * resize_factor));
				}
				else {
					int incolor = 0;
					for(z = resize_factor * x; z < resize_factor * (x + 1); z++)
						for(w = resize_factor * y; w < resize_factor * (y + 1); w++) {
							incolor += inc[z][w];
						}
					out->incolor[x][y] = (unsigned char) (incolor / (resize_factor * resize_factor));
				}			

			}
			else {
				if(out->colored) {
					int r = 0;
					int g = 0;
					int b = 0;
					for(z = resize_factor * x; z < resize_factor * (x + 1); z++)
						for(w = resize_factor * y; w < resize_factor * (y + 1); w++) {
							r += c[z][w].r * GK[z % 3][w % 3];
							g += c[z][w].g * GK[z % 3][w % 3];
							b += c[z][w].b * GK[z % 3][w % 3];	
						}
					out->color[x][y].r = (unsigned char) (r / 16);
					out->color[x][y].g = (unsigned char) (g / 16);
					out->color[x][y].b = (unsigned char) (b / 16);
				}
				else {
					int incolor = 0;
					for(z = resize_factor * x; z < resize_factor * (x + 1); z++)
						for(w = resize_factor * y; w < resize_factor * (y + 1); w++) {
							incolor += inc[z][w] * GK[z % 3][w % 3];	
						}
					out->incolor[x][y] = (unsigned char) (incolor / 16);
				}

				
				
			}	

}

void resize(image *in, image *out) {

	int o;
	pthread_t tid[num_threads];
	out->colored = in->colored;
	out->maxval = in->maxval;
	out->width = (in->width - in->width % resize_factor) / resize_factor;
	out->height = (in->height - in->height % resize_factor) / resize_factor;

	if(in->colored)
		c = in->color;
	else
		inc = in->incolor;

	if(out->colored) 
		out->color = (rgb**) malloc(out->height * sizeof(rgb*));
	else 
		out->incolor = (unsigned char**) malloc(out->height * sizeof(unsigned char*));

	for(o = 0; o < out->height; o++)
		if(out->colored) 
			out->color[o] = (rgb*) malloc(out->width * sizeof(rgb));
		else 
			out->incolor[o] = (unsigned char*) malloc(out->width * sizeof(unsigned char));

	thread_id = (int *) calloc(num_threads, sizeof(int));

	for(o = 0; o < num_threads; o++) {
		pthread_create(&(tid[o]), NULL, threadFunction, &(out));
	}

	for(o = 0; o < num_threads; o++) {
		pthread_join(tid[o], NULL);
	}
	
}
