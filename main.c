#include <stdio.h>

#include "graphics.h"
#include "lists.h"
#include "gnuplot_c.h"
#include "directory.h"

const char data_folder[NAME_MAX] = "./data/";

int main(int argc, char* args[])
{
	struct BMP_Picture* pic;
	size_t bmp_size;
	size_t bmp_height;
	struct List* pictures;
	
	printf("Files in folder: %ld\n", count_files(data_folder));

	size_t num = 0;
	char** files = search_files(data_folder, "bmp", &num);
	if(NULL == files[0])
	{
		perror("No bmp files to analize!\n");
		return 0;
	}
 
	char filename[NAME_MAX] = {'\0'}; 
	sprintf(filename, "%s%s.bmp", data_folder, files[0]);                                          
        printf("File: %s\n", filename);                                                         
        pic = load_bmp_image(filename);
	if(NULL == pic)
	{
		perror("Loading bmp file failed!\n");
		return -1;
	}
                                            
        bmp_size = (size_t) pic->size;
	bmp_height = (size_t) pic->height;

	pictures = init_list(bmp_size); //Only because of this line 
	push(pictures, pic->data);                                                                      
        free_bmp_image(pic);

	for(size_t i=1; i < num; i++)
	{
		sprintf(filename, "%s%s.bmp", data_folder, files[i]);
		printf("File: %s\n", filename);
		pic = load_bmp_image(filename);
		if(NULL != pic)
		{
			push(pictures, pic->data);
			free_bmp_image(pic);	
		}	
	}

	
	/*
	for(size_t j=0; j < lenght(pictures); j++)
	{
		printf("\nPicture numer %ld start: \n\n", j);
		for(size_t i=0; i < bmp_size; i++)
		{
			printf("%u ", (unsigned int) ((unsigned char*) get(pictures, j))[i] );
		}
		printf("\n\nPicture end.\n");	
	}
	*/

	//Calculations:

	double* black_pix = (double*) malloc(sizeof(double) * lenght(pictures));
	double* white_pix = (double*) malloc(sizeof(double) * lenght(pictures));

	double spliting_tresh = 0.42;

	double* grating_d = (double*) malloc(sizeof(double) * lenght(pictures));

	double value_read = 0;
	double pixel_val;
	double splits = 0;

	
	double pixel_size = 1;
	if(argc > 1)
		pixel_size = (double) atof(args[1]);
	if(pixel_size < 0)
		pixel_size = 1;

	for(size_t j=0; j < lenght(pictures); j++)
	{
		black_pix[j] = 0;
		white_pix[j] = 0;

		for(size_t i=0; i < bmp_size; i += 3)
		{
			value_read += (double) ((unsigned char*) get(pictures, j))[i];
			value_read += (double) ((unsigned char*) get(pictures, j))[i+1];
			value_read += (double) ((unsigned char*) get(pictures, j))[i+2];
			pixel_val = (double) value_read / (3.0 * 255.0);
			value_read = 0;
			
			if(pixel_val > spliting_tresh)
			{
				white_pix[j] += 1;
			}
			else
			{
				grating_d[j] += white_pix[j];
				if(white_pix[j] != 0)
					splits += 1.0;
				
				white_pix[j] = 0;
			}

			if(pixel_val < spliting_tresh)
			{
				black_pix[j] += 1;
			}
			else
			{
				grating_d[j] += black_pix[j];
				black_pix[j] = 0;
			}
			
		}
		
		printf("Split count: %lf\n", splits/bmp_height);
		if(splits != 0)
			grating_d[j] /= splits;

		grating_d[j] *= pixel_size;	
		splits = 0;

		if(pixel_size == 1)
			printf("Photo [%s] : Average pixel perioid = %lf\n", files[j], grating_d[j]);
		else if(pixel_size > 1)
			printf("Photo [%s] : Average grating constant = %lf mm\n", files[j], grating_d[j]);
		else
			printf("Photo [%s] : Average grating constant = %lf um\n", files[j], 1000 * grating_d[j]);

	
	}

	//Plot data:

	init_gnuplot("data.tmp");
	load_double_data(grating_d, lenght(pictures));
	write_double_data_tmp();
	set_style(LINESPOINTS);
	
	set_xlabel("Photo num\0");
	set_ylabel("Grating constant [pixels]\0");
	set_title("Grating constants in experiment\0");
	plot_default();

	quit_gnuplot();
	
	free(white_pix);
	free(black_pix);
	free(grating_d);
	free_filenames(files, &num);	
	free_list(pictures);

	return 0;
}
