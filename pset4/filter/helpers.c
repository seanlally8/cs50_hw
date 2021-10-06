#include "helpers.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    uint8_t avg;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            	//printf("original blue value: %i\n", image[i][j].rgbtBlue);
		        //printf("original green value: %i\n", image[i][j].rgbtGreen);
	        	//printf("original red value: %i\n", image[i][j].rgbtRed);
		        avg = ((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);
            	image[i][j].rgbtBlue = round(avg);
            	image[i][j].rgbtGreen = round(avg);
            	image[i][j].rgbtRed = round(avg);
        }
    }

//	printf("%f", round(avg));
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, n = width; j < (width - (width/2)); j++)
        {
            temp = image[i][j];
            image[i][j] = image[i][j + n];
            image[i][j + n] = temp;
            n = n - 2;
        }
    }
    return;
}

void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE cp_image[height][width];
    uint8_t sumBlue;
    uint8_t sumGreen;
	uint8_t sumRed;
    uint8_t avgb;
    uint8_t avgg;
    uint8_t avgr;
    int count;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            sumBlue = 0;
            sumGreen = 0;
	    	sumRed = 0;
        	avgb = 0;
           	avgg = 0;
        	avgr = 0;
        	count = 0;

            for (int x = i - 1; x < i + 2; x++)
            {
                for (int y = j - 1; y < j + 2; y++)
                {

                    if (x < 0 || y < 0 || x > (height - 1) || y > (width - 1))
                    {
                        continue;
                    }

                    else
                    {
                       	sumRed = sumRed + image[x][y].rgbtRed;
                       	sumGreen = sumGreen + image[x][y].rgbtGreen;
                       	sumBlue = sumBlue + image[x][y].rgbtBlue;
			            count = count + 1;

                    }
                }
            }
	//printf("%i\n", count);
            	        avgr = sumRed / count;
            	        avgg = sumGreen / count;
		                avgb = sumBlue / count;

                        cp_image[i][j].rgbtRed = avgr;
            	        cp_image[i][j].rgbtGreen = avgg;
       	    	        cp_image[i][j].rgbtBlue  = avgb;
	    }
    }

    for (int m = 0; m < height; m++)
    {
        for (int p = 0; p < width; p++)
        {
            	image[m][p].rgbtRed = cp_image[m][p].rgbtRed;
            	image[m][p].rgbtGreen = cp_image[m][p].rgbtGreen;
            	image[m][p].rgbtBlue = cp_image[m][p].rgbtBlue;
	    }

    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
