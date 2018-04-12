#include "Image.h"
#include "FileIO.h"
#include "Constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/* Get the color intensity of the Red, Green, and Blue channels of pixel (x, y) in image */
unsigned char GetPixelR(const IMAGE *image, unsigned int x, unsigned int y)
{
	unsigned char tempR;
	/*assert...*/
	assert(image);
	assert(image->W);
	assert(image->H);
	assert(x<ImageWidth(image) && y<ImageHeight(image) && y>=0 && x>=0);
	tempR = 0;	
	return *(image->R+(image->W*y+x));
}

unsigned char GetPixelG(const IMAGE *image, unsigned int x, unsigned int y)
{
	/*assert...*/
	assert(image);
	assert(image->W);
	assert(image->H);
	assert(x<ImageWidth(image) && y<ImageHeight(image) && y>=0 && x>=0);
	return *(image->G+(image->W*y+x));
}

unsigned char GetPixelB(const IMAGE *image, unsigned int x, unsigned int y)
{
	/*assert...*/
	assert(image);
	assert(image->W);
	assert(image->H);
	assert(x<ImageWidth(image) && y<ImageHeight(image) && y>=0 && x>=0);
	return *(image->B+(image->W*y+x));
}

/* Set the color intensity of the Red, Green, and Blue channels of pixel (x, y) in image to R, G, and B */
void SetPixelR(IMAGE *image, unsigned int x, unsigned int y, unsigned char r)
{
	assert(image);
	assert(image->W);
	assert(image->H);
	assert(x<ImageWidth(image) && y<ImageHeight(image) && y>=0 && x>=0);
	assert(r >= 0 && r <= 255);

	image->R[image->W*y+x] = r;		
}

void SetPixelG(IMAGE *image, unsigned int x, unsigned int y, unsigned char g)
{
	assert(image);
	assert(image->W);
	assert(image->H);
	assert(x<ImageWidth(image) && y<ImageHeight(image) && y>=0 && x>=0);
	assert(g >= 0 && g <= 255);
	image->G[image->W*y+x] = g;	
}

void SetPixelB(IMAGE *image, unsigned int x, unsigned int y, unsigned char b)
{
	assert(image);
	assert(image->W);
	assert(image->H);
	assert(x<ImageWidth(image) && y<ImageHeight(image) && y>=0 && x>=0);
	assert(b >= 0 && b <= 255);
	image->B[image->W*y+x] = b;
}

/* Allocate dynamic memory for the image structure and its R/G/B values */
/* Return the pointer to the image, or NULL in case of error */
IMAGE *CreateImage(unsigned int Width, unsigned int Height)
{
	IMAGE *image;
	image = (IMAGE *)malloc(sizeof(IMAGE));
	if(! image)
	{
		printf("not enough memory!");
		assert(1);
	}	
		image->W = Width;
		image->H = Height;
		image->R = (unsigned char *)malloc(Width * Height * sizeof(unsigned char));
		image->G = (unsigned char *)malloc(Width * Height * sizeof(unsigned char));
		image->B = (unsigned char *)malloc(Width * Height * sizeof(unsigned char));
		
		return image;
	
}

/* Free the memory for the R/G/B values and IMAGE structure */
void DeleteImage(IMAGE *image)
{
	assert(image);
	assert(image->W);
	assert(image->H);
	free(image->R);
	free(image->G);
	free(image->B);
	free(image);
	//image->R = NULL;
	//image->G = NULL;
	//image->B = NULL;
}

/* Return the image’s width in pixels */
unsigned int ImageWidth(const IMAGE *image)
{
	assert(image);
	assert(image->W);
	assert(image->H);
	return image->W;
}

/* Return the image’s height in pixels */
unsigned int ImageHeight(const IMAGE *image)
{
	assert(image);
	assert(image->W);
	assert(image->H);
	return image->H;
}

