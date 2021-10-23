#include "helpers.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //Variable to capture the average of the pixel's color value
    float avg;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Calculate average color value for pixel and set each color value to that average.
            avg = ((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = round(avg);
            image[i][j].rgbtGreen = round(avg);
            image[i][j].rgbtRed = round(avg);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //Variable for swapping
    RGBTRIPLE temp;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, n = width - 1; j < (width - (width / 2)); j++)
        {
            //place the pixel to the left in a temporary variable
            temp = image[i][j];
            //place the pixel to the right in the now-empty pixel to the left
            image[i][j] = image[i][j + n];
            //place the pixel from the left side to the now-empty pixel on the right side.
            image[i][j + n] = temp;
            n = n - 2;
        }
    }
    return;
}

void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //The RGBTRIPLE variable will be used to hold the average color values, so the operations can be done on the original
    //image. Without this temporary variable, the original image would be altered, affecting the subsequent calculations and distorting the outcome.
    RGBTRIPLE cp_image[height][width];

    //The first two for loops carry us pixel by pixel through the image, row by row.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            //the "sum" variables will hold the total sum of color values, which will then be divided by "count", which is the number of color values, to get
            //the average color value. Setting values to zero, so that the same operation can be performed on the next pixel.
            float sumBlue = 0;
            float sumGreen = 0;
            float sumRed = 0;
            float avgb = 0;
            float avgg = 0;
            float avgr = 0;
            float count = 0;

            //these second two loops carry us through the 9-pixel grid surrounding the central pixel, where the avg color value will ultimately be stored.
            for (int x = i - 1; x < i + 2; x++)
            {
                for (int y = j - 1; y < j + 2; y++)
                {
                    //This conditional checks to see whether we're outside the bounds of the image[][] array. If we are outside the bounds
                    //We move to the next pixel in this loop without performing any operations and without counting this toward the average.
                    if (x < 0 || y < 0 || x > (height - 1) || y > (width - 1))
                    {
                        continue;
                    }
                    else
                    {
                        //Add up the color values to be averaged.
                        sumRed = sumRed + image[x][y].rgbtRed;
                        sumGreen = sumGreen + image[x][y].rgbtGreen;
                        sumBlue = sumBlue + image[x][y].rgbtBlue;
                        //count the number of values so that we can perform the averaging calculationg
                        count = count + 1.0;
                    }
                }
            }
            //Storing the average color values in the avg variables.
            avgr = sumRed / count;
            avgg = sumGreen / count;
            avgb = sumBlue / count;
            //Storing the averages in a temporary RGBTRIPLE variable, which will then be iterated over into the actual image[][] array
            cp_image[i][j].rgbtRed = round(avgr);
            cp_image[i][j].rgbtGreen = round(avgg);
            cp_image[i][j].rgbtBlue  = round(avgb);
        }
    }
    //Here, we're placing the average color values, pixel by pixel, into the original image[][] array.
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
    //As with blur, we want to store values in another array as we work on the original image. If we don't do this, we distort the values
    //and our calculations will be wrong.
    RGBTRIPLE cp_image[height][width];


    //These first two loops move through the array pixel by pixel row by row.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Variables to hold the resultant gx and gy values
            float gxred = 0;
            float gxgreen = 0;
            float gxblue = 0;
            float gyred = 0;
            float gygreen = 0;
            float gyblue = 0;

            //These will capture the sobel operators.
            float sobelred = 0;
            float sobelgreen = 0;
            float sobelblue = 0;

            //These second two loops chart the 9-pixel grid around (and including) the main pixel, where our sobel value will ultimately be stored.
            for (int x = i - 1; x < i + 2; x++)
            {
                for (int y = j - 1; y < j + 2; y++)
                {
                    //Check to see if pixel is outside the frame of the image[][] array. If so, assume the color values are zero, and thus the
                    //convulutional value will be zero.
                    if (x < 0 || x > height - 1 || y < 0 || y > width - 1)
                    {
                        gxred += 0;
                        gxgreen += 0;
                        gxblue += 0;
                        gyred += 0;
                        gygreen += 0;
                        gyblue += 0;
                    }
                    //Add up the convulutional values.
                    else if (x == i - 1 && y == j - 1)
                    {
                        gxred += -1 * image[x][y].rgbtRed;
                        gxgreen += -1 * image[x][y].rgbtGreen;
                        gxblue += -1 * image[x][y].rgbtBlue;
                        gyred += -1 * image[x][y].rgbtRed;
                        gygreen += -1 * image[x][y].rgbtGreen;
                        gyblue += -1 * image[x][y].rgbtBlue;
                    }
                    else if (x == i - 1 && y == j)
                    {
                        gxred += 0 * image[x][y].rgbtRed;
                        gxgreen += 0 * image[x][y].rgbtGreen;
                        gxblue += 0 * image[x][y].rgbtBlue;
                        gyred += -2 * image[x][y].rgbtRed;
                        gygreen += -2 * image[x][y].rgbtGreen;
                        gyblue += -2 * image[x][y].rgbtBlue;
                    }
                    else if (x == i - 1 && y == j + 1)
                    {
                        gxred += 1 * image[x][y].rgbtRed;
                        gxgreen += 1 * image[x][y].rgbtGreen;
                        gxblue += 1 * image[x][y].rgbtBlue;
                        gyred += -1 * image[x][y].rgbtRed;
                        gygreen += -1 * image[x][y].rgbtGreen;
                        gyblue += -1 * image[x][y].rgbtBlue;
                    }
                    else if (x == i && y == j - 1)
                    {
                        gxred += -2 * image[x][y].rgbtRed;
                        gxgreen += -2 * image[x][y].rgbtGreen;
                        gxblue += -2 * image[x][y].rgbtBlue;
                        gyred += 0 * image[x][y].rgbtRed;
                        gygreen += 0 * image[x][y].rgbtGreen;
                        gyblue += 0 * image[x][y].rgbtBlue;
                    }
                    else if (x == i && y == j)
                    {
                        gxred += 0 * image[x][y].rgbtRed;
                        gxgreen += 0 * image[x][y].rgbtGreen;
                        gxblue += 0 * image[x][y].rgbtBlue;
                        gyred += 0 * image[x][y].rgbtRed;
                        gygreen += 0 * image[x][y].rgbtGreen;
                        gyblue += 0 * image[x][y].rgbtBlue;
                    }
                    else if (x == i && y == j + 1)
                    {
                        gxred += 2 * image[x][y].rgbtRed;
                        gxgreen += 2 * image[x][y].rgbtGreen;
                        gxblue += 2 * image[x][y].rgbtBlue;
                        gyred += 0 * image[x][y].rgbtRed;
                        gygreen += 0 * image[x][y].rgbtGreen;
                        gyblue += 0 * image[x][y].rgbtBlue;
                    }
                    else if (x == i + 1 && y == j - 1)
                    {
                        gxred += -1 * image[x][y].rgbtRed;
                        gxgreen += -1 * image[x][y].rgbtGreen;
                        gxblue += -1 * image[x][y].rgbtBlue;
                        gyred += 1 * image[x][y].rgbtRed;
                        gygreen += 1 * image[x][y].rgbtGreen;
                        gyblue += 1 * image[x][y].rgbtBlue;
                    }
                    else if (x == i + 1 && y == j)
                    {
                        gxred += 0 * image[x][y].rgbtRed;
                        gxgreen += 0 * image[x][y].rgbtGreen;
                        gxblue += 0 * image[x][y].rgbtBlue;
                        gyred += 2 * image[x][y].rgbtRed;
                        gygreen += 2 * image[x][y].rgbtGreen;
                        gyblue += 2 * image[x][y].rgbtBlue;
                    }
                    else if (x == i + 1 && y == j + 1)
                    {
                        gxred += 1 * image[x][y].rgbtRed;
                        gxgreen += 1 * image[x][y].rgbtGreen;
                        gxblue += 1 * image[x][y].rgbtBlue;
                        gyred += 1 * image[x][y].rgbtRed;
                        gygreen += 1 * image[x][y].rgbtGreen;
                        gyblue += 1 * image[x][y].rgbtBlue;
                    }
                }
            }

            //Compute the sobel operator
            sobelred = sqrt(pow(gxred, 2) + pow(gyred, 2));
            sobelgreen = sqrt(pow(gxgreen, 2) + pow(gygreen, 2));
            sobelblue = sqrt(pow(gxblue, 2) + pow(gyblue, 2));

            cp_image[i][j].rgbtRed = round(sobelred);
            cp_image[i][j].rgbtGreen = round(sobelgreen);
            cp_image[i][j].rgbtBlue = round(sobelblue);

            //Cap the color value of the pixel at 255
            if (sobelred > 255)
            {
                cp_image[i][j].rgbtRed = 255;
            }
            if (sobelgreen > 255)
            {
                cp_image[i][j].rgbtGreen = 255;
            }
            if (sobelblue > 255)
            {
                cp_image[i][j].rgbtBlue = 255;
            }
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
