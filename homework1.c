#include "homework1.h"

int num_threads;
int resolution;

int *thread_id;

void initialize(image *im) {

	int o;
	im->render = (unsigned char **) malloc(resolution * sizeof(unsigned char *));
	for(o = 0; o < resolution; o++)
		im->render[o] = (unsigned char *) calloc(resolution, sizeof(unsigned char ));

}

void* threadFunction(void *img) {
	image *im = *(image **) img;	
	int x, y;
	for(y = 0; y < num_threads; y++)
		if(thread_id[y] == 0) {
			thread_id[y] = 1;
			break;
		}
	
	for(x = y; x < resolution; x = x + num_threads)
		for(y = 0; y < resolution; y++)
			if(( ((float) abs( 2 * ((float) resolution - 1.0f - x + 0.5f) * 100.0f / (float) resolution - (y + 0.5f) * 100.0f / (float) resolution)) / (float) sqrt(5)) > 3.0f ) 
			
				im->render[x][y] = 255;	

}

void render(image *im) {

	int o;
	pthread_t tid[num_threads];	
	thread_id = (int *) calloc(num_threads, sizeof(int));

	for(o = 0; o < num_threads; o++) {
		pthread_create(&(tid[o]), NULL, threadFunction, &(im));
	}

	for(o = 0; o < num_threads; o++) {
		pthread_join(tid[o], NULL);
	}		

}

void writeData(const char * fileName, image *img) {

	int o;
	FILE * f = fopen(fileName, "w");
	fprintf(f, "%s\n%d %d\n%d\n", "P5", resolution, resolution, 255);
	for(o = 0; o < resolution; o++) 
		fwrite(img->render[o], sizeof(unsigned char), resolution, f);

}

