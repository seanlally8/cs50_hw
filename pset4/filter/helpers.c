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
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
		
            uint8_t sumBlue = 0;
        	uint8_t sumGreen = 0;
	    	uint8_t sumRed = 0;
        	uint8_t avgb = 0;
           	uint8_t avgg = 0;
        	uint8_t avgr = 0;
        	int count = 0;
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
			
                        //printf("count: %i\n", count);
			            //printf("original Red Value: %i\n", image[x][y].rgbtRed);
			            //printf("sumRed: %i\n", sumRed);
			            //printf("original Green Value: %i\n", image[x][y].rgbtGreen);
			            //printf("sumGreen: %i\n", sumGreen);
			            //printf("\n");
                    }
                }
            }
		
        //                printf("AVERAGES\n");
            	        avgr = sumRed / count;
		  //              printf("avg red: %i\n", avgr);
            	        avgg = sumGreen / count;
		    //            printf("avg green: %i\n", avgg);
		      //          printf("\n");
		                avgb = sumBlue / count;
            	
                        cp_image[i][j].rgbtRed = round(avgr);
            	        cp_image[i][j].rgbtGreen = round(avgg);
       	    	        cp_image[i][j].rgbtBlue  = round(avgb); 
	}
    }
    for (int m = 0; m < height; m++)
    {
        for (int p = 0; p < width; p++)
        {
            	image[m][p].rgbtRed = cp_image[m][p].rgbtRed;
            	image[m][p].rgbtGreen = cp_image[m][p].rgbtGreen;
            	image[m][p].rgbtBlue = cp_image[m][p].rgbtBlue;
 //		printf("%hhu and %hhu\n", cp_image[200][200].rgbtRed, image[200][200].rgbtRed);	
	}
	
    }
    return; 
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
