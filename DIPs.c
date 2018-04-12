#include "DIPs.h"
#include "Image.h"
#include "Constants.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

/* Example of DIP */
/* make the picture color black & white */
IMAGE *BlackNWhite(IMAGE *image)
{
    int x, y, tmp;
	
	assert(image);
	
	IMAGE *ImageOut = NULL;
	ImageOut = CreateImage(ImageWidth(image),ImageHeight(image));

    for (y = 0; y < ImageHeight(image); y++)
    {   
        for (x = 0; x < ImageWidth(image); x++) 
        {
            tmp = (GetPixelR(image, x, y) + GetPixelG(image, x, y) + GetPixelB(image, x, y)) / 3;
            SetPixelR(image, x, y, tmp); 
	    SetPixelG(image, x, y, tmp);
	    SetPixelB(image, x, y, tmp);
        }
    }
	return image;

}

/* reverse image color*/
IMAGE *Negative(IMAGE *image) {
    
    int x, y;
    int tempR, tempG, tempB;
    int R, G, B;

    for (y = 0; y < ImageHeight(image); y++) {
        for (x = 0; x < ImageWidth(image); x++) {
	
	    tempR = GetPixelR(image, x, y);
	    tempG = GetPixelG(image, x, y);
	    tempB = GetPixelB(image, x, y);	

            R = MAX_PIXEL - tempR;
            G = MAX_PIXEL - tempG;
            B = MAX_PIXEL - tempB;
	
	    SetPixelR(image, x, y, R);
	    SetPixelG(image, x, y, G);
	    SetPixelB(image, x, y, B);
        }
    }
	return image;

}

/*colorfiler */
IMAGE *ColorFilter (IMAGE *image,
        int target_r, int target_g, int target_b, int threshold, int replace_r, int replace_g, int replace_b) {
    
    replace_r = (replace_r > MAX_PIXEL)? MAX_PIXEL : (replace_r < 0) ? 0 : replace_r;
    replace_g = (replace_g > MAX_PIXEL)? MAX_PIXEL : (replace_g < 0) ? 0 : replace_g;
    replace_b = (replace_b > MAX_PIXEL)? MAX_PIXEL : (replace_b < 0) ? 0 : replace_b;
    
    int x, y;
    int tempR, tempG, tempB;

    for (y = 0; y < ImageHeight(image); y++){
        for (x = 0; x < ImageWidth(image); x++){

		tempR = GetPixelR(image, x, y);
		tempG = GetPixelG(image, x, y);
		tempB = GetPixelB(image, x, y);

            if (abs(tempR - target_r) <= threshold
                    && abs(tempG - target_g) <= threshold
                    && abs(tempB - target_b) <= threshold) {
                SetPixelR(image, x, y, replace_r);
                SetPixelG(image, x, y, replace_g);
                SetPixelB(image, x, y, replace_b);
            }
        }
    }
	return image;

}

/* Add edge to the image*/
IMAGE *Edge(IMAGE *image)
{   

    int             x, y, m, n, a, b, R, G, B;
    int   tmpR[ImageWidth(image)][ImageHeight(image)];
    int   tmpG[ImageWidth(image)][ImageHeight(image)];
    int   tmpB[ImageWidth(image)][ImageHeight(image)];

	assert(image);
	
	IMAGE *ImageOut = NULL;
	ImageOut = CreateImage(ImageWidth(image),ImageHeight(image));


    for (y = 0; y < ImageHeight(image); y++){
        for (x = 0; x < ImageWidth(image); x++) {
            tmpR[x][y] = GetPixelR(image,x,y);
            tmpG[x][y] = GetPixelG(image,x,y);
            tmpB[x][y] = GetPixelB(image,x,y);
		
	    SetPixelR(ImageOut,x,y,GetPixelR(image,x,y));
	    SetPixelG(ImageOut,x,y,GetPixelG(image,x,y));
	    SetPixelB(ImageOut,x,y,GetPixelB(image,x,y));
        }
    }
    int sumR = 0;   /* sum of the intensity differences with neighbors */
    int sumG = 0;
    int sumB = 0;
    for (y = 1; y < ImageHeight(image) - 1; y++){
        for (x = 1; x < ImageWidth(image) - 1; x++){
            for (n = -1; n <= 1; n++){
                for (m = -1; m <= 1; m++) {
                    a = (x + m >= image->W) ? image->W - 1 : (x + m < 0) ? 0 : x + m;
                    b = (y + n >= image->H) ? image->H - 1 : (y + n < 0) ? 0 : y + n;
                    sumR += (tmpR[x][y] - tmpR[a][b]);
                    sumG += (tmpG[x][y] - tmpG[a][b]);
                    sumB += (tmpB[x][y] - tmpB[a][b]);
                }
            }
            R = (sumR > MAX_PIXEL) ? MAX_PIXEL: (sumR < 0) ? 0: sumR;
            G = (sumG > MAX_PIXEL) ? MAX_PIXEL: (sumG < 0) ? 0: sumG;
            B = (sumB > MAX_PIXEL) ? MAX_PIXEL: (sumB < 0) ? 0: sumB;
		SetPixelR(ImageOut,x,y,R);
		SetPixelG(ImageOut,x,y,G);
		SetPixelB(ImageOut,x,y,B);
            sumR = sumG = sumB = 0;
        }
    }
    /* set all four borders to 0 */
    for (y = 0; y < ImageHeight(image); y++) {
        SetPixelR(ImageOut,0,y,0);
	SetPixelG(ImageOut,0,y,0);
	SetPixelB(ImageOut,0,y,0);

	SetPixelR(ImageOut,image->W-1,y,0);
	SetPixelG(ImageOut,image->W-1,y,0);
	SetPixelB(ImageOut,image->W-1,y,0);
    }
    for (x = 0; x < ImageWidth(image); x++) {
        SetPixelR(ImageOut,x,0,0);
	SetPixelG(ImageOut,x,0,0);
	SetPixelB(ImageOut,x,0,0);

	SetPixelR(ImageOut,x,image->H-1,0);
	SetPixelG(ImageOut,x,image->H-1,0);
	SetPixelB(ImageOut,x,image->H-1,0);
    }
	image = ImageOut;
	
return image;

}

/* flip image vertically */
IMAGE *VFlip(IMAGE *image)
{
    int             x, y;
    unsigned char   r, g, b;
    unsigned char tmpr, tmpg, tmpb;
    
    for (y = 0; y < ImageHeight(image)/2; y ++) 
    {
        for (x = 0; x < ImageWidth(image); x ++) 
        {
	    tmpr = GetPixelR(image, x, ImageHeight(image)-1-y);
	    tmpg = GetPixelG(image, x, ImageHeight(image)-1-y);
	    tmpb = GetPixelB(image, x, ImageHeight(image)-1-y);
            r = GetPixelR(image, x, y);
            g = GetPixelG(image, x, y);
            b = GetPixelB(image, x, y);
            
	    SetPixelR(image, x, ImageHeight(image)-1-y, r); 
	    SetPixelG(image, x, ImageHeight(image)-1-y, g);
	    SetPixelB(image, x, ImageHeight(image)-1-y, b);
            
            SetPixelR(image, x, y, tmpr);
            SetPixelG(image, x, y, tmpg);
            SetPixelB(image, x, y, tmpb);
        }
    }
	return image;
	
}

/* mirror image vertically */
IMAGE * VMirror(IMAGE *image) {
    int x, y;
	int R, G, B;
    for (y = 0; y < ImageHeight(image) / 2; y ++) {
        for (x = 0; x < ImageWidth(image); x ++) {
	    R = GetPixelR(image, x, y);
	    G = GetPixelG(image, x, y);
	    B = GetPixelB(image, x, y);

	    SetPixelR(image, x, ImageHeight(image)-1-y, R);
            SetPixelG(image, x, ImageHeight(image)-1-y, G);
	    SetPixelB(image, x, ImageHeight(image)-1-y, B);
        }
    }
	return image;
	
}


/* Shuffle the image */
IMAGE * Shuffle(IMAGE *image)
{
    int block_cnt = SHUFF_HEIGHT_DIV * SHUFF_WIDTH_DIV;
    int block_width = (ImageWidth(image))/SHUFF_WIDTH_DIV;
    int block_height = (ImageHeight(image))/SHUFF_HEIGHT_DIV;
    int block[SHUFF_WIDTH_DIV][SHUFF_HEIGHT_DIV];
    int i, j;
    int temp2;
    

    srand(time(NULL));
    /* Initialize block markers to not done (-1) */
    for (i = 0; i < SHUFF_WIDTH_DIV; i++) {
        for (j = 0; j< SHUFF_HEIGHT_DIV; j++) {
            block[i][j] = -1;
        }
    }

    while (block_cnt > 0) {
        /* Generate a random pair of blocks */
        int dest_height = rand() % SHUFF_HEIGHT_DIV;
        int dest_width = rand() % SHUFF_WIDTH_DIV;
        int src_height = rand() % SHUFF_HEIGHT_DIV;
        int src_width = rand() % SHUFF_WIDTH_DIV;

        /* Check if these blocks are already swaped, if not swap blocks */
        if ((block[dest_width][dest_height] == -1) && (block[src_width][src_height] == -1)) {
            /* Swap blocks */
            for (i = 0; i < block_height; i++) {
                /* Init src and dest height offset */
                int h_dest = ((dest_height * block_height) + i) % ImageHeight(image);
                int h_src  = ((src_height * block_height) + i) % ImageHeight(image); 
                for (j = 0; j < block_width; j++) {
                    int temp;
                    /* Init src and dest width offset */
                    int w_src  = ((src_width * block_width) + j) % ImageWidth(image); 
                    int w_dest = ((dest_width * block_width) + j) % ImageWidth(image);

                    temp = GetPixelR(image, w_dest, h_dest);
		    temp2 = GetPixelR(image, w_src, h_src);
                    SetPixelR(image, w_dest, h_dest, temp2);
                    SetPixelR(image, w_src, h_src, temp);

		    temp = GetPixelG(image, w_dest, h_dest);
		    temp2 = GetPixelG(image, w_src, h_src);
                    SetPixelG(image, w_dest, h_dest, temp2);
                    SetPixelG(image, w_src, h_src, temp);            
	
		    temp = GetPixelB(image, w_dest, h_dest);
		    temp2 = GetPixelB(image, w_src, h_src);
                    SetPixelB(image, w_dest, h_dest, temp2);
                    SetPixelB(image, w_src, h_src, temp);
                }
            }
            /* Set marker as done */
            block[dest_width][dest_height] = 1;
            block[src_width][src_height] = 1;
            /* Decrease block count */
            block_cnt -= 2; /* Two blocks are swapped */
        }
        
    }
	return image;
	
}

/* add border to the image */
IMAGE *AddBorder(IMAGE *image, char color[SLEN], int border_width) {
    int x, y;
    int border_r = 255;
    int border_g = 255;
    int border_b = 255;
    if (!strcmp(color, "black")) {
        border_r = 0;
        border_g = 0;
        border_b = 0;
    } 
    else if (!strcmp(color, "white")) {
        border_r = 255;
        border_g = 255;
        border_b = 255;
    }
    else if (!strcmp(color, "red")) {
        border_r = 255;
        border_g = 0;
        border_b = 0;
    }
    else if (!strcmp(color, "green")) {
        border_r = 0;
        border_g = 255;
        border_b = 0;
    }
    else if (!strcmp(color, "blue")) {
        border_r = 0;
        border_g = 0;
        border_b = 255;
    }
     else if (!strcmp(color, "yellow")) {
        border_r = 255;
        border_g = 255;
        border_b = 0;
    }
    else if (!strcmp(color, "cyan")) {
        border_r = 0;
        border_g = 255;
        border_b = 255;
    }
     else if (!strcmp(color, "pink")) {
        border_r = 255;
        border_g = 192;
        border_b = 203;
    } 
    else if (!strcmp(color, "orange")) {
        border_r = 255;
        border_g = 165;
        border_b = 0;
    } 
    else {
        printf("Unsurported color.\n");
        return image;
	
    }
    const int X_BORDER_WIDTH = border_width * 9 / 16;
    for (y = 0; y < ImageHeight(image); y++) {
        for (x = 0; x < ImageWidth(image); x++) {
            if ((y < border_width) || (y > ImageHeight(image) - 1 - border_width) ||
                    (x < X_BORDER_WIDTH) || (x > ImageWidth(image) - 1 - X_BORDER_WIDTH)) {
                SetPixelR(image, x, y, border_r);
                SetPixelG(image, x, y, border_g);
                SetPixelB(image, x, y, border_b);
            }
        }
    }
 return image;
}

/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
