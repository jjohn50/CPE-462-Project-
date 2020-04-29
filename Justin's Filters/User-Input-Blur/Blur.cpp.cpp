/**********************************************************************************
* imageproc.c
* Usage: imageproc in_file_name out_file_name width height
 **********************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std; 

#include "CImg.h"
using namespace cimg_library;

int main(int argc, char *argv[])
{
	FILE  *in, *out;
	int   j, k, width, height;
	int ** image_in, ** image_out;
	float value_in=0.0;
	int maskmat = 0, maskmat1 = 0; int mask1 = 0, mask2 = 0, mask3 =0, mask4 =0, mask5 =0, mask6 =0, mask7 =0, mask8 =0, mask9 =0; 
	printf("Please the [][] mask that you want"); 
	cin >> maskmat;
	cin >> maskmat1; 
	float mask[3][3] = { {mask1, mask2, mask3},
				  {mask4, mask5, mask6},
				  {mask7, mask8, mask9} }; /* any mask (filter) */
	cout << "Please enter "<< maskmat*maskmat1 << " values for the Mask";
	for (int i = 0; i < maskmat; i++) {
		for (int j = 0; j < maskmat1; j++) {
			cin >> mask1;
			mask[i][j] = mask1; 
		}
	}
	float value_out=0.0;
	int m, n;
	float sum;

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

	for (j=0; j<height; j++)
		for (k=0; k<width; k++)
	    {
			if (j==0 || k==0 || j==height-1 || k==width-1)
				image_out[j][k]=image_in[j][k];
			else
			{
				sum=0.0;
				for (m=0; m<3; m++)
					for (n=0; n<3; n++)
					{
						sum += ((float) image_in[j+m-1][k+n-1])*mask[m][n]/36.0;  /* the 36.0 is part of this filter */
					}
				value_out = sum;
				value_out = (value_out > 255 ? 255: value_out);
				value_out = (value_out < 0 ? 0: value_out);
				image_out[j][k]= (int)value_out;
			}
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



