#include <stdio.h>

#include "graphics.h"
#include "lists.h"
#include "gnuplot_c.h"
#include "directory.h"

const char data_folder[NAME_MAX] = "./data/";

int main()
{
	struct BMP_Picture* pic;
	size_t bmp_size;
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

	free_filenames(files, &num);	
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

	double* Imin = (double*) malloc(sizeof(double) * lenght(pictures));
	double* Imax = (double*) malloc(sizeof(double) * lenght(pictures));

	double min_tresh = 0.45f;
	double min_tresh2 = 0.4f;
	double max_tresh = 0.5f;
	double max_tresh2 = 0.95f;

	double* contrast = (double*) malloc(sizeof(double) * lenght(pictures));

	double value_read;

	for(size_t j=0; j < lenght(pictures); j++)
	{
		Imin[j] = 0;
		Imax[j] = 0;

		for(size_t i=0; i < bmp_size; i += 3)
		{
			value_read = (double) ((unsigned char*) get(pictures, j))[i];
			value_read /= 255;

			if(value_read > max_tresh && value_read < max_tresh2)
			{
				Imax[j] += value_read; 
			}
			else if(value_read < min_tresh && value_read > min_tresh2)
			{
				Imin[j] += value_read;
			}
		}
		contrast[j] = (Imax[j]-Imin[j])/(Imax[j]+Imin[j]);
		printf("Photo [%ld] : Contrast = %lf\n", j, contrast[j]);	
	}

	//Plot data:

	init_gnuplot("data.tmp");
	load_double_data(contrast, lenght(pictures));
	write_double_data_tmp();
	set_style(LINESPOINTS);
	
	set_xlabel("Lenght difference [cm]");
	set_ylabel("Contrast [a. u.]");
	set_title("Contrast in Michelson interferometer");
	plot_default();

	quit_gnuplot();
	
	free(Imin);
	free(Imax);
	free(contrast);

	free_list(pictures);

	return 0;
}
