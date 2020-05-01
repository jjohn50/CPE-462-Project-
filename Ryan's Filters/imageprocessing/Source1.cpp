/**********************************************************************************
* imageproc.c
* Usage: imageproc in_file_name out_file_name width height
 **********************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "CImg.h"
using namespace cimg_library;


int main(int argc, char* argv[])
{
	FILE* in, * out;
	int   j, k, width, height;
	int** image_in, ** image_out;
	float value_in = 0.0;
	float sum1, sum2, burn;
	long count1, count2;
	float value_out = 0.0;
	int m, n;
	float sum;

	if (argc < 5) { printf("ERROR: Insufficient parameters!\n"); return(1); }

	width = atoi(argv[3]);
	height = atoi(argv[4]);

	image_in = (int**)calloc(height, sizeof(int*));
	if (!image_in)
	{
		printf("Error: Can't allocate memmory!\n");
		return(1);
	}

	image_out = (int**)calloc(height, sizeof(int*));
	if (!image_out)
	{
		printf("Error: Can't allocate memmory!\n");
		return(1);
	}

	for (j = 0; j < height; j++)
	{
		image_in[j] = (int*)calloc(width, sizeof(int));
		if (!image_in[j])
		{
			printf("Error: Can't allocate memmory!\n");
			return(1);
		}

		image_out[j] = (int*)calloc(width, sizeof(int));
		if (!image_out[j])
		{
			printf("Error: Can't allocate memmory!\n");
			return(1);
		}

	}

	if ((in = fopen(argv[1], "rb")) == NULL)
	{
		printf("ERROR: Can't open in_file!\n");
		return(1);
	}

	if ((out = fopen(argv[2], "wb")) == NULL)
	{
		printf("ERROR: Can't open out_file!\n");
		return(1);
	}

	for (j = 0; j < height; j++)
		for (k = 0; k < width; k++)
		{
			if ((image_in[j][k] = getc(in)) == EOF)
			{
				printf("ERROR: Can't read from in_file!\n");
				return(1);
			}
		}
	if (fclose(in) == EOF)
	{
		printf("ERROR: Can't close in_file!\n");
		return(1);
	}

	/* display image_in */
	CImg<int> image_disp(width, height, 1, 1, 0);
	/* CImg<type> image_name(width,height,temporal_frame_number,color_plane_number,initial_value) */

	for (j = 0; j < height; j++)
		for (k = 0; k < width; k++)
		{
			image_disp(k, j, 0, 0) = image_in[j][k];
		}
	CImgDisplay disp_in(image_disp, "Image_In", 0);
	/* CImgDisplay display_name(image_displayed, "window title", normalization_factor) */


/********************************************************************/
/* Image Processing begins                                          */
/********************************************************************/

//Burn tool
	std::cout << "Please enter how dark you would like to make your image (the higher the number the darker the image)" << std::endl;
	std::cin >> burn;

	for (j = 0; j < height; j++)
		for (k = 0; k < width; k++)
		{
			if (j == 0 || k == 0 || j == height - 1 || k == width - 1)
				image_out[j][k] = image_in[j][k];
			else
			{
				sum = 0.0;
				for (m = 0; m < 3; m++)
					for (n = 0; n < 1; n++)
					{
						sum += ((float)image_in[j + m - 1][k + n - 1]) / burn;
					}
				value_out = sum;
				value_out = (value_out > 255 ? 255 : value_out);
				value_out = (value_out < 0 ? 0 : value_out);
				image_out[j][k] = (int)value_out;
			}
		}


	//Custom Thresholding
	float thresh;
	std::cout << "Please enter the threshold value you would like to apply (We recommend between 50 and 200)" << std::endl;
	std::cin >> thresh;

	for (j = 0; j < height; j++)
		for (k = 0; k < width; k++)
		{
			if (thresh < 50)
				image_out[j][k] = (image_in[j][k] >= 50 ? 255 : 0);
			else if (thresh > 200)
				image_out[j][k] = (image_in[j][k] >= 200 ? 255 : 0);
			else
				image_out[j][k] = (image_in[j][k] >= thresh ? 255 : 0);
		}

	/********************************************************************/
	/* Image Processing ends                                            */
	/********************************************************************/

		/* display image_out */
	for (j = 0; j < height; j++)
		for (k = 0; k < width; k++)
		{
			image_disp(k, j, 0, 0) = image_out[j][k];
		}
	CImgDisplay disp_out(image_disp, "Image_Out", 0);


	/* save image_out into out_file in RAW format */
	for (j = 0; j < height; j++)
		for (k = 0; k < width; k++)
		{
			if ((putc(image_out[j][k], out)) == EOF)
			{
				printf("ERROR: Can't write to out_file!\n");
				return(1);
			}
		}

	if (fclose(out) == EOF)
	{
		printf("ERROR: Can't close out_file!\n");
		return(1);
	}

	/* closing */
	while (!disp_in.is_closed)
		disp_in.wait();
	while (!disp_out.is_closed)
		disp_out.wait();

	for (j = 0; j < height; j++)
	{
		free(image_in[j]);
		free(image_out[j]);
	}
	free(image_in);
	free(image_out);

	return 0;
}