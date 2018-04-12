#include "Advanced.h"
#include <assert.h>

#include <stdlib.h>
#include <time.h>
#include <stdio.h>


/* Add noise to an image */
IMAGE *Noise(IMAGE *image, int n)
{
	int x, y, i;
	int num; /* number of noise added */
	num = ( n*ImageHeight(image)*ImageWidth(image) )/100;
	srand(time(NULL));

	for ( i = 0; i < num; i++ ) {
		x = ( (double)rand()/RAND_MAX )*(ImageWidth(image)-1);
		y = ( (double)rand()/RAND_MAX )*(ImageHeight(image)-1);
		SetPixelR(image, x , y, 255);
		SetPixelG(image, x, y, 255);
		SetPixelB(image, x, y, 255);
	}
	return image;

}


/* sharpen the image */
IMAGE * Sharpen(IMAGE *image)
{
	int		x, y, m, n, a, b, R, G, B;
	int		tmpR = 0;
	int		tmpG = 0;
	int		tmpB = 0;
	
	unsigned char	R_tmp[image->W][image->H];
	unsigned char	G_tmp[image->W][image->H];
	unsigned char	B_tmp[image->W][image->H];

	assert(image);
	
	IMAGE *ImageOut = NULL;
	ImageOut = CreateImage(ImageWidth(image),ImageHeight(image));

	for (y = 0; y < ImageHeight(image); y++){
		for (x = 0; x < ImageWidth(image); x++) {
			R_tmp[x][y] = GetPixelR(image,x,y);
			G_tmp[x][y] = GetPixelG(image,x,y);
			B_tmp[x][y] = GetPixelB(image,x,y);
		}
	}

	for (y = 0; y < ImageHeight(image); y++){
		for (x = 0; x < ImageWidth(image); x++){
			for (n = -1; n <= 1; n++){
				for (m = -1; m <= 1; m++) {
					a = x + m;
					b = y + n;
					if (a > image->W - 1)
						a = image->W - 1;
					if (a < 0)
						a = 0;
					if (b > image->H - 1)
						b = image->H - 1;
					if (b < 0)
						b = 0;
					
					if ((n==0)&&(m==0))
					{
						tmpR += 9*R_tmp[a][b] ; 
						tmpG += 9*G_tmp[a][b] ; 
						tmpB += 9*B_tmp[a][b] ; 
					}
					else
					{
						tmpR -= R_tmp[a][b] ; 
						tmpG -= G_tmp[a][b] ; 
						tmpB -= B_tmp[a][b] ; 
					}
				}
			}
			R = (tmpR>255)? 255: (tmpR<0)? 0: tmpR ;
			G = (tmpG>255)? 255: (tmpG<0)? 0: tmpG ;
			B = (tmpB>255)? 255: (tmpB<0)? 0: tmpB ;
			
			SetPixelR(ImageOut, x, y, R);
			SetPixelG(ImageOut, x, y, G);
			SetPixelB(ImageOut, x, y, B);

			
			tmpR = tmpG = tmpB = 0;		
		}
	}
	image = ImageOut;
	return image;


}

/* make the image posterized */
IMAGE *Posterize(IMAGE *image, unsigned int rbits, unsigned int gbits, unsigned int bbits)
{
	int x, y;
	int tmpR, tmpG, tmpB;
	for(x=0; x<ImageWidth(image); x++) {
		for(y=0; y<ImageHeight(image); y++) {
			unsigned char one = 1;
			int i;

			tmpR = GetPixelR(image, x, y);	
			tmpG = GetPixelG(image, x, y);
			tmpB = GetPixelB(image, x, y);

			for(i=0; i<rbits-1; i++) {
				tmpR |= one;
				one <<= 1;
				SetPixelR(image, x, y, tmpR);
			}
			one = ~one;
			tmpR &= one;
			SetPixelR(image, x, y, tmpR);
			
			one = 1;
			for(i=0; i<gbits-1; i++) {
				tmpG |= one;
				one <<= 1;
				SetPixelG(image, x, y, tmpG);
			}
			one = ~one;
			tmpG &= one;
			SetPixelG(image, x, y, tmpG);

			one = 1;
			for(i=0; i<bbits-1; i++) {
				tmpB |= one;
				one <<= 1;
				SetPixelB(image, x, y, tmpB);
			}
			one = ~one;
			tmpB &= one;
			SetPixelB(image, x, y, tmpB);
		}
	}
	return image;

}

/* add motion blur to the image */
IMAGE *MotionBlur(IMAGE *image, unsigned char BlurAmount)
{
	int x,y,m;
	unsigned int temp_r , temp_g , temp_b;
	unsigned int r, g, b;
	unsigned char R, G, B;

	for (x = 0; x < ImageWidth(image); x++)
        	for (y = 0; y < ImageHeight(image) ; y++) 
		{
			r = GetPixelR(image, x, y);
			g = GetPixelG(image, x, y);
			b = GetPixelB(image, x, y);
				 
			temp_r = r/2;
			temp_b = b/2;
			temp_g = g/2;
			
			for (m = 1; m<=BlurAmount ; m++)
			{
				if ((x+m) < ImageWidth(image))
				{
					R = GetPixelR(image, x+m, y);
					G = GetPixelG(image, x+m, y);
					B = GetPixelB(image, x+m, y);

					temp_r = temp_r + R * 0.5/BlurAmount;
					temp_b = temp_b + B * 0.5/BlurAmount;
					temp_g = temp_g + G * 0.5/BlurAmount;
				}
			}
			
			SetPixelR(image, x, y, temp_r);
			SetPixelG(image, x, y, temp_g);
			SetPixelB(image, x, y, temp_b);
		}	
	return image;

}

/* crop the image*/
IMAGE *Crop(IMAGE *image, int x, int y, int W, int H)
{

/*	if(x > ImageWidth(image))
	{
		x = ImageWidth(image);	
	}
	if(y > ImageHeight(image))
	{
		y = ImageHeight(image);
	}

	for(g = x-W/2; g < x+W/2; g++)
	{
		for(h = y-H/2; h < y+H/2; h++)
		{
			a = GetPixelR(image, g, h);
			b = GetPixelG(image, g, h);
			c = GetPixelB(image, g, h);
			
			SetPixelR(image, g, h, a);
			SetPixelG(image, g, h, b);
			SetPixelB(image, g, h, c);
			

		}

	}
	
	for(t = x-W/2; t < x-W/2 && t > x+W/2; t++)
	{
		for(u = y-H/2; u < y-H/2 && u > y+H/2; u++)
		{
			SetPixelR(image, t, u, 0);
			SetPixelG(image, t, u, 0);
			SetPixelB(image, t, u, 0);
		}	

	}
*/
	
	return image;

}

IMAGE *Resize(IMAGE *image, int percentage)
{	

	int resize_width;
	resize_width = (image->W)*percentage/100.00;
		
        int resize_height;
	resize_height = (image->H)*percentage/100.00;

        int a, b;

	
        IMAGE *reTmp = CreateImage(resize_width,resize_height);
        for(a=0;a<resize_width;a++){
                for(b=0;b<resize_height;b++){
                        SetPixelR(reTmp,a,b,GetPixelR(image,(a/(percentage/100.00)),b/(percentage/100.00)));
                        SetPixelG(reTmp,a,b,GetPixelG(image,(a/(percentage/100.00)),b/(percentage/100.00)));
                        SetPixelB(reTmp,a,b,GetPixelB(image,(a/(percentage/100.00)),b/(percentage/100.00)));
                }
        }
        image->W=resize_width;
        image->H=resize_height;
        image->R=reTmp->R;
        image->G=reTmp->G;
        image->B=reTmp->B;    
        return image;


}

IMAGE *BrightnessAndContrast(IMAGE *image, int brightness, int contrast)
{
	int x, y;
	double factor;
	unsigned char pixel_valueR, pixel_valueG, pixel_valueB;
	int new_pixel_valueR, new_pixel_valueG, new_pixel_valueB;

	factor = (double)(259 * (contrast + 255)) / (double)(255 * (259 - contrast));
	
	for(x = 0; x < ImageWidth(image); x++)
	{
		for(y = 0; y < ImageHeight(image); y++)
		{
			pixel_valueR = GetPixelR(image,x,y);
			pixel_valueG = GetPixelG(image,x,y);
			pixel_valueB = GetPixelB(image,x,y); 
	
			new_pixel_valueR = (int)(factor * (pixel_valueR - 128) + 128) + brightness;
			new_pixel_valueG = (int)(factor * (pixel_valueG - 128) + 128) + brightness;
			new_pixel_valueB = (int)(factor * (pixel_valueB - 128) + 128) + brightness;

			if(new_pixel_valueR > 255)
			{
				new_pixel_valueR = 255;
			}
			if(new_pixel_valueR < 0)
			{
				new_pixel_valueR = 0;
			}  
			if(new_pixel_valueG > 255)
			{
				new_pixel_valueG = 255;
			}
			if(new_pixel_valueG < 0)
			{
				new_pixel_valueG = 0;
			}
			if(new_pixel_valueB > 255)
			{
				new_pixel_valueB = 255;
			}
			if(new_pixel_valueB < 0)
			{
				new_pixel_valueB = 0;
			}
	
			SetPixelR(image,x,y,new_pixel_valueR);
			SetPixelG(image,x,y,new_pixel_valueG);
			SetPixelB(image,x,y,new_pixel_valueB);
		}
	}
return image;

}
/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
