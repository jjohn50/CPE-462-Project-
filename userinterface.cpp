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
using namespace std;

FILE* in, * out;
int   j, k, width, height;
int** image_in, ** image_out;
float value_out = 0.0;
int m, n;
float sum;

void high_contrast() {
	float hist[256] = { 0.0 };
	float map[256] = { 0.0 };

	for (j = 0; j < height; j++) {
		for (k = 0; k < width; k++) {
			hist[image_in[j][k]] += (1.0 / ((float)width * height));
		}
	}

	for (j = 0; j < 256; j++) {
		for (k = 0; k < j + 1; k++) {
			map[j] += hist[k];
		}
		map[j] = floor(map[j] * 255.0);
	}

	for (j = 0; j < height; j++) {
		for (k = 0; k < width; k++) {
			image_out[j][k] = (int)map[image_in[j][k]];
		}
	}
}

void left_to_right_mirror() {
	for (j = 0; j < height; j++) {
		for (k = 0; k < width / 2; k++) {
			image_out[j][k] = image_in[j][k];
		}
	}

	for (j = 0; j < height; j++) {
		for (k = width / 2; k < width; k++) {
			image_out[j][k] = image_in[j][width - 1 - k];
		}
	}
}

void right_to_left_mirror() {
	for (j = 0; j < height; j++) {
		for (k = 0; k < width / 2; k++) {
			image_out[j][k] = image_in[j][width - 1 - k];
		}
	}

	for (j = 0; j < height; j++) {
		for (k = width / 2; k < width; k++) {
			image_out[j][k] = image_in[j][k];
		}
	}
}

void top_to_bottom_mirror() {
	for (j = 0; j < height / 2; j++) {
		for (k = 0; k < width; k++) {
			image_out[j][k] = image_in[j][k];
		}
	}

	for (j = height / 2; j < height; j++) {
		for (k = 0; k < width; k++) {
			image_out[j][k] = image_in[height - 1 - j][k];
		}
	}
}

void bottom_to_top_mirror() {
	for (j = 0; j < height / 2; j++) {
		for (k = 0; k < width; k++) {
			image_out[j][k] = image_in[height - 1 - j][k];
		}
	}

	for (j = height / 2; j < height; j++) {
		for (k = 0; k < width; k++) {
			image_out[j][k] = image_in[j][k];
		}
	}
}

void burn_tool(float burn) {
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
						if (burn <= 3)
							sum += ((float)image_in[j + m - 1][k + n - 1]) / 4;
						else
							sum += ((float)image_in[j + m - 1][k + n - 1]) / burn;
					}
				value_out = sum;
				value_out = (value_out > 255 ? 255 : value_out);
				value_out = (value_out < 0 ? 0 : value_out);
				image_out[j][k] = (int)value_out;
			}
		}
}

void custom_thresholding(float thresh) {
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
}

void blur_tool() {
	int mask1 = 0;
	float mask[4][4] = { 0 }; /* any mask (filter) */
	cout << "Please enter " << 16 << " values for the Mask" << endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cin >> mask1;
			mask[i][j] = mask1;
		}
	}
	for (j = 0; j < height; j++)
		for (k = 0; k < width; k++)
		{
			if (j == 0 || k == 0 || j == height - 1 || k == width - 1)
				image_out[j][k] = image_in[j][k];
			else
			{
				sum = 0.0;
				for (m = 0; m < 3; m++)
					for (n = 0; n < 3; n++)
					{
						sum += ((float)image_in[j + m - 1][k + n - 1]) * mask[m][n] / 36.0;  /* the 36.0 is part of this filter */
					}
				value_out = sum;
				value_out = (value_out > 255 ? 255 : value_out);
				value_out = (value_out < 0 ? 0 : value_out);
				image_out[j][k] = (int)value_out;
			}
		}
}

void sharpening_tool() {
	int mask1;
	cout << "Please enter " << 1 << " value for the Mask" << endl;
	cin >> mask1;
	float mask[3][3] = { 0, -mask1, 0, -mask1, 1+(4*mask1), -mask1, 0, -mask1, 0 }; /* any mask (filter) */
	for (j = 0; j < height; j++)
		for (k = 0; k < width; k++)
		{
			if (j == 0 || k == 0 || j == height - 1 || k == width - 1)
				image_out[j][k] = image_in[j][k];
			else
			{
				sum = 0.0;
				for (m = 0; m < 3; m++)
					for (n = 0; n < 3; n++)
					{
						sum += ((float)image_in[j + m - 1][k + n - 1]) * mask[m][n];
					}
				value_out = sum;
				value_out = (value_out > 255 ? 255 : value_out);
				value_out = (value_out < 0 ? 0 : value_out);
				image_out[j][k] = (int)value_out;
			}
		}
}

int main(int argc, char *argv[])
{
	if(argc<5) { printf("ERROR: Insufficient parameters!\n"); return(1);}

	width = atoi(argv[3]);
	height = atoi(argv[4]);

	image_in = (int**) calloc(height, sizeof(int*));
	if(!image_in)
	{
		printf("Error: Can't allocate memmory!\n");
		return(1);
	}

	image_out = (int**) calloc(height, sizeof(int*));
	if(!image_out)
	{
		printf("Error: Can't allocate memmory!\n");
		return(1);
	}

	for (j=0; j<height; j++)
	{
		image_in[j] = (int *) calloc(width, sizeof(int));
		if(!image_in[j])
		{
			printf("Error: Can't allocate memmory!\n");
			return(1);
		}

		image_out[j] = (int *) calloc(width, sizeof(int));
		if(!image_out[j])
		{
			printf("Error: Can't allocate memmory!\n");
			return(1);
		}

	}

	if((in=fopen(argv[1],"rb"))==NULL)
	{
		printf("ERROR: Can't open in_file!\n");
		return(1);
	}

	if((out=fopen(argv[2],"wb"))==NULL)
	{
		printf("ERROR: Can't open out_file!\n");
		return(1);
	}

	for (j=0; j<height; j++)
		for (k=0; k<width; k++)
	    	{
			if((image_in[j][k]=getc(in))==EOF)
			{
				printf("ERROR: Can't read from in_file!\n");
				return(1);
		      }
	    	}
      if(fclose(in)==EOF)
	{
		printf("ERROR: Can't close in_file!\n");
		return(1);
	}

	/* display image_in */
	CImg<int> image_disp(width, height,1,1,0);
	/* CImg<type> image_name(width,height,temporal_frame_number,color_plane_number,initial_value) */

	for (j=0; j<height; j++)
		for (k=0; k<width; k++)
	   	{
			image_disp(k,j,0,0) = image_in[j][k];
		}
	CImgDisplay disp_in(image_disp,"Image_In",0);
	/* CImgDisplay display_name(image_displayed, "window title", normalization_factor) */


/********************************************************************/
/* Image Processing begins                                          */
/********************************************************************/
	int choice, mirror_choice;
	float burn, thresh;

	cout << "Press 1 for high contrast tool" << endl;
	cout << "Press 2 for mirror tool" << endl;
	cout << "Press 3 for burn tool" << endl;
	cout << "Press 4 for custom thresholding tool" << endl;
	cout << "Press 5 for blur tool" << endl;
	cout << "Press 6 for sharpening tool" << endl;
	cout << "Press anything else to quit" << endl;
	cin >> choice;

	switch (choice) {
	case 1:
		high_contrast();
		break;
	case 2:
		cout << "Press 1 for left-to-right mirror" << endl;
		cout << "Press 2 for right-to-left mirror" << endl;
		cout << "Press 3 for top-to-bottom mirror" << endl;
		cout << "Press 4 for bottom-to-top mirror" << endl;
		cin >> mirror_choice;
		switch (mirror_choice) {
		case 1:
			left_to_right_mirror();
			break;
		case 2:
			right_to_left_mirror();
			break;
		case 3:
			top_to_bottom_mirror();
			break;
		case 4:
			bottom_to_top_mirror();
			break;
		default:
			break;
		}
	break;
	case 3:
		cout << "Please enter how dark you would like to make your image (Any value over 3 will darken the image)" << endl;
		cin >> burn;
		burn_tool(burn);
		break;
	case 4:
		cout << "Please enter the threshold value you would like to apply (We recommend between 50 and 200)" << endl;
		cin >> thresh;
		custom_thresholding(thresh);
		break;
	case 5:
		blur_tool();
		break;
	case 6:
		sharpening_tool();
		break;
	default:
		cout << "Goodbye!" << endl;
		exit(0);
	}

/********************************************************************/
/* Image Processing ends                                          */
/********************************************************************/

	/* display image_out */
	for (j=0; j<height; j++)
		for (k=0; k<width; k++)
	   	{
			image_disp(k,j,0,0) = image_out[j][k];
		}
	CImgDisplay disp_out(image_disp,"Image_Out",0);


	/* save image_out into out_file in RAW format */
	for (j=0; j<height; j++)
		for (k=0; k<width; k++)
	    {
	            if((putc(image_out[j][k],out))==EOF)
	            {
		        	printf("ERROR: Can't write to out_file!\n");
				    return(1);
	            }
		}

    if(fclose(out)==EOF)
	{
		printf("ERROR: Can't close out_file!\n");
		return(1);
	}

	/* closing */
	while (!disp_in.is_closed)
		disp_in.wait();
	while (!disp_out.is_closed)
		disp_out.wait();

	for (j=0; j<height; j++)
	{
		free(image_in[j]);
		free(image_out[j]);
	}
	free(image_in);
	free(image_out);

    return 0;
}



